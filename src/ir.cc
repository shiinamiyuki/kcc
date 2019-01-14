//
// Created by xiaoc on 2018/9/17.
//
#include "cfg.h"
#include "ir.h"
#include "format.h"

using namespace kcc;

std::string kcc::IRNode::dump() const {
    switch (op) {
        case Opcode::func_begin:
            return format("FUNC: {},{}", s, a);
        case Opcode::func_end:
            return std::string("END");
        case Opcode::iconst:
            return format("t{} = ${}", a, b);
        case Opcode::fconst:
            return format("t{} = ${}", a, b.fImm);
        case Opcode::sconst:
            return format("t{} = \"{}\"", a, s);
        case Opcode::cvtf2i:
            return format("t{} = (int)t{}", a, b);
        case Opcode::cvti2f:
            return format("t{} = (double)t{}", a, b);
        case Opcode::iadd:
            return format("t{} = t{} + t{}", a, b, c);
        case Opcode::isub:
            return format("t{} = t{} - t{}", a, b, c);
        case Opcode::imul:
            return format("t{} = t{} * t{}", a, b, c);
        case Opcode::idiv:
            return format("t{} = t{} / t{}", a, b, c);
        case Opcode::il:
            return format("t{} = t{} < t{}", a, b, c);
        case Opcode::ile:
            return format("t{} = t{} <= t{}", a, b, c);
        case Opcode::ig:
            return format("t{} = t{} > t{}", a, b, c);
        case Opcode::ige:
            return format("t{} = t{} >= t{}", a, b, c);
        case Opcode::ie:
            return format("t{} = t{} == t{}", a, b, c);
        case Opcode::ine:
            return format("t{} = t{} != t{}", a, b, c);
        case Opcode::fadd:
            return format("t{} = t{} +. t{}", a, b, c);
        case Opcode::fsub:
            return format("t{} = t{} -. t{}", a, b, c);
        case Opcode::fmul:
            return format("t{} = t{} *. t{}", a, b, c);
        case Opcode::fdiv:
            return format("t{} = t{} /. t{}", a, b, c);
        case Opcode::fl:
            return format("t{} = t{} <. t{}", a, b, c);
        case Opcode::fle:
            return format("t{} = t{} <=. t{}", a, b, c);
        case Opcode::fg:
            return format("t{} = t{} >. t{}", a, b, c);
        case Opcode::fge:
            return format("t{} = t{} >=. t{}", a, b, c);
        case Opcode::fe:
            return format("t{} = t{} ==. t{}", a, b, c);
        case Opcode::fne:
            return format("t{} = t{} !=. t{}", a, b, c);
        case Opcode::jmp:
            return format("jmp {}", a);
        case Opcode::branch:
            return format("branch t{}, %true. {}, %false. {}", a, b, c);
        case Opcode::load:
            return format("t{} = [{}]_{}", a, b, version);
        case Opcode::store:
            return format("[{}]_{} = t{}", a, version, b);
        case Opcode::empty:
            return std::string("end");
        case Opcode::ret:
            return format("ret t{}", a);
        case Opcode::pushi:
            return format("pushi t{}", a);
        case Opcode::pushf:
            return format("pushf t{}", a);
        case Opcode::callGlobal:
            return format("call global {}", s);
        default:
            return format("unknown opcode {}", (int) op);
    }
}

void Function::findEdges() {
    ir.emplace_back(IRNode(Opcode::empty));//to prevent segfaults :D
    for (int i = 0; i < ir.size(); i++) {
        if (ir[i].op == Opcode::jmp) {
            ir[ir[i].a.getImm()].in.emplace_back(i);
            ir[i].out.emplace_back(ir[i].a.getImm());
        } else if (ir[i].op == Opcode::branch) {
            ir[ir[i].b.getImm()].in.emplace_back(i);
            ir[ir[i].c.getImm()].in.emplace_back(i);
            ir[i].out.emplace_back(ir[i].b.getImm());
            ir[i].out.emplace_back(ir[i].c.getImm());
        } else if (ir[i].op == Opcode::ret) {
            ir[i].out.emplace_back(ir.size() - 1);
        }
    }
}


CFG *Function::generateCFG() {
    findEdges();
    cfg = new CFG();
    naive(cfg);
    assignEdgeToBB();
    return cfg;
}


void Function::naive(CFG *cfg) {
    /*I don't understand.
     * */
    for (int idx = 0; idx < ir.size();) {
        auto bb = new BasicBlock();
        if (!ir[idx].out.empty()) {
            ir[idx].bb = bb;
            bb->block.push_back(ir[idx++]);
        } else {
            if (!ir[idx].in.empty() && ir[idx].out.empty()) {
                ir[idx].bb = bb;
                bb->block.push_back(ir[idx++]);
            }
            while (idx < ir.size() && ir[idx].out.empty() && ir[idx].in.empty()) {
                ir[idx].bb = bb;
                bb->block.push_back(ir[idx++]);
            }
            if (ir[idx - 1].out.empty() && idx < ir.size())
                ir[idx - 1].out.emplace_back(idx);

        }
        cfg->addBasicBlock(bb);
        while (idx < ir.size() && ir[idx].bb)
            idx++;
    }
}


void Function::assignEdgeToBB() {
    for (auto i : ir) {
        if (!i.out.empty()) {
            auto bb = i.bb;
            if (!bb)continue;
            auto jt = ir[i.out[0]].bb;
            assert(bb != jt);
            bb->branchTrue = Edge(bb, jt);
            jt->in.emplace_back(Edge(bb, jt));
            if (i.out.size() == 2) {
                auto jf = ir[i.out[1]].bb;
                assert(bb != jf);
                bb->branchFalse = Edge(bb, jf);
                jf->in.emplace_back(Edge(bb, jf));
            }
        }
    }
}

std::string Phi::dump() const {
    std::string s = format("[{}]_{} = phi(", result.addr, result.ver);
    for (auto i:param) {
        s.append(format("[{}]_{},", i.addr, i.ver));
    }
    s.append(")");
    return s;
}
