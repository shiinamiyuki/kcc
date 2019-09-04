# CMakeList.txt : CMake project for kcc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)
# Add source to this project's executable.
include_directories("src/")
file(GLOB srcDir "src/*.cc")
file(GLOB fmtDir "src/fmt/*.cc")
add_executable (kcc ${srcDir} ${fmtDir})

# TODO: Add tests and install targets if needed.
