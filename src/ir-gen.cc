//
// Created by xiaoc on 2018/9/17.
//

#include "ir-gen.h"

using namespace kcc;

void kcc::IRGenerator::visit(kcc::For *aFor) {

}

void kcc::IRGenerator::visit(kcc::Identifier *identifier) {
    emit(Opcode::load, identifier->getReg(), identifier->getAddr());
}

void kcc::IRGenerator::visit(kcc::While *aWhile) {
    int begin = (int) ir.size();
    aWhile->cond()->accept(this);
    int cond = aWhile->cond()->getReg();
    int branchIdx = (int) ir.size();
    emit(Opcode::branch, cond, 0, 0);
    aWhile->body()->accept(this);
    emit(Opcode::jmp, begin);
    patch(branchIdx, Opcode::branch, cond, branchIdx + 1, ir.size());
}

void kcc::IRGenerator::visit(kcc::Block *block) {
    for(auto i:*block){
        i->accept(this);
    }
}

void kcc::IRGenerator::visit(kcc::TopLevel *level) {
    visitAll(level);
}

void kcc::IRGenerator::visit(kcc::If *anIf) {
    anIf->cond()->accept(this);
    int cond = anIf->cond()->getReg();
    int branchIdx = ir.size();
    emit(Opcode::branch, cond, 0, 0);
    anIf->body()->accept(this);
    int jmpIdx = (int) ir.size();
    emit(Opcode::jmp, 0);
    int a = (int) ir.size();
    if (anIf->size() == 3) {
        anIf->elsePart()->accept(this);
    }
    patch(branchIdx, Opcode::branch, cond, branchIdx + 1, a);
    patch(jmpIdx, Opcode::jmp, ir.size());

}

void kcc::IRGenerator::visit(kcc::TernaryExpression *expression) {

}

void kcc::IRGenerator::visit(kcc::Number *number) {
    if (number->isFloat)
        emit(Opcode::fconst, number->getReg(), std::stod(number->tok()));
    else
        emit(Opcode::iconst, number->getReg(), std::stoi(number->tok()));
}

void kcc::IRGenerator::visit(kcc::Return *aReturn) {

}

void kcc::IRGenerator::visit(kcc::Empty *empty) {

}

void kcc::IRGenerator::visit(kcc::PrimitiveType *type) {

}

void kcc::IRGenerator::visit(kcc::PointerType *type) {

}

void kcc::IRGenerator::visit(kcc::ArrayType *type) {

}

void kcc::IRGenerator::visit(kcc::ArgumentExepressionList *list) {

}

void kcc::IRGenerator::visit(kcc::FuncDefArg *arg) {

}

void kcc::IRGenerator::visit(kcc::FuncDef *def) {
    def->block()->accept(this);
}

void kcc::IRGenerator::visit(kcc::CallExpression *expression) {

}

void kcc::IRGenerator::visit(kcc::CastExpression *expression) {

}

void kcc::IRGenerator::visit(kcc::IndexExpression *expression) {

}

void kcc::IRGenerator::visit(kcc::Declaration *declaration) {

}

void kcc::IRGenerator::visit(kcc::DeclarationList *list) {

}

void kcc::IRGenerator::visit(kcc::Literal *literal) {

}

void kcc::IRGenerator::visit(kcc::BinaryExpression *expression) {
    expression->rhs()->accept(this);

    auto &op = expression->tok();
    if (op == "=") {
        if (expression->lhs()->isFloat && !expression->rhs()->isFloat) {
            emit(Opcode::cvti2f, expression->rhs()->getReg(), expression->rhs()->getReg());
        }
        if (!expression->lhs()->isFloat && expression->rhs()->isFloat) {
            emit(Opcode::cvtf2i, expression->rhs()->getReg(), expression->rhs()->getReg());
        }
        if (expression->lhs()->kind() == Identifier().kind()) {
            emit(Opcode::store, expression->lhs()->getAddr(), expression->rhs()->getReg());
        }
    } else {
        expression->lhs()->accept(this);
        if (expression->lhs()->isFloat || expression->rhs()->isFloat) {
            if (!expression->lhs()->isFloat) {
                emit(Opcode::cvti2f, expression->lhs()->getReg(),
                     expression->lhs()->getReg());
            }
            if (!expression->rhs()->isFloat) {
                emit(Opcode::cvti2f, expression->rhs()->getReg(),
                     expression->rhs()->getReg());
            }
            Opcode opcode;
            if (op == "+") {
                opcode = Opcode::fadd;
            } else if (op == "-") {
                opcode = Opcode::fsub;
            } else if (op == "*") {
                opcode = Opcode::fmul;
            } else if (op == "/") {
                opcode = Opcode::fdiv;
            } else if (op == "<") {
                opcode = Opcode::fl;
            } else if (op == "<=") {
                opcode = Opcode::fle;
            } else if (op == ">") {
                opcode = Opcode::fg;
            } else if (op == ">=") {
                opcode = Opcode::fge;
            } else if (op == "==") {
                opcode = Opcode::fe;
            } else if (op == "!=") {
                opcode = Opcode::fne;
            }
            emit(opcode, expression->getReg(),
                 expression->lhs()->getReg(),
                 expression->rhs()->getReg());
        } else {
            Opcode opcode;
            if (op == "+") {
                opcode = Opcode::iadd;
            } else if (op == "-") {
                opcode = Opcode::isub;
            } else if (op == "*") {
                opcode = Opcode::imul;
            } else if (op == "/") {
                opcode = Opcode::idiv;
            } else if (op == "<") {
                opcode = Opcode::il;
            } else if (op == "<=") {
                opcode = Opcode::ile;
            } else if (op == ">") {
                opcode = Opcode::ig;
            } else if (op == ">=") {
                opcode = Opcode::ige;
            } else if (op == "==") {
                opcode = Opcode::ie;
            } else if (op == "!=") {
                opcode = Opcode::ine;
            }
            emit(opcode, expression->getReg(),
                 expression->lhs()->getReg(),
                 expression->rhs()->getReg());
        }
    }
}

