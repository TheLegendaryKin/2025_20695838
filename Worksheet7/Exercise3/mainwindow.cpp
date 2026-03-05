#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>   // (kept) you were using this for earlier worksheet exercises
#include <QFileDialog>   // For "Open File" action
#include "optiondialog.h"
#include <QFileInfo>     // Extract filename from a full path

// Qt + VTK widget support
#include <QVTKOpenGLNativeWidget.h>

// VTK core rendering pipeline includes (existing cylinder demo)
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

// VTK STL loading (Exercise 4)
#include <vtkSTLReader.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // -------------------------------------------------------------------------
    // PART 1: Build the Qt model that drives the tree view (your existing work)
    // -------------------------------------------------------------------------

    // Create the model that backs the tree view.
    // The tree view will display whatever data ModelPartList exposes.
    this->partList = new ModelPartList("PartsList");
    ui->treeView->setModel(this->partList);

    // Get root node and populate with dummy items (as you already had).
    // This will later become your "CAD parts" list.
    ModelPart* rootItem = this->partList->getRootItem();

    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible }, rootItem);
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString childName = QString("Item %1,%2").arg(i).arg(j);
            ModelPart* childChildItem = new ModelPart({ childName, visible }, childItem);
            childItem->appendChild(childChildItem);
        }
    }

    // -------------------------------------------------------------------------
    // PART 2: Hook up UI signals to slots (your existing work)
    // -------------------------------------------------------------------------

    // Status bar message relay.
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    // Button example (you had two buttons and a generic button handler)
    connect(ui->pushButton,   &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);

    // Tree view click:
    // QTreeView::clicked emits clicked(const QModelIndex &index)
    // so our slot should accept a QModelIndex argument.
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);

    // Allow context-menu action to be available on the tree view.
    ui->treeView->addAction(ui->actionItem_Options);

    // -------------------------------------------------------------------------
    // PART 3: VTK inside Qt (Exercise 3.3.2 foundation)
    //
    // Your existing code created local renderer/renderWindow objects.
    // That works for a one-off cylinder demo, but Exercise 4 needs us to
    // refresh the render output repeatedly when STL files are added.
    //
    // Therefore:
    // - we keep your cylinder pipeline for initial proof-of-life
    // - we ALSO store the renderer / renderWindow into MainWindow member
    //   variables (declared in mainwindow.h) so updateRender() can use them.
    //
    // IMPORTANT: For this file to compile, you must add these members in
    // mainwindow.h:
    //   vtkSmartPointer<vtkRenderer> renderer;
    //   vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    // and include <vtkSmartPointer.h> there.
    // -------------------------------------------------------------------------

    // Create a VTK OpenGL render window compatible with Qt's OpenGL context.
    // Store it in the MainWindow member so we can re-render later.
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);

    // Create a renderer and attach it to the render window.
    // Store it in the MainWindow member so we can add/remove actors later.
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.05, 0.05, 0.05); // dark background
    ui->widget->renderWindow()->AddRenderer(renderer);

    // -------------------------------------------------------------------------
    // Initial demo geometry (cylinder) to prove VTK rendering works in Qt.
    // After you implement STL loading, updateRender() will replace this with
    // whatever is in the tree (STL actors).
    // -------------------------------------------------------------------------

    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(64); // higher resolution = smoother cylinder

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
    renderer->ResetCamera();

    // Render once so you immediately see something in the widget.
    ui->widget->renderWindow()->Render();

    // Optional: leave a message so you know VTK is initialised.
    emit statusUpdateMessage("VTK widget initialised: cylinder rendered.", 0);
}

MainWindow::~MainWindow()
{
    // Note: partList is owned by you. If you want to be tidy, delete it here
    // (assuming it isn't parented into Qt's QObject ownership elsewhere).
    // delete partList;
    delete ui;
}

// -----------------------------------------------------------------------------
// Exercise 4: Rendering update helpers
//
// The worksheet expects a function that clears the renderer, walks the model
// tree, and re-adds every ModelPart's actor into the renderer, then renders.
// -----------------------------------------------------------------------------

