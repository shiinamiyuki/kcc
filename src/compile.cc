//
// Created by xiaoc on 2018/9/11.
//

#include "compile.h"
#include "x64-gen.h"
using namespace kcc;

void kcc::Compiler::compileFile(const char *filename) {
    std::string src;
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintln(stderr, "{} does not exist", filename);
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
    //   println("{}", ast->str());
    Sema sema;
    ast->accept(&sema);
    IRGenerator irGenerator;
    ast->accept(&irGenerator);
    irGenerator.printIR();
    //  irGenerator.buildSSA();
    DirectCodeGen gen;
    irGenerator.gen(gen);
    gen.writeFile("out");

}