void kcc::IRGenerator::visit(kcc::UnaryExpression *expression) {

}

void kcc::IRGenerator::pre(kcc::AST *ast) {

}

void kcc::IRGenerator::visit(kcc::Enum *anEnum) {

}

void kcc::IRGenerator::visit(kcc::FuncType *type) {

}

void kcc::IRGenerator::visit(kcc::PostfixExpr *expr) {

}

void kcc::IRGenerator::visit(kcc::FuncArgType *type) {

}

void IRGenerator::findEdges() {
    emit(Opcode::empty, 0);//to prevent segfaults :D
    for (int i = 0; i < ir.size(); i++) {
        if (ir[i].op == Opcode::jmp) {
            ir[ir[i].a].in.emplace_back(i);
            ir[i].out.emplace_back(ir[i].a);
        } else if (ir[i].op == Opcode::branch) {
            ir[ir[i].b].in.emplace_back(i);
            ir[ir[i].c].in.emplace_back(i);
            ir[i].out.emplace_back(ir[i].b);
            ir[i].out.emplace_back(ir[i].c);
        }
    }
}

CFG *IRGenerator::generateCFG() {
    findEdges();
    auto cfg = new CFG();
    //trace(cfg, 0);
    naive(cfg);
    assignEdgeToBB();
    return cfg;
}


void IRGenerator::naive(CFG* cfg) {
    /*I don't understand.
     * */
    for(int idx = 0;idx<ir.size();){
        auto bb = new BasicBlock();
        if(!ir[idx].out.empty()){
            ir[idx].bb = bb;
            bb->block.push_back(ir[idx++]);
        }else {
            if (!ir[idx].in.empty() && ir[idx].out.empty()) {
                ir[idx].bb = bb;
                bb->block.push_back(ir[idx++]);
            }
            while (idx < ir.size() && ir[idx].out.empty() && ir[idx].in.empty()) {
                ir[idx].bb = bb;
                bb->block.push_back(ir[idx++]);
            }
            if(ir[idx-1].out.empty() && idx<ir.size())
                ir[idx-1].out.emplace_back(idx);

        }
        cfg->addBasicBlock(bb);
        while (idx < ir.size() && ir[idx].bb)
            idx++;
    }
}


void IRGenerator::trace(CFG *cfg, int idx) {
    if (idx >= ir.size())
        return;
    auto bb = new BasicBlock();
    cfg->addBasicBlock(bb);
    if(!ir[idx].in.empty()){
        ir[idx].bb = bb; //marked as covered
        bb->block.push_back(ir[idx++]);
    }
    while (ir[idx].out.empty() && ir[idx].in.empty() && idx < ir.size()) {
        ir[idx].bb = bb; //marked as covered
        bb->block.push_back(ir[idx++]);
    }
    if(idx>=ir.size()){return;}
    if(!ir[idx].out.empty()) {
        ir[idx].bb = bb;
        bb->block.push_back(ir[idx]);
        for (auto i:ir[idx].out) {
            if (!ir[i].bb)// not yet covered
                trace(cfg, i);
        }
    }else if(!ir[idx].in.empty()){
        trace(cfg,idx);

    }



}

void IRGenerator::assignEdgeToBB() {
    for(auto i : ir){
        if(!i.out.empty()){
            auto bb = i.bb;
            if(!bb)continue;
            auto jt = ir[i.out[0]].bb;
            assert(bb != jt);
            bb->branchTrue = Edge(bb,jt);
            jt->in.emplace_back(Edge(bb,jt));
            if(i.out.size() == 2){
                auto jf = ir[i.out[1]].bb;
                assert(bb != jf);
                bb->branchFalse = Edge(bb,jf);
                jf->in.emplace_back(Edge(bb,jf));
            }
        }
    }
}


