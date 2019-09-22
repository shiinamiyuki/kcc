#include "codegen.h"
#include "magic_enum.hpp"

namespace kcc {
    Register systemVArgRegisters[] = {rdi, rsi, rdx, rcx, r8, r9};
    size_t systemVArgRegisterN = sizeof(systemVArgRegisters) / sizeof(Register);
    std::set<Register> scratchRegisters = {rax, rdi, rsi, rdx, rcx, r8, r9, r10, r11};
    std::set<Register> functionPreservedRegisters = {rbx, rsp, rbp, r12, r13, r14, r15};

    Register tosca[] = {rbx, rcx, rdx, r10, r11, r12};
    Reg toscaTemp{true, rax};
    static constexpr size_t toscaN = sizeof(tosca) / sizeof(Register);


    void CodeGenerator::visit(AST::For *aFor) {

    }

    void CodeGenerator::visit(AST::Identifier *identifier) {
        int addr = identifier->addr;
        pushIntValue(addr);
    }


    void CodeGenerator::visit(AST::Block *block) {
        for (auto i:*block) {
            spStack.push_back(getSp());
            stack.clear();
            i->accept(this);
            setSp(spStack.back());
            spStack.pop_back();
        }
    }

    void CodeGenerator::visit(AST::TopLevel *top) {
        for (auto i:*top) {
            i->accept(this);
        }
    }

    void CodeGenerator::visit(AST::While *aWhile) {
        auto cond = aWhile->cond();
        auto body = aWhile->body();
        auto startLabel = fmt::format(".L{}", labelCounter++);
        auto endLabel = fmt::format(".L{}", labelCounter++);
        emit("{}:", startLabel);
        cond->accept(this);
        auto top = pop();
        if (top.isMachineReg()) {
            emit("cmpq $0, {}", genReg(top));
        } else {
            emit("movq {}, {}", genReg(top), genReg(toscaTemp));
            emit("cmpq $0, {}", genReg(top), genReg(toscaTemp));
        }
        emit("je {}", endLabel);
        body->accept(this);
        emit("jmp {}", startLabel);
        emit("{}:", endLabel);
    }

    void CodeGenerator::visit(AST::If *anIf) {
        auto cond = anIf->cond();
        auto ifPart = anIf->ifPart();
        auto notTakenLabel = fmt::format(".L{}", labelCounter++);

        cond->accept(this);
        auto top = pop();
        if (top.isMachineReg()) {
            emit("cmpq $0, {}", genReg(top));
        } else {
            emit("movq {}, {}", genReg(top), genReg(toscaTemp));
            emit("cmpq $0, {}", genReg(top), genReg(toscaTemp));
        }
        emit("je {}", notTakenLabel);
        ifPart->accept(this);
        if (anIf->size() == 3) {
            auto endLabel = fmt::format(".L{}", labelCounter++);
            emit("jmp {}", endLabel);
            emit("{}:", notTakenLabel);
            auto elsePart = anIf->elsePart();
            elsePart->accept(this);
            emit("{}:", endLabel);
        }
    }

    void CodeGenerator::visit(AST::TernaryExpression *expression) {

    }

    void CodeGenerator::visit(AST::Number *number) {
        if (number->type()->isInt()) {
            int i = std::stoi(number->tok());
            pushImmInt(i);
        } else {
            AssertThrow(false);
        }

    }

    void CodeGenerator::visit(AST::Return *aReturn) {
        if (aReturn->size() == 1) {
            auto e = cast<AST::Expression *>(aReturn->first());
            e->accept(this);
            emit("movq {}, %rax", genReg(pop()));
        }
        emit("jmp {}", currentFunction.functionEndLabel);
    }

    void CodeGenerator::visit(AST::Empty *empty) {

    }

    void CodeGenerator::visit(AST::PrimitiveType *type) {

    }

    void CodeGenerator::visit(AST::PointerType *type) {

    }

    void CodeGenerator::visit(AST::ArrayType *type) {

    }

    void CodeGenerator::visit(AST::ArgumentExepressionList *list) {

    }

    void CodeGenerator::visit(AST::FuncDefArg *arg) {

    }

