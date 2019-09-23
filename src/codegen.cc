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
        auto size = cond->type()->size();
        auto top = pop();
        auto suffix = getSuffix(size);
        if (top.isMachineReg()) {
            emit("cmp{} $0, {}", suffix, genReg(top, size));
        } else {
            emit("mov{} {}, {}", suffix, genReg(top, size), genReg(toscaTemp, size));
            emit("cmp{} $0, {}", suffix, genReg(top, size), genReg(toscaTemp, size));
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
        auto size = cond->type()->size();
        auto top = pop();
        auto suffix = getSuffix(size);
        if (top.isMachineReg()) {
            emit("cmp{} $0, {}", suffix, genReg(top, size));
        } else {
            emit("mov{} {}, {}", suffix, genReg(top, size), genReg(toscaTemp, size));
            emit("cmp{} $0, {}", suffix, genReg(top, size), genReg(toscaTemp, size));
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
        } else {
            emit("{}:", notTakenLabel);
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
            auto type = e->type();
            emit("movq {}, %rax", genReg(pop(), 8));
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
            auto size = identifier->type()->size();
            auto suffix = getSuffix(size);
            if (i < systemVArgRegisterN) {
                emit("mov{} %{},{} ", suffix, toString(systemVArgRegisters[i], size), genAddr(identifier->addr));
            } else {
                auto _rax = Reg{true, rax};
                emit("mov{} {}(%rbp), {}", suffix, (i - systemVArgRegisterN) * 8 + 16, genReg(_rax, size));
                emit("mov{} {}, {}", suffix, genReg(_rax, size), genAddr(identifier->addr));
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
            auto size = cast<AST::Expression *>(e)->type()->size();
            auto suffix = getSuffix(size);
            auto reg = pop();
            if (i < systemVArgRegisterN) {
                // store in registers
                if (reg.isMachineReg()) {
                    emit("mov{} {}, %{}", suffix, genReg(reg, size), toString(systemVArgRegisters[i], size));
                } else {
                    emit("mov{} {}, {}", suffix, genReg(reg, size), genReg(toscaTemp, size));
                    emit("mov{} {}, %{}", suffix, genReg(toscaTemp, size), toString(systemVArgRegisters[i], size));
                }
            } else {
                // pushed to stack
                if (reg.isMachineReg()) {
                    emit("pushq {}", genReg(reg, 8));
                } else {
                    emit("mov{} {}, {}", suffix, genReg(reg, size), genReg(toscaTemp, size));
                    emit("pushq {}", genReg(toscaTemp, 8));
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
            store(reg, addr, expr->type()->size());
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
        auto size = std::max(right->type()->size(), left->type()->size());
        if (!isAssignOp(expression->tok())) {
            right->accept(this);
            if (right->type()->isPointer()) {
                auto stride = cast<Type::PointerType *>(right->type())->baseType()->size();
                auto &r = stack.back();
                emit("movq {}, %rax", genReg(r, 8));
                emit("mulq ${}", stride);
                emit("movq %rax, {}", genReg(r, 8));
            }
            left->accept(this);
            iop(expression->tok(), size);
        } else {
//            if (typeid(*left) == typeid(AST::Identifier)) {
//                auto identifier = cast<AST::Identifier *>(left);
//                auto addr = identifier->addr;
//                right->accept(this);
//                store(pop(), addr, size);
//            } else {
//                AssertThrow(false);
//            }
            LValueEvaluator evaluator(*this);
            right->accept(this);
            left->accept(&evaluator);
            auto rvalue = pop();
            auto lvalue = evaluator.stack.back();
            debug("lvalue :{}\n", lvalue);
            store(rvalue, lvalue, size);
        }
    }

    void CodeGenerator::visit(AST::UnaryExpression *expression) {
        auto op = expression->tok();
        if (op == "&") {
            LValueEvaluator evaluator(*this);
            expression->first()->accept(&evaluator);
            push("lea", evaluator.stack.back(), 8);

        } else if (op == "*") {
            expression->first()->accept(this);
            auto &top = stack.back();
            auto size = cast<AST::Expression *>(expression->first())->type()->size();
            AssertThrow(size == 8);
            auto suffix = getSuffix(size);
            if (top.isMachineReg()) {
                emit("mov{} ({}), {}", suffix, genReg(top, 8), genReg(top, size));
            } else {
                emit("movq ({}), %rax", genReg(top, 8));
                emit("movq (%rax), {}", genReg(top, 8));
            }
        } else {
            expression->first()->accept(this);
            iUnaryOp(op, expression->type()->size());
        }
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

    void CodeGenerator::iop(const std::string &op, size_t size) {
        auto top = pop();
        auto &newTop = stack.back();
        auto _rax = Reg{true, rax};
        std::string suffix = getSuffix(size);
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

            emit("mov{} {}, {}", suffix, genReg(newTop, size), genReg(_rax, size));
            emit("cmp{} {}, {}", suffix, genReg(_rax, size), genReg(top, size));
            emit("{} %al", opcode);
            emit("movzbl %al, {}", genReg(newTop, size));
            return;
        }

        if (op == "+") {
            opcode = "add";
        } else if (op == "-") {
            opcode = "sub";
        } else if (op == "*") {
            opcode = "imul";
        } else {
            AssertThrow(false);
        }

        // newTop = newTop op top
        if (newTop.isMachineReg() && top.isMachineReg()) {
            emit("{}{} {}, {}", opcode, suffix, genReg(top, size), genReg(newTop, size));
        } else if (newTop.isMachineReg() && !top.isMachineReg()) {
            emit("{}{} {}, {}", opcode, suffix, genReg(top, size), genReg(newTop, size));
        } else if (!newTop.isMachineReg() && !top.isMachineReg()) {
            emit("mov{} {}, {}", suffix, genReg(newTop, size), genReg(_rax, size));
            emit("{}{} {}, {}", opcode, suffix, genReg(top, size), genReg(_rax, size));
            emit("mov{} {}, {}", suffix, genReg(_rax, size), genReg(newTop, size));
        } else {
            KCC_NOT_IMPLEMENTED();
        }
    }

    void CodeGenerator::fop(char op) {

    }

    std::string CodeGenerator::genReg(const Reg &r, size_t size) {
        if (r.isInt) {
            if (r.isMachineReg()) {
                std::string regName;
                if (size == 8)
                    regName = toString((Register) r.index);
                else if (size == 4) {
                    regName = toString((Register32) r.index);
                } else {
                    KCC_NOT_IMPLEMENTED();
                }
                return fmt::format("%{}", regName);
            } else {
                return fmt::format("-{}%(rbp)", r.index);
            }
        } else {
            KCC_NOT_IMPLEMENTED();
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

    void CodeGenerator::push(const std::string &opcode, const std::string &value, size_t size) {
        Reg reg = stackTopElement();
        auto suffix = getSuffix(size);
        if (reg.isMachineReg()) {
            emit("{}{} {}, {}", opcode, suffix, value, genReg(reg, size));
        } else {
            emit("{}{} {}, {}", opcode, suffix, value, genReg(toscaTemp, size));
            emit("mov{} {}, {}", suffix, genReg(toscaTemp, size), genReg(reg, size));
        }
        _push(reg);
    }

    void CodeGenerator::pushImmInt(int value) {
        push("mov", fmt::format("${}", value), 8);
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
        push("mov", genAddr(addr), 8);
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
        push("lea", fmt::format("{}(%rip)", name), 8);
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
        push("mov", "%rax", 8);
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

    void CodeGenerator::iUnaryOp(const std::string &op, size_t size) {
        if (op == "+") {
            return;
        }
        auto &top = stack.back();
        auto suffix = getSuffix(size);
        std::string opcode;
        auto _rax = Reg{true, rax};
        if (op == "!") {
            emit("cmp{} $0, {}", suffix, genReg(top, size));
            emit("sete %al");
            emit("movzb{} %al, {}", genReg(top, size));
            return;
        }
        if (op == "-") {
            opcode = "neg";
        } else if (op == "~") {
            opcode = "not";
        }
        if (top.isMachineReg()) {
            emit("{}{} {}", opcode, suffix, genReg(top, size));
        } else {

            emit("mov{} {}, {}", suffix, genReg(top, size), genReg(_rax, size));
            emit("{}{} {}", opcode, suffix, genReg(_rax, size));
            emit("mov{} {}, {}", suffix, genReg(_rax, size), genReg(top, size));
        }
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

    void CodeGenerator::LValueEvaluator::visit(AST::Identifier *identifier) {
        stack.push_back(codeGenerator.genAddr(identifier->addr));
    }

    void CodeGenerator::LValueEvaluator::visit(AST::TernaryExpression *expression) {
        KCC_NOT_IMPLEMENTED();
    }

    void CodeGenerator::LValueEvaluator::visit(AST::CallExpression *expression) {
        KCC_NOT_IMPLEMENTED();
    }

    void CodeGenerator::LValueEvaluator::visit(AST::CastExpression *expression) {
        KCC_NOT_IMPLEMENTED();
    }

    void CodeGenerator::LValueEvaluator::visit(AST::IndexExpression *expression) {
        KCC_NOT_IMPLEMENTED();
    }

    void CodeGenerator::LValueEvaluator::visit(AST::BinaryExpression *expression) {
        expression->accept(&codeGenerator);
    }

    void CodeGenerator::LValueEvaluator::visit(AST::UnaryExpression *expression) {
        auto op = expression->tok();
        if (op != "*") {
            throw std::runtime_error("ICE");
        }
        // op = "*"
        expression->first()->accept(&codeGenerator);
        auto r = codeGenerator.pop();
        if (r.isMachineReg()) {
            stack.push_back(fmt::format("({})", codeGenerator.genReg(r, 8)));
        } else {
            codeGenerator.emit("movq {}, %rax", codeGenerator.genReg(r, 8));
            stack.push_back("(%rax)");
        }


    }
}

