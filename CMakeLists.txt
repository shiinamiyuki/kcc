cmake_minimum_required(VERSION 3.12)
project(kcc)

set(CMAKE_CXX_STANDARD 14)

add_executable(kcc src/main.cc src/lex.cc src/format.cc src/ast.cc src/parse.cc src/sema.cc src/sema.h src/compile.cc src/compile.h src/type.cc src/type.h)