    void CodeGenerator::visit(AST::FuncDef *func) {
        currentFunction = FunctionCode();
        currentFunction.name = func->name();
        currentFunction.reservedBytes = 8;
        currentFunction.localVarSize = func->localAllocatedSize;
        currentFunction.functionEndLabel = fmt::format("L_{}", currentFunction.name);
        setSp(currentFunction.localVarSize + currentFunction.reservedBytes);
        // save arguments to stack
        auto arg = func->arg();
        for (int i = 0; i < arg->size(); i++) {
            auto decl = cast<AST::Declaration *>(arg->get(i));
            auto identifier = decl->identifier();
            if (i < systemVArgRegisterN) {
                emit("movq %{},{} ", toString(systemVArgRegisters[i]), genAddr(identifier->addr));
            } else {
                emit("movq {}(%rbp), %rax", (i - systemVArgRegisterN) * 8 + 16);
                emit("movq %rax, {}", genAddr(identifier->addr));
            }
        }


        func->block()->accept(this);
        currentFunction.bytesAllocated = align16(currentFunction.bytesAllocated);
        debug("allocted bytes: {}\n", currentFunction.bytesAllocated);
        compiledFunctions.push_back(currentFunction);
    }

    void CodeGenerator::visit(AST::CallExpression *expression) {
        auto callee = expression->callee();
        auto args = expression->arg();

        std::set<Register> usedScratchRegisters;
        for (const auto &r: stack) {
            if (r.isMachineReg()) {
                usedScratchRegisters.insert((Register) r.index);
            }
        }
        std::vector<std::pair<Register, int>> savedRegisters;

        stack.clear();
        size_t _sp = getSp();


        // save scratch registers onto stack
        if (!usedScratchRegisters.empty()) {
            for (auto reg: usedScratchRegisters) {
                setSp(getSp() + 8);
                emit("movq %{}, -{}(%rbp)", toString(reg), getSp());
                savedRegisters.emplace_back(reg, getSp());
            }
        }

        // eval args
        size_t pushCount = 0;
        for (int64_t i = args->size() - 1; i >= 0; i--) {
            auto e = args->get(i);
            e->accept(this);
            auto reg = pop();
            if (i < systemVArgRegisterN) {
                // store in registers
                if (reg.isMachineReg()) {
                    emit("movq {}, %{}", genReg(reg), toString(systemVArgRegisters[i]));
                } else {
                    emit("movq {}, {}", genReg(reg), genReg(toscaTemp));
                    emit("movq {}, %{}", genReg(toscaTemp), toString(systemVArgRegisters[i]));
                }
            } else {
                // pushed to stack
                if (reg.isMachineReg()) {
                    emit("pushq {}", genReg(reg));
                } else {
                    emit("movq {}, {}", genReg(reg), genReg(toscaTemp));
                    emit("pushq {}", genReg(toscaTemp));
                }
                pushCount++;
            }
        }

        // eval callee
        if (typeid(*callee) == typeid(AST::Identifier)) {
            auto name = cast<AST::Identifier *>(callee)->tok();
            emit("call {}", name);
        } else {
            AssertThrow(false);
        }
        pushRet();


        // rebalance stack
        if (pushCount != 0) {
            emit("addq ${}, %rsp", pushCount * 8);
        }

        // restore saved regs
        if (!usedScratchRegisters.empty()) {
            for (auto iter = savedRegisters.rbegin(); iter != savedRegisters.rend(); iter++) {
                auto reg = iter->first;
                auto addr = iter->second;
                emit("movq -{}(%rbp), {}", addr, toString(reg));
            }
        }
        setSp(_sp);
    }

    void CodeGenerator::visit(AST::CastExpression *expression) {

    }

    void CodeGenerator::visit(AST::IndexExpression *expression) {

    }

    void CodeGenerator::visit(AST::Declaration *declaration) {

        auto addr = declaration->identifier()->addr;

        setSp(std::max(getSp(), addr));
        if (declaration->size() == 3) {
            auto expr =
                    cast<AST::Expression *>(declaration->third());
            expr->accept(this);
            auto reg = pop();
            store(reg, addr);
        }
    }

