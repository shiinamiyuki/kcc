#include "codegen.h"

namespace kcc {
    Register systemVArgRegisters[] = {rdi, rsi, rdx, rcx, r8, r9};
    size_t systemVArgRegisterN = sizeof(systemVArgRegisters) / sizeof(Register);
    std::set<Register> scratchRegisters = {rax, rdi, rsi, rdx, rcx, r8, r9, r10, r11};
    std::set<Register> functionPreservedRegisters = {rbx, r12, r13, r14, r15};

    XmmRegister xmmTOSCA[] = {xmm8, xmm9, xmm11, xmm12, xmm13, xmm14, xmm15, mmx0};
    Register tosca[] = {rbx, rcx, r10, r11, r12, r13, r14, r15};
    Reg toscaTemp{true, rax};
    Reg toscaTemp2{true, rdx};
    Reg toscaTempF{false, mmx1};
    static constexpr size_t toscaN = sizeof(tosca) / sizeof(Register);


    void CodeGenerator::visit(AST::Identifier *identifier) {
        if (identifier->type()->isStruct() || identifier->type()->isArray()) {
            LValueEvaluator evaluator(*this);
            identifier->accept(&evaluator);
            push("lea", evaluator.stack.back(), 8);
        } else {
            if (identifier->isGlobal) {
                auto name = identifier->tok();
                auto iter = globals.find(name);
                if (iter == globals.end()) {
                    globals[name] = std::make_pair(name, identifier->type()->size());
                    iter = globals.find(name);
                }
                push("mov", fmt::format("{}(%rip)", iter->second.first), iter->second.second);
            } else {
                int addr = identifier->addr;
                pushIntValue(addr, identifier->type()->size());
            }
        }
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

    void CodeGenerator::visit(AST::For *aFor) {
        auto init = aFor->init();
        auto cond = aFor->cond();
        auto step = aFor->step();
        auto body = aFor->body();
        init->accept(this);

        auto startLabel = fmt::format(".L{}", labelCounter++);
        auto endLabel = fmt::format(".L{}", labelCounter++);
        emit("{}:", startLabel);
        if (typeid(*cond) != typeid(AST::Empty)) {

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
        }
        loopStartLabelStack.push_back(startLabel);
        loopEndLabelStack.push_back(endLabel);
        body->accept(this);
        loopStartLabelStack.pop_back();
        loopEndLabelStack.pop_back();
        step->accept(this);
        emit("jmp {}", startLabel);
        emit("{}:", endLabel);

    }

    void CodeGenerator::visit(AST::DoWhile *aWhile) {
        auto cond = aWhile->cond();
        auto body = aWhile->body();
        auto startLabel = fmt::format(".L{}", labelCounter++);
        auto endLabel = fmt::format(".L{}", labelCounter++);
        emit("{}:", startLabel);
        loopStartLabelStack.push_back(startLabel);
        loopEndLabelStack.push_back(endLabel);
        body->accept(this);
        loopStartLabelStack.pop_back();
        loopEndLabelStack.pop_back();
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
        emit("jmp {}", startLabel);
        emit("{}:", endLabel);
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
        loopStartLabelStack.push_back(startLabel);
        loopEndLabelStack.push_back(endLabel);
        body->accept(this);
        loopStartLabelStack.pop_back();
        loopEndLabelStack.pop_back();
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
        auto cond = expression->cond();
        cond->accept(this);
        AssertThrow(!stack.empty());
        auto label = fmt::format(".L{}", labelCounter++);
        auto r = pop();
        emit("movq {}, %rax", genReg(r, 8));
        emit("jz {}", label);
        auto end = fmt::format(".L{}", labelCounter++);

        expression->part1()->accept(this);
        AssertThrow(!stack.empty());
        pop();
        emit("jmp {}", end);
        emit("{}:", label);
        expression->part2()->accept(this);
        AssertThrow(!stack.empty());
        emit("{}:", end);
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
        debug("allocated bytes: {}\n", currentFunction.bytesAllocated);
        compiledFunctions.push_back(currentFunction);
    }

    void CodeGenerator::visit(AST::CallExpression *expression) {
        auto callee = expression->callee();
        auto args = expression->arg();

        std::set<Reg> usedScratchRegisters;

        for (const auto &r: stack) {
            if (r.isMachineReg()) {
                if (r.isInt && scratchRegisters.find((Register) r.index) != scratchRegisters.end())
                    usedScratchRegisters.insert(r);
                if (!r.isInt)
                    usedScratchRegisters.insert(r);
            }
        }
        std::vector<std::pair<Reg, int>> savedRegisters;

        //std::vector<Reg> _stack = stack;
        //stack.clear();
        size_t _sp = getSp();


        // save scratch registers onto stack
        if (!usedScratchRegisters.empty()) {
            for (auto reg: usedScratchRegisters) {
                setSp(getSp() + 8);
                if (reg.isInt) {
                    emit("movq %{}, -{}(%rbp)", toString((Register) reg.index), getSp());
                } else {
                    emit("movss %{}, -{}(%rbp)", toString((XmmRegister) reg.index), getSp());
                }
                savedRegisters.emplace_back(reg, getSp());
            }
        }

        // eval args
        int64_t pushCount = std::max<int64_t>(0, args->size() - systemVArgRegisterN);
        if (pushCount % 2 == 1) {
            pushCount++;
            emit("subq $8, %rsp");
        }
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
                if (reg.isInt)
                    emit("movq -{}(%rbp), %{}", addr, toString((Register) reg.index));
                else
                    emit("movss -{}(%rbp), %{}", addr, toString((XmmRegister) reg.index));
            }
        }
        setSp(_sp);
        //stack = _stack;
    }

    void CodeGenerator::visit(AST::CastExpression *expression) {
        expression->second()->accept(this);
    }

    void CodeGenerator::visit(AST::IndexExpression *expression) {

    }

    void CodeGenerator::visit(AST::Declaration *declaration) {
        auto identifier = declaration->identifier();
        auto addr = identifier->addr;
        if (identifier->isGlobal) {
            if (!identifier->type()->isFunction())
                globals[identifier->tok()] = std::make_pair(identifier->tok(), identifier->type()->size());
        }
        setSp(std::max(getSp(), addr));
        if (declaration->size() == 3) {
            auto expr =
                    cast<AST::Expression *>(declaration->third());
            expr->accept(this);
            auto reg = pop();
            if (!identifier->isGlobal) {
                store(reg, addr, identifier->type()->size());
            } else {
                KCC_NOT_IMPLEMENTED();
            }
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

    void CodeGenerator::LValueEvaluator::visit(AST::TernaryExpression *expression) {


    }

    void CodeGenerator::visit(AST::BinaryExpression *expression) {
        auto right = cast<AST::Expression *>(expression->rhs());
        auto left = cast<AST::Expression *>(expression->lhs());
        auto size = std::max(right->type()->size(), left->type()->size());
        auto op = expression->tok();
        if (op == "&&") {
            auto endLabel = fmt::format(".L{}", labelCounter++);
            left->accept(this);
            emit("jz {}", endLabel);
            auto reg = pop();
            right->accept(this);
            auto reg2 = stack.back();
            AssertThrow(reg.index == reg2.index);
            emit("{}:", endLabel);
        } else if (op == "||") {
            auto endLabel = fmt::format(".L{}", labelCounter++);
            left->accept(this);
            emit("jnz {}", endLabel);
            auto reg = pop();
            right->accept(this);
            auto reg2 = stack.back();
            AssertThrow(reg.index == reg2.index);
            emit("{}:", endLabel);
        } else if (!isAssignOp(op)) {
            left->accept(this);
            if (right->type()->isPointer()) {
                auto stride = cast<Type::PointerType *>(right->type())->baseType()->size();
                if (stride != 1) {
                    auto &r = stack.back();
                    emit("movq {}, %rax", genReg(r, 8));
                    emit("imull ${}, %eax, %eax", stride);
                    emit("movq %rax, {}", genReg(r, 8));
                }
            }

            right->accept(this);
            if (left->type()->isPointer()) {
                auto stride = cast<Type::PointerType *>(left->type())->baseType()->size();
                if (stride != 1) {
                    auto &r = stack.back();
                    emit("movq {}, %rax", genReg(r, 8));
                    emit("imull ${}, %eax, %eax", stride);
                    emit("movq %rax, {}", genReg(r, 8));
                }
            }
            if (left->type()->isArray() || right->type()->isArray()) {
                iop(expression->tok(), 8);
            } else {
                iop(expression->tok(), size);
            }
        } else {
            size = left->type()->size();
            LValueEvaluator evaluator(*this);
            right->accept(this);
            left->accept(&evaluator);
            auto &rvalue = stack.back();
            auto lvalue = evaluator.stack.back();
            debug("lvalue :{}\n", lvalue);
            store(rvalue, lvalue, size);
        }
    }

    void CodeGenerator::visit(AST::PostfixExpr *expression) {
        auto op = expression->tok();
        expression->first()->accept(this);
        Reg _rdx{true, rdx};
        if (op == "++") {
            LValueEvaluator evaluator(*this);
            expression->first()->accept(&evaluator);
            auto rvalue = evaluator.stack.back();
            emit("leaq {}, %rax", rvalue);
            auto top = stackTopElement();
            auto size = expression->type()->size();
            emit("mov{}  (%rax), {}", getSuffix(size), genReg(_rdx, size));
            emit("mov{}  {}, {}", getSuffix(size), genReg(_rdx, size), genReg(top, size));
            _push(top);
            emit("addl $1, (%rax)");
        } else if (op == "--") {
            LValueEvaluator evaluator(*this);
            expression->first()->accept(&evaluator);
            auto rvalue = evaluator.stack.back();
            emit("leaq {}, %rax", rvalue);
            auto top = stackTopElement();
            auto size = expression->type()->size();
            emit("mov{}  (%rax), {}", getSuffix(size), genReg(_rdx, size));
            emit("mov{}  {}, {}", getSuffix(size), genReg(_rdx, size), genReg(top, size));
            _push(top);
            emit("subl $1, (%rax)");
        } else {
            KCC_NOT_IMPLEMENTED();
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
            auto ty = cast<AST::Expression *>(expression->first())->type();
            if (ty->isArray()) {
                if (expression->type()->isArray()) {

                    return;

                }
                size = 8;
            }
            if (ty->isPointer()) {
                auto p = cast<Type::PointerType *>(ty);
                if (p->baseType()->isStruct()) {
                    debug("pointer to struct\n");
                    return;
                }
            }
            AssertThrow(size == 8);

            auto suffix = getSuffix(size);
            if (top.isMachineReg()) {
                emit("mov{} ({}), {}", suffix, genReg(top, 8), genReg(top, size));
            } else {
                emit("movq {}, %rax", genReg(top, 8));
                emit("movq (%rax),%rax", genReg(top, 8));
                emit("movq %rax, {}", genReg(top, 8));
            }

        } else if (op == "++") {
            LValueEvaluator evaluator(*this);
            expression->first()->accept(&evaluator);
            auto rvalue = evaluator.stack.back();
            emit("leaq {}, %rax", rvalue);
            emit("addl $1, (%rax)");
            push("mov", "(%rax)", expression->type()->size());

        } else if (op == "--") {
            LValueEvaluator evaluator(*this);
            expression->first()->accept(&evaluator);
            auto rvalue = evaluator.stack.back();
            emit("leaq {}, %rax", rvalue);
            emit("subl $1, (%rax)");
            push("mov", "(%rax)", expression->type()->size());

        } else if (op == "sizeof") {
            pushImmInt(cast<AST::TypeDeclaration *>(expression->first())->type->size());
        } else {
            expression->first()->accept(this);
            iUnaryOp(op, expression->type()->size());
        }
    }

    void CodeGenerator::visit(AST::Enum *anEnum) {

    }

    void CodeGenerator::visit(AST::FuncType *type) {

    }


    void CodeGenerator::visit(AST::FuncArgType *type) {

    }


    Reg CodeGenerator::pop() {
        //debug("poped\n");
        setSp(getSp() - 8);
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
        AssertThrow(stack.size() >= 2);
        auto top = pop();
        auto &newTop = stack.back();
        auto _rax = Reg{true, rax};
        auto _rdx = Reg{true, rdx};
        auto _rcx = Reg{true, rdx};
        std::string suffix = getSuffix(size);
        std::string opcode;
        if (op == "/") {
            emit("mov{} {}, {}", suffix, genReg(newTop, size), genReg(_rax, size));
            emit("cltd");
            emit("idiv{} {}", suffix, genReg(top, size));
            emit("mov{} {}, {}", suffix, genReg(_rax, size), genReg(newTop, size));
        } else if (op == "%") {
            emit("mov{} {}, {}", suffix, genReg(newTop, size), genReg(_rax, size));
            emit("cltd");
            emit("idiv{} {}", suffix, genReg(top, size));
            emit("mov{} {}, {}", suffix, genReg(Reg{true, rdx}, size), genReg(newTop, size));
        } else if (op == "<<") {
            emit("pushq %rcx");
            emit("mov{} {}, {}", suffix, genReg(newTop, size), genReg(_rax, size));
            emit("mov{} {}, {}", suffix, genReg(top, size), genReg(_rcx, size));
            emit("sal{} %cl, {}", suffix, genReg(_rcx, size));
            emit("mov{} {}, {}", suffix, genReg(_rcx, size), genReg(newTop, size));
            emit("popq %rcx");
        } else if (op == ">>") {
            emit("pushq %rcx");
            emit("mov{} {}, {}", suffix, genReg(newTop, size), genReg(_rax, size));
            emit("mov{} {}, {}", suffix, genReg(top, size), genReg(_rcx, size));
            emit("sar{} %cl, {}", suffix, genReg(_rcx, size));
            emit("mov{} {}, {}", suffix, genReg(_rcx, size), genReg(newTop, size));
            emit("popq %rcx");
        } else if (comparisonOps.find(op) != comparisonOps.end()) {
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
            emit("cmp{} {}, {}", suffix, genReg(top, size), genReg(_rax, size));
            emit("{} %al", opcode);
            emit("movzbq %al, %rax");
            emit("mov{} {}, {}", suffix, genReg(_rax, size), genReg(newTop, size));

        } else {
            size_t sizeR = size;
            if (op == "+") {
                opcode = "add";
            } else if (op == "-") {
                opcode = "sub";
            } else if (op == "*") {
                opcode = "imul";
            } else if (op == "&") {
                opcode = "and";
            } else if (op == "|") {
                opcode = "or";
            } else if (op == "^") {
                opcode = "xor";
            } else {
                AssertThrow(false);
            }

            // newTop = newTop op top
            if (newTop.isMachineReg() && top.isMachineReg()) {
                emit("{}{} {}, {}", opcode, suffix, genReg(top, sizeR), genReg(newTop, size));
            } else if (newTop.isMachineReg() && !top.isMachineReg()) {
                emit("{}{} {}, {}", opcode, suffix, genReg(top, sizeR), genReg(newTop, size));
            } else if (!newTop.isMachineReg() && !top.isMachineReg()) {
                emit("mov{} {}, {}", suffix, genReg(newTop, size), genReg(_rax, size));
                emit("{}{} {}, {}", opcode, suffix, genReg(top, sizeR), genReg(_rax, size));
                emit("mov{} {}, {}", suffix, genReg(_rax, size), genReg(newTop, size));
            } else {
                KCC_NOT_IMPLEMENTED();
            }
        }
    }

    void CodeGenerator::fop(const std::string &op, size_t size) {
        AssertThrow(stack.size() >= 2);
        auto top = pop();
        auto &newTop = stack.back();
        auto _rax = Reg{true, rax};
        std::string suffix = size == 4 ? "ss" : "sd";
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
            emit("comiss{} {}, {}", suffix, genReg(top, size), genReg(_rax, size));
            emit("{} %al", opcode);
            emit("movzbq %al, %rax");
            emit("mov{} {}, {}", suffix, genReg(_rax, size), genReg(newTop, size));

        } else {
            size_t sizeR = size;
            if (op == "+") {
                opcode = "add";
            } else if (op == "-") {
                opcode = "sub";
            } else if (op == "*") {
                opcode = "mul";
            } else if (op == "/") {
                opcode = "div";
            } else {
                AssertThrow(false);
            }

            // newTop = newTop op top
            if (newTop.isMachineReg() && top.isMachineReg()) {
                emit("{}{} {}, {}", opcode, suffix, genReg(top, sizeR), genReg(newTop, size));
            } else if (newTop.isMachineReg() && !top.isMachineReg()) {
                emit("{}{} {}, {}", opcode, suffix, genReg(top, sizeR), genReg(newTop, size));
            } else if (!newTop.isMachineReg() && !top.isMachineReg()) {
                emit("mov{} {}, {}", suffix, genReg(newTop, size), genReg(toscaTempF, size));
                emit("{}{} {}, {}", opcode, suffix, genReg(top, sizeR), genReg(toscaTempF, size));
                emit("mov{} {}, {}", suffix, genReg(toscaTempF, size), genReg(newTop, size));
            } else {
                KCC_NOT_IMPLEMENTED();
            }
        }
    }

    std::string CodeGenerator::genReg(const Reg &r, size_t size) {
        if (r.isInt) {
            if (r.isMachineReg()) {
                std::string regName;
                if (size == 8)
                    regName = toString((Register) r.index);
                else if (size == 4) {
                    regName = toString((Register32) r.index);
                } else if (size == 1) {
                    regName = toString((Register8) r.index);
                } else {
                    KCC_NOT_IMPLEMENTED();
                }
                return fmt::format("%{}", regName);
            } else {
                AssertThrow(r.index >= 0);
                return fmt::format("-{}(%rbp)", r.index);
            }
        } else {
            if (r.isMachineReg()) {
                return fmt::format("%{}", toString((XmmRegister) r.index));
            } else {
                AssertThrow(r.index >= 0);
                return fmt::format("-{}(%rbp)", r.index);
            }
        }
    }

    Reg CodeGenerator::stackTopElement() {
        if (stack.size() < toscaN) {
            return Reg{true, tosca[stack.size()]};
        } else {
            return Reg{true, int((1 + stack.size() - toscaN) * 8 + getSp())};
        }
    }

    Reg CodeGenerator::stackTopElementF() {
        if (stack.size() < toscaN) {
            return Reg{false, tosca[stack.size()]};
        } else {
            return Reg{false, int((1 + stack.size() - toscaN) * 8 + getSp())};
        }
    }

    void CodeGenerator::_push(const Reg &reg) {
        if (functionPreservedRegisters.find((Register) reg.index) != functionPreservedRegisters.end()) {
            currentFunction.usedPreservedRegisters.insert((Register) reg.index);
        }
        //debug("pushed\n");
        setSp(getSp() + 8);
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

    void CodeGenerator::pushIntValue(int addr, size_t size) {
//        Reg reg = stackTopElement();
//        if (reg.isMachineReg()) {
//            emit("movq {}, {}", genAddr(addr), genReg(reg));
//        } else {
//            emit("movq {}, {}", genAddr(addr), genReg(toscaTemp));
//            emit("movq {}, {}", genReg(toscaTemp), genReg(reg));
//        }
//        _push(reg);
        push("mov", genAddr(addr), size);
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
                out += "\\n";
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
        for (auto &pair : globals) {
            out << fmt::format("    .comm {}, {}, 4\n", pair.second.first, pair.second.second);
        }
        out << "    .text\n";
        for (auto &func :compiledFunctions) {
            out << func.generateFunctionCode();
        }

        out << std::endl;
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
            emit("movzb{} %al, {}", suffix, genReg(top, size));
            return;
        }
        if (op == "-") {
            opcode = "neg";
        } else if (op == "~") {
            opcode = "not";
        } else {
            KCC_NOT_IMPLEMENTED();
        }
        if (top.isMachineReg()) {
            emit("{}{} {}", opcode, suffix, genReg(top, size));
        } else {

            emit("mov{} {}, {}", suffix, genReg(top, size), genReg(_rax, size));
            emit("{}{} {}", opcode, suffix, genReg(_rax, size));
            emit("mov{} {}, {}", suffix, genReg(_rax, size), genReg(top, size));
        }
    }

    void CodeGenerator::pre(AST::AST *ast) {
        //debug("visiting {}\b", ast->str());
    }

    void CodeGenerator::post(AST::AST *ast) {
        Visitor::post(ast);
    }

    void CodeGenerator::visit(AST::Break *aBreak) {
        emit("jmp {}", loopEndLabelStack.back());
    }

    void CodeGenerator::visit(AST::Continue *aContinue) {
        emit("jmp {}", loopStartLabelStack.back());
    }

    void CodeGenerator::visit(AST::MemberAccessExpression *expression) {

        expression->lhs()->accept(this);
        // move the pointer to struct to rax
        emit("movq {}, %rax", genReg(pop(), 8));
        auto offset = expression->offset;
        auto ty = expression->type();
        if (!ty->isStruct()) {
            push("mov", fmt::format("{}(%rax)", offset), ty->size());
        } else {
            push("lea", fmt::format("{}(%rax)", offset), 8);
        }
    }

    void CodeGenerator::ftoi(Reg &reg, int fSize, int iSize) {
        if (reg.isMachineReg()) {
            auto old = reg;
            reg.isInt = true;
            iSize = std::max(iSize, 4);
            if (fSize == 4)
                emit("cvttss2sil {}, {}", genReg(old, fSize), genReg(reg, iSize));
            else {
                AssertThrow(fSize == 8);
                emit("cvttsd2sil {}, {}", genReg(old, fSize), genReg(reg, iSize));
            }
        } else {
            emit("movss {}, {}", genReg(reg, fSize), genReg(toscaTempF, fSize));
            iSize = std::max(iSize, 4);
            reg.isInt = true;
            if (fSize == 4)
                emit("cvttss2sil {}, {}", genReg(toscaTempF, fSize), genReg(reg, iSize));
            else {
                AssertThrow(fSize == 8);
                emit("cvttsd2sil {}, {}", genReg(toscaTempF, fSize), genReg(reg, iSize));
            }
        }
    }

    void CodeGenerator::itof(Reg &reg, int iSize, int fSize) {
        if (reg.isMachineReg()) {
            auto old = reg;
            reg.isInt = false;
            iSize = std::max(iSize, 4);
            if (fSize == 4)
                emit("cvttsi2ssl {}, {}", genReg(old, iSize), genReg(reg, fSize));
            else {
                AssertThrow(fSize == 8);
                emit("cvttsi2sdl {}, {}", genReg(old, iSize), genReg(reg, fSize));
            }
        } else {
            if (fSize == 4)
                emit("cvttsi2ssl {}, {}", genReg(reg, iSize), genReg(toscaTempF, fSize));
            else {
                AssertThrow(fSize == 8);
                emit("cvttsi2sdl {}, {}", genReg(reg, iSize), genReg(toscaTempF, fSize));
            }
            reg.isInt = false;
            emit("movss {}, {}", genReg(toscaTempF, fSize), genReg(reg, fSize));
        }
    }


    std::string FunctionCode::generateFunctionCode() {
        std::ostringstream out;
        out << fmt::format("    .globl {}\n", name);
        out << name << ":\n";
        out << R"(
    pushq %rbp
    movq  %rsp, %rbp
)";

        auto pad = align16(usedPreservedRegisters.size() * 8) - usedPreservedRegisters.size() * 8;
        out << fmt::format("    subq ${}, %rsp\n", bytesAllocated + pad);
        for (auto i: usedPreservedRegisters) {
            out << "    pushq %" << toString(i) << std::endl;
        }

        for (auto &i:code) {
            out << "    " << i << "\n";
        }
        out << functionEndLabel << ":\n";
        for (auto iter = usedPreservedRegisters.rbegin(); iter != usedPreservedRegisters.rend(); iter++) {
            out << "    popq %" << toString(*iter) << std::endl;
        }
        out << R"(
    movq %rbp, %rsp
    popq %rbp
    ret
)";
        out << std::ends;
        return out.str();
    }

    void CodeGenerator::LValueEvaluator::visit(AST::Identifier *identifier) {
        if (!identifier->isGlobal)
            stack.push_back(codeGenerator.genAddr(identifier->addr));
        else {
            auto &pair = codeGenerator.globals.at(identifier->tok());
            codeGenerator.emit("leaq {}(%rip), %rax", pair.first);
            stack.push_back("(%rax)");
        }
    }


    void CodeGenerator::LValueEvaluator::visit(AST::CallExpression *expression) {
        KCC_NOT_IMPLEMENTED();
    }

    void CodeGenerator::LValueEvaluator::visit(AST::CastExpression *expression) {
        expression->second()->accept(this);
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

    void CodeGenerator::LValueEvaluator::visit(AST::MemberAccessExpression *expression) {
        expression->lhs()->accept(&codeGenerator);
        // move the pointer to struct to rax
        codeGenerator.emit("movq {}, %rax", codeGenerator.genReg(codeGenerator.pop(), 8));
        auto offset = expression->offset;
        stack.push_back(fmt::format("{}(%rax)", offset));

    }

}

