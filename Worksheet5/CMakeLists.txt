cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(worksheet5_installer)

# Build targets
add_library(maths adder.cpp)
add_executable(calc calc.cpp)
target_link_libraries(calc maths)

# Installation rules
install(TARGETS calc maths
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
)

# Packaging via CPack
if(WIN32)
  set(CPACK_GENERATOR "NSIS")
else()
  set(CPACK_GENERATOR "ZIP")
endif()

include(CPack)