    void CodeGenerator::visit(AST::DeclarationList *list) {
        for (auto i:*list) {
            i->accept(this);
        }
    }

    void CodeGenerator::visit(AST::Literal *literal) {
        const auto &s = literal->tok();
        auto iter = literals.find(s);
        if (iter == literals.end()) {
            literals[s] = fmt::format(".LC{}", literals.size());
        }
        auto literalName = literals[s];
        pushLiteral(literalName);
    }

    bool isAssignOp(const std::string &op) {
        if (op.back() != '=') {
            return false;
        }
        if (op == "==" || op == ">=" || op == "<=" || op == "!=") {
            return false;
        }
        return true;
    }


    void CodeGenerator::visit(AST::BinaryExpression *expression) {
        auto right = cast<AST::Expression *>(expression->rhs());
        auto left = cast<AST::Expression *>(expression->lhs());
        if (!isAssignOp(expression->tok())) {
            right->accept(this);
            left->accept(this);
            iop(expression->tok());
        } else {
            if (typeid(*left) == typeid(AST::Identifier)) {
                auto identifier = cast<AST::Identifier *>(left);
                auto addr = identifier->addr;
                right->accept(this);
                store(pop(), addr);
            } else {
                AssertThrow(false);
            }
        }
    }

    void CodeGenerator::visit(AST::UnaryExpression *expression) {

    }

    void CodeGenerator::visit(AST::Enum *anEnum) {

    }

    void CodeGenerator::visit(AST::FuncType *type) {

    }

    void CodeGenerator::visit(AST::PostfixExpr *expr) {

    }

    void CodeGenerator::visit(AST::FuncArgType *type) {

    }


    Reg CodeGenerator::pop() {
        auto r = stack.back();
        stack.pop_back();
        return r;
    }

    static std::set<std::string> intArithmeticOps = {
            "+", "-", "*", "/", "%", "<<", ">>", "&", "|", "^"
    };
    static std::set<std::string> floatArithmeticOps = {
            "+", "-", "*", "/",
    };
    static std::set<std::string> logicArithmeticOps = {
            "&&", "||",
    };
    static std::set<std::string> comparisonOps = {
            "==", "!=", ">=", "<=", "<", ">"
    };
    static std::set<std::string> assignOps = {
            "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "|=", "^="
    };

    void CodeGenerator::iop(const std::string &op) {
        auto top = pop();
        auto &newTop = stack.back();
        std::string opcode;
        if (comparisonOps.find(op) != comparisonOps.end()) {
            if (op == ">") {
                opcode = "setg";
            } else if (op == ">=") {
                opcode = "setge";
            } else if (op == "<") {
                opcode = "setl";
            } else if (op == "<=") {
                opcode = "setle";
            } else if (op == "==") {
                opcode = "sete";
            } else if (op == "!=") {
                opcode = "setne";
            }
            emit("movq {}, %rax", genReg(newTop));
            emit("cmpq %rax, {}", genReg(top));
            emit("{} %al", opcode);
            emit("movzbl %al, {}", genReg(newTop));
            return;
        }

        if (op == "+") {
            opcode = "addq";
        } else if (op == "-") {
            opcode = "subq";
        } else {
            AssertThrow(false);
        }

        // newTop = newTop op top
        if (newTop.isMachineReg() && top.isMachineReg()) {
            emit("{} {}, {}", opcode, genReg(top), genReg(newTop));
        } else if (newTop.isMachineReg() && !top.isMachineReg()) {
            emit("{} {}, {}", opcode, genReg(top), genReg(newTop));
        } else if (!newTop.isMachineReg() && !top.isMachineReg()) {
            emit("movq {}, %rax", genReg(newTop));
            emit("{} {}, %rax", genReg(top));
            emit("movq %rax, {}", genReg(newTop));
        } else {
            AssertThrow(false);
        }
    }

    void CodeGenerator::fop(char op) {

    }

    std::string CodeGenerator::genReg(const Reg &r) {
        if (r.isInt) {
            if (r.isMachineReg()) {
                auto regName = toString((Register) r.index);
                return fmt::format("%{}", regName);
            } else {
                return fmt::format("-{}%(rbp)", r.index);
            }
        } else {
            AssertThrow(false);
        }
    }

