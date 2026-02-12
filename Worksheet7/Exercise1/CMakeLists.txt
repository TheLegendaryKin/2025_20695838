cmake_minimum_required(VERSION 3.16)
project(CylinderExample CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(VTK REQUIRED COMPONENTS
  CommonCore
  FiltersSources
  InteractionStyle
  RenderingCore
  RenderingOpenGL2
)

add_executable(CylinderExample Cylinder.cxx)
target_link_libraries(CylinderExample PRIVATE ${VTK_LIBRARIES})

# VTK 9+ module auto-init (safe even if not strictly required here)
vtk_module_autoinit(
  TARGETS CylinderExample
  MODULES ${VTK_LIBRARIES}
)
