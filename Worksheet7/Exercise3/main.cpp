// main.cpp
//
// Entry point for the Qt application.
// This file is responsible for:
//
// 1. Creating the QApplication object (required for all Qt GUI apps)
// 2. Configuring the OpenGL format needed by VTK
// 3. Creating and displaying the main window
// 4. Starting the Qt event loop
//
// The VTK + Qt integration requires one extra step:
// setting the default OpenGL format used by QVTKOpenGLNativeWidget.

#include "mainwindow.h"

#include <QApplication>

// Required so VTK can configure the correct OpenGL context
#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>

int main(int argc, char *argv[])
{
    // -------------------------------------------------------------------------
    // IMPORTANT: Configure OpenGL for VTK
    //
    // QVTKOpenGLNativeWidget requires a specific OpenGL format to work
    // correctly with Qt's rendering system. This line ensures Qt creates
    // compatible OpenGL contexts before any widgets are created.
    //
    // If this is missing, you may see rendering errors or crashes.
    // -------------------------------------------------------------------------
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    // -------------------------------------------------------------------------
    // Create the Qt application object
    //
    // QApplication manages:
    // - the GUI event loop
    // - system integration
    // - widgets and user input events
    // -------------------------------------------------------------------------
    QApplication a(argc, argv);

    // -------------------------------------------------------------------------
    // Create the main window of the application
    //
    // This window contains:
    // - the tree view (ModelPartList)
    // - the buttons
    // - the VTK rendering widget
    // -------------------------------------------------------------------------
    MainWindow w;

    // Display the main window on screen
    w.show();

    // -------------------------------------------------------------------------
    // Start the Qt event loop
    //
    // The program will remain running here until the user closes the window.
    // -------------------------------------------------------------------------
    return a.exec();
}
