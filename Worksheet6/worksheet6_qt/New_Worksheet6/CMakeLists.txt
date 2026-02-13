cmake_minimum_required(VERSION 3.19)
project(worksheet6_qt LANGUAGES CXX)

# Find the necessary Qt components
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

# Standard setup for Qt projects (enables AUTOMOC, AUTOUIC, and AUTORCC)
qt_standard_project_setup()

# Define all project sources
# Added ModelPart, ModelPartList, OptionDialog, and icons.qrc [cite: 362, 680, 735]
qt_add_executable(worksheet6_qt
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    optiondialog.cpp
    optiondialog.h
    optiondialog.ui
    icons.qrc
)

# Link to Qt libraries 
target_link_libraries(worksheet6_qt
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS worksheet6_qt
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET worksheet6_qt
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})