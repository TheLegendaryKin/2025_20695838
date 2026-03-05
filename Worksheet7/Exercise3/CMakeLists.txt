cmake_minimum_required(VERSION 3.19)
project(worksheet6_qt LANGUAGES CXX)

# Find the necessary Qt components
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

# Standard setup for Qt projects (enables AUTOMOC, AUTOUIC, and AUTORCC)
qt_standard_project_setup()

#Add VTK find_package
find_package(VTK REQUIRED COMPONENTS
  CommonCore
  CommonDataModel
  FiltersSources
  InteractionStyle
  RenderingCore
  RenderingOpenGL2
  RenderingFreeType
  RenderingContextOpenGL2
  GUISupportQt
  IOGeometry
)


# Define all project sources
# Added ModelPart, ModelPartList, OptionDialog, and icons.qrc 
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

target_include_directories(worksheet6_qt PRIVATE "C:/Program Files (x86)/VTK/include/vtk-9.6")

# Link to Qt libraries 
target_link_libraries(worksheet6_qt
  PRIVATE
    Qt::Core
    Qt::Widgets
    Qt::OpenGLWidgets
    VTK::CommonCore
    VTK::CommonDataModel
    VTK::FiltersSources
    VTK::InteractionStyle
    VTK::RenderingCore
    VTK::RenderingOpenGL2
    VTK::RenderingFreeType
    VTK::RenderingContextOpenGL2
    VTK::GUISupportQt
    VTK::IOGeometry
)

vtk_module_autoinit(
  TARGETS worksheet6_qt
  MODULES
    VTK::CommonCore
    VTK::CommonDataModel
    VTK::FiltersSources
    VTK::InteractionStyle
    VTK::RenderingCore
    VTK::RenderingOpenGL2
    VTK::RenderingFreeType
    VTK::RenderingContextOpenGL2
    VTK::GUISupportQt
    VTK::IOGeometry
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

#Add VRBindings copy target
add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E
  copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/
)