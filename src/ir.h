//
// Created by xiaoc on 2018/9/17.
//

#ifndef KCC_IR_H
#define KCC_IR_H

#include "kcc.h"
namespace kcc {
    // op A B C
    enum class Opcode {
        loadGlobal,
        storeGlobal,
        store,
        load,
        iadd,
        isub,
        imul,
        idiv,
        cvti2f,
        cvtf2i,
        ret,
        call,
        jmp,
        branch,
        iconst,
        fconst,
        fadd,
        fsub,
        fmul,
        fdiv,

    };
    struct Phi{

    };
    struct IRNode{
        Opcode op;
        int a;
        int b;
        int c;
        double fval;
        IRNode(Opcode _op,int _a,int _b,int _c):op(_op),a(_a),b(_b),c(_c){}
        IRNode(Opcode _op,int _a, int imm):op(_op),a(_a){}
        IRNode(Opcode _op,int _a,double f):op(_op),a(_a),fval(f){}
        std::string dump()const;
    };
}
#endif //KCC_IR_H
