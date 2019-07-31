//
// Created by xiaoc on 2018/9/11.
//

#include "compile.h"
using namespace kcc;

void kcc::Compiler::compileFile(const char *filename) {
    std::string src;
    FILE *f = fopen(filename, "r");
    if (!f) {
        fmt::print(stderr, "{} does not exist\n", filename);
    }
    while (!feof(f)) {
        char c = (char) fgetc(f);
        if (c != EOF && c)
            src += c;
    }
    Lexer lex(filename, src);
    lex.scan();
    Parser p(lex);
    auto ast = p.parse();
    ast->link();
    fmt::print("{}\n", ast->str());
  /*  Sema sema;
    ast->accept(&sema);
    IRGenerator irGenerator;
    ast->accept(&irGenerator);
    irGenerator.printIR();*/
  //  irGenerator.buildSSA();

}
