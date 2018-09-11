#include <iostream>
#include "compile.h"
int main() {
    kcc::Compiler compiler;
    compiler.compileFile("..\\test.c");
    return 0;
}