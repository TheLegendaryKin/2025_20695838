#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h" // Needed for the tree model [cite: 390]

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Define slots for UI interactions [cite: 280]
public slots:
    void handleButton();
    void handleTreeClicked();
    void on_actionOpen_File_triggered(); // Automatic connection [cite: 653]
    void on_actionItem_Options_triggered(); // For context menu [cite: 767]

    // Define signal to talk to the status bar [cite: 307]
signals:
    void statusUpdateMessage(const QString & message, int timeout);

private:
    Ui::MainWindow *ui;
    ModelPartList* partList; // Pointer to our data model [cite: 389]
};
#endif
