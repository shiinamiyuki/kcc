//
// Created by xiaoc on 2018/10/22.
//

#include "x64-gen.h"

std::vector<const char *> iReg = {"%rax", "%rbx", "%rcx", "%rdx", "%r8", "%r9"}
, fReg = {};

//using n-TOSCA
void kcc::DirectCodeGen::generateFunc(Function &function) {
    clearReg();
    iRegLevel = fRegLevel = 0;
    bytesForLocals = function.alloc;
    emit(".globl\t{}\n"
         ".def\t{}\n"
         ".seh_proc\t{}", function.name, function.name, function.name);
    emit("{}:", function.name);

    emit(" pushq   %rbp\n"
         " movq    %rsp, %rbp");
    emit("subq $64, %rsp");
    auto allocIdx = out.size();
    emit("");
    int labelCount = 0;
    int a, b, c;
    for (int i = 0; i < function.ir.size(); i++) {
        auto &node = function.ir[i];
        switch (node.op) {
            case Opcode::nop:
                break;
            case Opcode::sconst:
                a = iAlloc(node.a);
                if (directMove(a)) {
                    emit("leaq SC{}(%rip),{}", addString(node.s), getIReg(a));
                }
                break;
            case Opcode::iconst:
                a = iAlloc(node.a);
                emit("movq ${}, {}", node.b, getIReg(a));
                break;
            case Opcode::load:
                a = iAlloc(node.a);
                if(!directMove(a)){
                    int spilled;
                    int r = findiRegSpillIfNone(spilled);
                    emit("movq -{}(%rbp),{}", node.b, getIReg(r));
                    move(r, a);
                    if(spilled>=0){
                        move(spilled,r);
                        killReg(spilled);
                    }
                    killReg(r);
                }else {
                    emit("movq -{}(%rbp),{}", node.b, getIReg(a));
                }
                break;
            case Opcode::store:
                b = getReg(node.b);
                if(!directMove(b)){
                    int spilled;
                    int r = findiRegSpillIfNone(spilled);
                    move(b,r);
                    emit("movq {},-{}(%rbp)",getIReg(r),node.a);
                    if(spilled>=0){
                        move(spilled ,r);
                        killReg(spilled);
                    }
                    killReg(r);
                }else{
                    emit("movq {},-{}(%rbp)",getIReg(b),node.a);
                }
                break;
            case Opcode::iadd:
                b = getReg(node.b);
                c = getReg(node.c);
                a = iAlloc(node.a);
                terOp("addq", a, b, c);
                break;
            case Opcode::isub:
                b = getReg(node.b);
                c = getReg(node.c);
                a = iAlloc(node.a);
                terOp("subq", a, c,b);
                break;
            case Opcode::imul:
                b = getReg(node.b);
                c = getReg(node.c);
                a = iAlloc(node.a);
                terOp("imul", a, b, c);
                break;
            case Opcode::pushi:
                a = getReg(node.a);
                move( a, win_x64CalliReg(node.b));
                break;
            case Opcode::loadGlobal:
                a = iAlloc(node.a);
                emit("movq  {}(%rip),{}", node.s, getIReg(a));
                break;
            case Opcode::callGlobal:
                emit("call {}", node.s);
                break;
            case Opcode::ret:
                a = getReg(node.a);
                move(a,0);
                emit("jmp .L{}Ret",function.name);
                break;

        }
    }
    emit(".L{}Ret:",function.name);
    emit("subq $64, %rsp");
    emit(" popq    %rbp\n"
         " retq");
    emit(".seh_endproc");
}


int kcc::DirectCodeGen::getReg(int i) {
    if (regMap.find(i) == regMap.end()) {
        throw std::runtime_error("internal error");
    }
    killReg(regMap[i]);
    return (regMap[i]);
}

std::string kcc::DirectCodeGen::fAlloc(int i) {

}

int kcc::DirectCodeGen::iAlloc(int x) {
    //  println("alloc for {}",x);

    regMap[x] = findSpare();
    //      println("alloc {}",i);
    return (regMap[x]);
}

int kcc::DirectCodeGen::findSpare() {
    for (int i = 0; i < 99999; i++) {
        if (!regUsage[i]) {
            regUsage[i] = true;
            return i;
        }
    }
    return -1;
}


std::string kcc::DirectCodeGen::getIReg(int i) {
    if (i < iReg.size()) {
        return iReg[i];
    } else {
        return format("-{}(%rbp)", 8*(i - iReg.size()) + bytesForLocals);
    }
}

std::string kcc::DirectCodeGen::getFReg(int i) {
    if (i < fReg.size()) {
        return fReg[i];
    } else {
        return format("-{}(%rbp)", 8*(i - fReg.size() + bytesForLocals));
    }
}

kcc::DirectCodeGen::DirectCodeGen() {

}

bool kcc::DirectCodeGen::directMove(int a, int b) {
    return directMove(a) || directMove(b);
}

bool kcc::DirectCodeGen::directMove(int a) {
    return a < iReg.size();
}

//b = op(a,b)
void kcc::DirectCodeGen::binOp(const std::string &op, int a, int b) {
    if (directMove(a, b)) {
        emit("{} {}, {}", op, getIReg(a), getIReg(b));
    } else {
        int spill = -1;
        if (!directMove(a)) {
            bool markA = regUsage[a],markB = regUsage[b];
            if(!markA){
                markReg(a);
            }
            if(!markB){
                markReg(b);
            }
            int r = findiRegSpillIfNone(spill);
            move(a, r);
            binOp(op, r, b); // don't need to spill now
            if(!markA){
                killReg(a);
            }
            if(!markB){
                killReg(b);
            }
            if (spill >= 0) {
                move(spill, r);
                killReg(spill);
            }
            killReg(r);
        }else{
            //should not happen
            assert(0);
        }
    }

}

void kcc::DirectCodeGen::terOp(const std::string &op, int a, int b, int c) {
    binOp(op, b,c);
    move(c, a);
}


void kcc::DirectCodeGen::move(int a, int b) {
    if (a == b)return;
    binOp("movq",a,b);
}

int kcc::DirectCodeGen::findiRegSpillIfNone(int &spilled) {
    for (int i = 0; i < iReg.size(); i++) {
        if (!regUsage[i]) {
            spilled = -1;
            regUsage[i] = true;
            return i;
        }
    }
    int r = findSpare();
    spilled = r;
    regUsage[0] = true;
    move(0, r); //default using rax

    return 0;
}

int kcc::DirectCodeGen::win_x64CalliReg(int i) {
    if(i <= 4){
        return i + 2;
    }
    assert(0);
    return 0;
}

