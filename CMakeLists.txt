cmake_minimum_required(VERSION 3.12)
project(kcc)

set(CMAKE_CXX_STANDARD 14)

add_executable(kcc main.cc lex.cc format.cc ast.cc parse.cc sema.cc sema.h compile.cc compile.h)