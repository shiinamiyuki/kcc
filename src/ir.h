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
        il,
        ile,
        ig,
        ige,
        ie,
        ine,
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
        fl,
        fle,
        fg,
        fge,
        fe,
        fne,
        move,
        empty,
        break_placeholder,
        continue_placeholder,
    };
    struct Version{
        int addr;
        int ver;
        Version(int _addr,int _ver):addr(_addr),ver(_ver){ }
        Version():ver(-1){}
    };
    struct Phi{
        Version result;
        std::vector<Version> param;
    };
    struct BasicBlock;
    struct IRNode{
        Opcode op;
        int a;
        int b;
        int c;
        double fval;
        int version;
        BasicBlock * bb;
        IRNode(Opcode _op,int _a):op(_op),a(_a),bb(nullptr){}
        IRNode(Opcode _op,int _a,int _b,int _c):op(_op),a(_a),b(_b),c(_c),bb(nullptr){}
        IRNode(Opcode _op,int _a, int imm):op(_op),a(_a),b(imm),bb(nullptr){}
        IRNode(Opcode _op,int _a,double f):op(_op),a(_a),fval(f),bb(nullptr){}
        std::vector<int> in;
        std::vector<int> out;
        std::string dump()const;
    };
}
#endif //KCC_IR_H
