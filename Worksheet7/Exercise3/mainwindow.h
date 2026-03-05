#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h" // for tree model
#include <vtkSmartPointer.h>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class vtkRenderer;
class vtkGenericOpenGLRenderWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Define slots for UI interactions
public slots:
    void handleTreeClicked(const QModelIndex &index);
    void on_actionOpen_File_triggered(); // Automatic connection
    void on_actionItem_Options_triggered(); // For context menu
    void handleButton1();
    void handleButton2();
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index, int &actorIndex);
    // Define signal to talk to the status bar
signals:
    void statusUpdateMessage(const QString & message, int timeout);

private:
    Ui::MainWindow *ui;
    ModelPartList* partList; // Pointer to our data model
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
};
#endif
