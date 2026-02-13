#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox> // For Exercise 2 [cite: 290]
#include <QFileDialog> // For Exercise 8 [cite: 708]
#include "optiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Initialize the Model [cite: 394, 395]
    this->partList = new ModelPartList("PartsList");
    ui->treeView->setModel(this->partList);

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

    // 2. Connect status bar signal [cite: 325, 326]
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);

    // 3. Manual connection for the button [cite: 286]
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);

    // 4. Connect Tree View click [cite: 441]
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    // 5. Add action to tree view for context menu [cite: 771]
    ui->treeView->addAction(ui->actionItem_Options);
}

// Example slot implementation [cite: 330, 334]
void MainWindow::handleButton() {
    emit statusUpdateMessage(QString("Button was clicked"), 0);
}

void MainWindow::handleTreeClicked() {
    // Get the index of the selected item [cite: 490]
    QModelIndex index = ui->treeView->currentIndex();

    // Get a pointer to the actual ModelPart object from the index [cite: 491, 492]
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Retrieve the name string (stored in column 0) [cite: 494]
    QString text = selectedPart->data(0).toString();

    // Update the status bar [cite: 495]
    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}
// ... We will implement handleTreeClicked and the others next.

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionOpen_File_triggered() {
    // Open the standard file dialog [cite: 709, 712, 713]
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);; Text Files (*.txt)")
        );

    if (!fileName.isEmpty()) {
        // For now, just show the filename in the status bar [cite: 720]
        emit statusUpdateMessage(QString("File selected: ") + fileName, 0);

        // FUTURE STEP: We will later use the selected tree item
        // and update its name property to reflect this filename[cite: 852].
    }

    if (!fileName.isEmpty()) {
        QModelIndex index = ui->treeView->currentIndex();
        if (index.isValid()) {
            ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

            // Extract just the filename from the full path
            QFileInfo fileInfo(fileName);
            selectedPart->set(0, fileInfo.fileName());

            // Refresh the tree view
            emit partList->dataChanged(index, index);
            emit statusUpdateMessage(QString("Updated item name to: ") + fileInfo.fileName(), 0);
        }
    }
}

void MainWindow::on_actionItem_Options_triggered() {
    // 1. Identify which item was right-clicked
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;

    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // 2. Create and initialize the dialog [cite: 736]
    OptionDialog dialog(this);
    dialog.updateFromPart(selectedPart);

    // 3. Show the dialog and check if "OK" was pressed [cite: 737]
    if (dialog.exec() == QDialog::Accepted) {
        dialog.updatePart(selectedPart);

        // Notify the tree view that data has changed so it refreshes the display
        emit partList->dataChanged(index, index);
        emit statusUpdateMessage(QString("Properties updated for: ") + selectedPart->data(0).toString(), 0);
    } else {
        emit statusUpdateMessage(QString("Edit cancelled"), 0);
    }
}

