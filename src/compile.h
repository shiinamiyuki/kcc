//
// Created by xiaoc on 2018/9/11.
//

#ifndef KCC_COMPILE_H
#define KCC_COMPILE_H

#include "lex.h"
#include "parse.h"
//#include "sema.h"
//#include "ir-gen.h"
namespace  kcc{
    class Compiler{
    public:
        void compileFile(const char * filename);
    };
}
#endif //KCC_COMPILE_H