void MainWindow::updateRender()
{
    // Safety check: if renderer is not initialised yet, do nothing.
    if (!renderer) {
        emit statusUpdateMessage("Renderer not initialised.", 0);
        return;
    }

    // Remove everything currently displayed.
    renderer->RemoveAllViewProps();

    // Count all actors (used to space items apart so they do not overlap)
    int actorIndex = 0;

    // Walk the model tree from the first top-level item and add any actors found.
    // NEW: actorIndex must be passed by reference because updateRenderFromTree uses it.
    //updateRenderFromTree(partList->index(0, 0, QModelIndex()), actorIndex);
    QModelIndex root = QModelIndex();
    int topRows = partList->rowCount(root);
    for (int i = 0; i < topRows; i++) {
        updateRenderFromTree(partList->index(i, 0, root), actorIndex);
    }

    // Reset camera so newly added geometry is visible.
    renderer->ResetCamera();

    // Force an immediate redraw of the VTK widget.
    ui->widget->renderWindow()->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex &index, int &actorIndex)
{
    // If this is a valid node, attempt to retrieve the ModelPart and its actor.
    if (index.isValid()) {
        ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
        if (part) {
            // IMPORTANT:
            // This assumes you will add a method in ModelPart:
            //   vtkActor* getActor() const;
            //
            // And that loadSTL() creates an actor and stores it in the ModelPart.
            vtkActor* a = part->getActor();
            if (a != nullptr) {
                // spread items across X axis to prevent overlap
                const double spacing = 2.5;
                a->SetPosition(actorIndex * spacing, 0.0, 0.0);
                actorIndex++;

                renderer->AddActor(a);
            }
        }
    }

    // If no children, stop recursion.
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    // Recurse into children.
    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        // NEW: must pass actorIndex into recursive call as well
        updateRenderFromTree(partList->index(i, 0, index), actorIndex);
    }
}

// -----------------------------------------------------------------------------
// UI slots / handlers
// -----------------------------------------------------------------------------

void MainWindow::handleButton1()
{
    emit statusUpdateMessage("Button 1 Clicked!", 0);
}

void MainWindow::handleButton2()
{
    emit statusUpdateMessage("Button 2 Clicked!", 0);
}

// IMPORTANT: This signature must match the signal:
// QTreeView::clicked(const QModelIndex &index)
void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        emit statusUpdateMessage("No valid item selected.", 0);
        return;
    }

    // internalPointer() is used by custom models to expose their underlying node.
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) {
        emit statusUpdateMessage("Error: could not resolve selected ModelPart.", 0);
        return;
    }

    // Column 0 stores the display name in your model.
    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage(QString("Selected item: ") + text, 0);
}

void MainWindow::on_actionOpen_File_triggered()
{
    // Exercise 4 requirement:
    // - Get filename selected
    // - Add a new item to tree view (ideally as child of currently selected)
    // - Call loadSTL() on newly created item
    // - Refresh renderer so the STL appears immediately

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open STL File"),
        "C:\\",
        tr("STL Files (*.stl)")
        );

    if (fileName.isEmpty()) {
        emit statusUpdateMessage("No file selected.", 0);
        return;
    }

    // Choose parent: selected item if there is one, otherwise root.
    ModelPart* parentPart = partList->getRootItem();
    QModelIndex parentIndex = ui->treeView->currentIndex();

    if (parentIndex.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(parentIndex.internalPointer());
        if (selectedPart) {
            parentPart = selectedPart;
        }
    }

    // Create new ModelPart node as a child of parentPart.
    QFileInfo fileInfo(fileName);
    QString visible("true");

    ModelPart* newPart = new ModelPart({ fileInfo.fileName(), visible }, parentPart);
    parentPart->appendChild(newPart);

    // Ask the new item to load its STL and build its VTK pipeline.
    // This requires you to implement ModelPart::loadSTL(const QString&).

    const QString stlPath = fileName;
    newPart->loadSTL(stlPath);

    // Update the Qt view so the new item appears immediately.
    ui->treeView->viewport()->update();

    // Re-render the VTK viewport from the tree contents.
    updateRender();

    emit statusUpdateMessage(QString("Loaded STL: ") + fileInfo.fileName(), 0);
}

void MainWindow::on_actionItem_Options_triggered()
{
    // Identify which item is currently selected.
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected.", 0);
        return;
    }

    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) {
        emit statusUpdateMessage("Error: selected item is invalid.", 0);
        return;
    }

    // Create the dialog and load current properties from the selected part.
    OptionDialog dialog(this);
    dialog.updateFromPart(selectedPart);

    // If user pressed OK, write dialog values back into the selected part.
    if (dialog.exec() == QDialog::Accepted) {
        dialog.updatePart(selectedPart);

        // Refresh the tree row.
        emit partList->dataChanged(index, index);

        // After options change (for example visibility), re-render from the tree.
        // This becomes important once you wire the Visible? column into actor visibility.
        updateRender();

        emit statusUpdateMessage(
            QString("Properties updated for: ") + selectedPart->data(0).toString(), 0
            );
    } else {
        emit statusUpdateMessage("Edit cancelled.", 0);
    }
}
