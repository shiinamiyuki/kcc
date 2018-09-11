//
// Created by xiaoc on 2018/9/11.
//

#include "compile.h"
using namespace kcc;
void kcc::Compiler::compileFile(const char *filename) {
    std::string src;
    FILE * f = fopen(filename,"r");
    if(!f){
        fprintln(stderr,"{} does not exist",filename);
    }
    while(!feof(f)){
        src += (char)fgetc(f);
    }
    println("{}",src);
    Lexer lex(src);
    lex.scan();
    Parser p(lex);
    auto ast = p.parse();
    ast->link();
    println("{}",ast->str());
}
