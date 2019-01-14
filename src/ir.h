//
// Created by xiaoc on 2018/9/17.
//

#ifndef KCC_IR_H
#define KCC_IR_H

#include "kcc.h"
#include "ast.h"

namespace kcc {
    // op A B C
    enum class Opcode {
        nop,
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
        sconst,
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
        func_begin,
        func_end,
        pushi,
        pushf,
        callGlobal,
    };

    struct Version {
        int addr;
        int ver;

        Version(int _addr, int _ver) : addr(_addr), ver(_ver) {}

        Version() : ver(-1) {}
    };

    struct Phi {
        Version result;
        std::vector<Version> param;

        Phi(int addr, size_t nNodes) : result(addr, -1) {
            param.resize(nNodes);
            for (auto &i:param) {
                i.addr = addr;
            }
        }

        std::string dump() const;
    };

    struct BasicBlock;

//    template<typename T>
//    struct UseDef {
//        T *def;
//        std::vector<T *> use;
//
//        UseDef() { def = nullptr; }
//
//        UseDef(T *u, T *d) : def(d) { use.emplace_back(u); }
//    };

    struct IRNode {
        Opcode op;
        Value a;
        Value b;
        Value c;
        int version;
        std::string s;
        BasicBlock *bb;

        //  UseDef<IRNode> useDef;
        IRNode(Opcode _op, Value _a, const std::string &_s) : op(_op), s(_s), a(_a) {}

        IRNode(Opcode _op, const std::string &_s, Value _a) : op(_op), s(_s), a(_a) {}

        IRNode(Opcode _op, const std::string &_s) : op(_op), s(_s) {}

        IRNode(Opcode _op) : op(_op) {}

        IRNode(Opcode _op, Value _a) : op(_op), a(_a), bb(nullptr) {}

        IRNode(Opcode _op, Value _a, Value _b, Value _c) : op(_op), a(_a), b(_b), c(_c), bb(nullptr) {}

        IRNode(Opcode _op, Value _a, Value _b) : op(_op), a(_a), b(_b), bb(nullptr) {}

        std::vector<int> in;
        std::vector<int> out;

        std::string dump() const;

        void erase() { op = Opcode::nop; }
    };

    struct CFG;

    struct Function {
        std::vector<IRNode> ir;
        unsigned int alloc;
        CFG *cfg;

        CFG *generateCFG();

        std::string name;

        void assignEdgeToBB();

        void findEdges();

        void naive(CFG *);

        Function() {}

        Function(const std::string &_name, int _a) : name(_name), alloc(_a) {}
    };

}
#endif //KCC_IR_H