    Reg CodeGenerator::stackTopElement() {
        if (stack.size() < toscaN) {
            return Reg{true, tosca[stack.size()]};
        } else {
            return Reg{true, int(toscaN + stack.size() + getSp()) * 8};
        }
    }

    void CodeGenerator::_push(const Reg &reg) {
        if (functionPreservedRegisters.find((Register) reg.index) != functionPreservedRegisters.end()) {
            currentFunction.usedPreservedRegisters.insert((Register) reg.index);
        }
        stack.push_back(reg);
    }

    void CodeGenerator::push(const std::string &opcode, const std::string &value) {
        Reg reg = stackTopElement();
        if (reg.isMachineReg()) {
            emit("{} {}, {}", opcode, value, genReg(reg));
        } else {
            emit("{} {}, {}", opcode, value, genReg(toscaTemp));
            emit("movq {}, {}", genReg(toscaTemp), genReg(reg));
        }
        _push(reg);
    }

    void CodeGenerator::pushImmInt(int value) {
        push("movq", fmt::format("${}", value));
    }

    void CodeGenerator::pushIntValue(int addr) {
//        Reg reg = stackTopElement();
//        if (reg.isMachineReg()) {
//            emit("movq {}, {}", genAddr(addr), genReg(reg));
//        } else {
//            emit("movq {}, {}", genAddr(addr), genReg(toscaTemp));
//            emit("movq {}, {}", genReg(toscaTemp), genReg(reg));
//        }
//        _push(reg);
        push("movq", genAddr(addr));
    }

    void CodeGenerator::pushRegister(Register) {

    }

    void CodeGenerator::pushLiteral(const std::string &name) {
//        Reg reg = stackTopElement();
//        if (reg.isMachineReg()) {
//            emit("leaq {}(%rip), {}", name, genReg(reg));
//        } else {
//            emit("leaq {}(%rip), {}", name, genReg(toscaTemp));
//            emit("movq {}, {}", genReg(toscaTemp), genReg(reg));
//        }
//        _push(reg);
        push("leaq", fmt::format("{}(%rip)", name));
    }

    void CodeGenerator::pushAddr(int addr) {
        Reg reg = stackTopElement();
        if (reg.isMachineReg()) {
            emit("lea {}, {}", genAddr(addr), genReg(reg));
        } else {
            emit("movq ${}, {}", genAddr(addr), genReg(toscaTemp));
            emit("movq {}, {}", genReg(toscaTemp), genReg(reg));
        }
        _push(reg);
    }

    void CodeGenerator::pushRet() {
//        Reg reg = stackTopElement();
//        if (reg.isMachineReg()) {
//            emit("movq %{}, {}", toString(rax), genReg(reg));
//        } else {
//            emit("movq %{}, {}", toString(rax), genReg(toscaTemp));
//            emit("movq {}, {}", genReg(toscaTemp), genReg(reg));
//        }
//        _push(reg);
        push("movq", "%rax");
    }

    std::string originalString(const std::string &s) {
        std::string out;
        for (auto c:s) {
            if (c == '\n') {
                out += "\n";
            } else {
                out += c;
            }

        }
        return out;
    }

    std::string CodeGenerator::generateAsm() {
        std::ostringstream out;
        out << ".section\t.rodata\n";
        for (auto iter:literals) {
            out << fmt::format("{}:\n    .string \"{}\"\n", iter.second, originalString(iter.first));
        }
        for (auto &func :compiledFunctions) {
            out << func.generateFunctionCode();
        }

        out << std::ends;
        return out.str();
    }


    std::string FunctionCode::generateFunctionCode() {
        std::ostringstream out;
        out << fmt::format("   .text\n.globl {}\n", name);
        out << name << ":\n";
        out << R"(
    pushq %rbp
    movq  %rsp, %rbp
)";
        out << fmt::format("    subq ${}, %rsp\n", bytesAllocated);
        for (auto &i:code) {
            out << "    " << i << "\n";
        }
        out << functionEndLabel << ":\n";
        out << R"(
    movq %rbp, %rsp
    popq %rbp
    ret
)";
        out << std::ends;
        return out.str();
    }
}

