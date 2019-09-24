#include "sema.h"
#include "ast.h"
#include "log.h"
#include <mutex>

namespace kcc {
    void Sema::error(const std::string &message) {
        std::cout.flush();
        std::cerr.flush();
        auto pos = sourcePosStack.back();
        fmt::print(stderr, "error: {} at {}:{}:{}\n", message, pos.filename, pos.line, pos.col);
        std::exit(-1);
    }

    void Sema::visit(AST::For *aFor) {
        pushScope();
        auto init = aFor->init();
        auto cond = aFor->cond();
        auto step = aFor->step();
        init->accept(this);
        cond->accept(this);
        step->accept(this);
        aFor->body()->accept(this);
        popScope();
    }

    void Sema::visit(AST::Identifier *identifier) {
        auto iden = identifier->tok();
        identifier->type() = table.find(iden).type;
        identifier->addr = table.find(iden).addr;
        identifier->isGlobal = table.find(iden).isGlobal;
    }

    void Sema::visit(AST::While *whileStmt) {
        whileStmt->cond()->accept(this);
        whileStmt->body()->accept(this);
    }

    void Sema::pushScope() {
        table.pushScope();
    }

    void Sema::popScope() {
        table.popScope();
    }

    void Sema::visit(AST::Block *block) {
        pushScope();
        for (auto i : *block) {
            i->accept(this);
        }
        if (!table.underGlobal()) {
            funcLocalAllocatedSize = std::max(funcLocalAllocatedSize, table.symTable->localOffset);
        }
        popScope();
    }

    void Sema::visit(AST::TopLevel *top) {
        for (auto i : *top) {
            i->accept(this);
        }
    }

    void Sema::visit(AST::If *ifStmt) {
        ifStmt->cond()->accept(this);
        ifStmt->ifPart()->accept(this);
        if (ifStmt->size() == 3)
            ifStmt->elsePart()->accept(this);
    }

    void Sema::visit(AST::TernaryExpression *expression) {
        auto cond = expression->cond();
        auto part1 = expression->part1();
        auto part2 = expression->part2();
        cond->accept(this);
        part1->accept(this);
        part2->accept(this);
        expression->type() = part1->type();

    }

    void Sema::visit(AST::Number *number) {
        auto num = number->tok();
        auto pos = num.find('.');
        if (pos == std::string::npos) {
            number->type() = Type::getPrimitiveTypes()[Type::EInt];
        } else {
            number->type() = Type::getPrimitiveTypes()[Type::EDouble];
        }
    }

    void Sema::visit(AST::Return *aReturn) {
        if (aReturn->size() == 1) {
            aReturn->first()->accept(this);
        }
    }

    void Sema::visit(AST::Empty *) {
    }

    void Sema::visit(AST::PrimitiveType *type) {
        static std::unordered_map<std::string, Type::IType *> primitiveTypeMap;
        static std::once_flag flag;

        std::call_once(flag, [&]() {
            primitiveTypeMap["int"] = Type::getPrimitiveTypes()[Type::EInt];
            primitiveTypeMap["char"] = Type::getPrimitiveTypes()[Type::EChar];
            primitiveTypeMap["float"] = Type::getPrimitiveTypes()[Type::EFloat];
            primitiveTypeMap["double"] = Type::getPrimitiveTypes()[Type::EDouble];
            primitiveTypeMap["void"] = Type::getPrimitiveTypes()[Type::EVoid];
        });

        type->type = primitiveTypeMap.at(type->repr());
    }

    void Sema::visit(AST::PointerType *type) {
        auto baseType = type->ptrTo();
        baseType->accept(this);
        type->type = new Type::PointerType(baseType->type);
    }

    void Sema::visit(AST::ArrayType *) {
    }

    void Sema::visit(AST::ArgumentExepressionList *) {
    }

    void Sema::visit(AST::FuncDefArg *args) {
        for (auto i : *args) {
            i->accept(this);
        }
    }

    Type::FunctionType *convert(AST::FuncType *type) {
        std::vector<Type::IType *> args;
        for (auto i : *type->arg()) {
            if (typeid(*i) == typeid(AST::Declaration)) {
                auto decl = cast<AST::Declaration *>(i);
                AssertThrow(decl);
                AssertThrow(decl->type());
                AssertThrow(decl->type()->type);
                args.emplace_back(decl->type()->type);
            } else {
                auto variadic = cast<AST::VariadicArgType *>(i);
                args.emplace_back(new Type::VariadicType());
            }
        }
        return new Type::FunctionType(type->ret()->type, args);
    }

    void Sema::visit(AST::FuncType *func) {

        auto ret = func->ret();
        pushScope();
        auto args = func->arg();
        ret->accept(this);
        args->accept(this);
        func->type = convert(func);
        debug("func type {}\n", func->str());
        popScope();
    }

    void Sema::visit(AST::FuncDef *func) {
        funcLocalAllocatedSize = 0;
        auto &funcname = func->name();
        auto ret = func->ret();
        auto args = func->arg();
        pushScope();
        ret->accept(this);
        args->accept(this);
        auto type = func->extractCallSignature();
        debug("type:{}\n", type->str());
        auto funcType = convert(type);
        debug("added func {}: {}\n", funcname, funcType->toString());
        table.symTable->parentScope->addSymbol(funcname, funcType);
        func->block()->accept(this);
        popScope();
        func->localAllocatedSize = align16(funcLocalAllocatedSize);
        funcLocalAllocatedSize = 0;
        debug("allocated {} bytes for local variables\n", func->localAllocatedSize);
    }

    void Sema::visit(AST::CallExpression *callExpression) {
        auto func = cast<AST::Expression *>(callExpression->first());
        auto arg = callExpression->arg();
        func->accept(this);
        for (auto i: *arg) {
            i->accept(this);
        }
        debug("checking call expression\n");
        auto _func_type = func->type();
        if (_func_type->isFunction()) {
            auto func_type = cast<Type::FunctionType *>(_func_type);
            bool variadic = false;
            size_t NArgsToBeChecked = func_type->args.size();
            if (func_type->args.size() > 1 && typeid(*func_type->args.back()) == typeid(Type::VariadicType)) {
                variadic = true;
                NArgsToBeChecked--;
            }
            auto expected = func_type->args.size();
            auto actual = arg->size();
            if (!variadic) {
                if (expected != actual) {
                    error(fmt::format("expected {} arguments but have {}", expected, actual));
                }
            } else if (variadic && actual < NArgsToBeChecked) {
                error(fmt::format("expected at least {} arguments but have {}", NArgsToBeChecked, actual));
            }
            for (int i = 0; i < NArgsToBeChecked; i++) {
                auto e = cast<AST::Expression *>(arg->get(i));
                if (!Type::convertible(e->type(), func_type->args[i])) {
                    error(fmt::format("cannot convert argument {} from {} to {}",
                                      i + 1, e->type()->toString(), func_type->args[i]->toString()));
                }
            }
            func->type() = func_type;
            callExpression->type() = func_type->ret;
            AssertThrow(func_type->ret);
        } else {
            error("expected to call a function");
        }
    }

    void Sema::visit(AST::CastExpression *expression) {
        auto ty = cast<AST::TypeDeclaration *>(expression->first());
        auto e = cast<AST::Expression *>(expression->second());
        ty->accept(this);
        e->accept(this);
        if (Type::canCast(e->type(), ty->type)) {
            expression->type() = ty->type;
        } else {
            error(fmt::format("cannot cast from {} to {}", e->type()->toString(), ty->type->toString()));
        }

    }

    void Sema::visit(AST::IndexExpression *) {
    }


    void Sema::visit(AST::Declaration *decl) {
        auto typeNode = decl->type();
        auto identifier = decl->identifier();
        typeNode->accept(this);
        auto iden = identifier->getToken();
        auto type = typeNode->type;
        std::string idenName = iden.tok;
        if (typeid(*decl->getParent()) == typeid(AST::FuncDefArg)) {
            // omitted args
            if (idenName.empty()) {
                idenName = std::string("##").append(std::to_string(intRegCounter));
            }
        }
        if (decl->size() == 3) {
            auto init = decl->init();
            init->accept(this);
            if (!Type::checkBinaryExpr(type, init->type(), "=")) {
                error(fmt::format("cannot assign {} to {}", init->type()->toString(), type->toString()));
            }
        }

        table.addSymbol(iden.tok, type);
        identifier->isGlobal = table.find(iden.tok).isGlobal;
        identifier->type() = type;
        identifier->addr = table.find(idenName).addr;
        debug("added {}: {} at {}\n", idenName, type->toString(), table.find(idenName).addr);
    }

    void Sema::visit(AST::DeclarationList *list) {
        for (auto i : *list) {
            i->accept(this);
        }
    }

    void Sema::visit(AST::Literal *literal) {
        literal->type() = new Type::PointerType(Type::getPrimitiveTypes()[Type::EChar]);
    }

    void Sema::visit(AST::MemberAccessExpression *expr) {
        if (expr->tok() == ".") {
            auto member = dynamic_cast<AST::Identifier *>(expr->rhs());
            if (!member) {
                error(fmt::format("must have struct/union member on the right of '.'"));
            }
            expr->lhs()->accept(this);
            auto ty = expr->lhs()->type();
            auto memberName = member->tok();
            auto structType = dynamic_cast<Type::StructType *>(ty);
            if (!structType) {
                error(fmt::format("must have struct/union on the left of '.'"));
            }
            if (structType->contains(memberName)) {
                expr->type() = structType->get(memberName).type;
                expr->offset = structType->get(memberName).offset;
            } else {
                error(fmt::format("{} does not have member named {}", structType->name, memberName));
            }
        } else if (expr->tok() == "->") {
            auto member = dynamic_cast<AST::Identifier *>(expr->rhs());
            if (!member) {
                error(fmt::format("must have struct/union member on the right of '.'"));
            }
            expr->lhs()->accept(this);
            auto ty = expr->lhs()->type();
            if (!ty->isPointer()) {
                error(fmt::format("must have pointer to struct/union on the left of '->'"));
            }
            auto memberName = member->tok();
            auto structType = dynamic_cast<Type::StructType *>(cast<Type::PointerType *>(ty)->baseType());
            if (structType->contains(memberName)) {
                expr->type() = structType->get(memberName).type;
                expr->offset = structType->get(memberName).offset;
            } else {
                error(fmt::format("{} does not have member named {}", structType->name, memberName));
            }
        } else {
            AssertThrow(false);
        }
    }

    void Sema::visit(AST::BinaryExpression *expr) {
        expr->rhs()->accept(this);
        expr->lhs()->accept(this);
        debug("checking type\n");
        auto ty = Type::checkBinaryExpr(expr->lhs()->type(), expr->rhs()->type(), expr->tok());
        if (!ty) {
            error(format("cannot have {} between {} and {}",
                         expr->tok(),
                         expr->rhs()->type()->toString(),
                         expr->lhs()->type()->toString()));
        }
        expr->type() = ty;

    }

    void Sema::visit(AST::UnaryExpression *expr) {
        if (expr->tok() == "sizeof") {
//            auto type = cast<AST::TypeDeclaration*>(expr->first());
//            type->accept(this);
            expr->type() = Type::getPrimitiveTypes()[Type::EInt];
            expr->first()->accept(this);
            return;
        }
        expr->expr()->accept(this);
        auto ty = expr->expr()->type();
        if (expr->tok() == "*") {

            if (!ty->isPointer()) {
                error(format("cannot dereference type {}", ty->toString()));
            } else {
                expr->type() = cast<Type::PointerType *>(ty)->baseType();
            }
        } else if (expr->tok() == "&") {
            LValueChecker checker(this);
            expr->first()->accept(&checker);
            if (checker.stack.back()) {
                expr->type() = new Type::PointerType(ty);
            } else {
                error(format("lvalue required on '&' operand", ty->toString(), expr->tok()));
            }
        } else if (expr->tok() == "++" || expr->tok() == "--") {
            if (ty->isPrimitive() || ty->isPointer()) {
                expr->type() = ty;
            } else {
                error(format("cannot have {} on unary operator {}", ty->toString(), expr->tok()));
            }
        } else if (expr->tok() == "!") {
            expr->type() = Type::getPrimitiveTypes()[Type::EInt];
        } else if (expr->tok() == "+" || expr->tok() == "-") {
            if (ty->isPrimitive() || ty->isPointer()) {
                expr->type() = ty;
            } else {
                error(format("cannot have {} on unary operator {}", ty->toString(), expr->tok()));
            }
        } else {
            KCC_NOT_IMPLEMENTED();
        }
    }

    void Sema::pre(AST::AST *ast) {
        //  debug("visiting {}\b", ast->str());
        sourcePosStack.push_back(ast->pos);
    }

    void Sema::post(AST::AST *ast) {
        sourcePosStack.pop_back();
    }

    void Sema::visit(AST::Enum *) {
    }

    void Sema::visit(AST::PostfixExpr *expr) {
        expr->first()->accept(this);
        auto ty = cast<AST::Expression *>(expr->first())->type();
        if (ty->isPointer() || ty->isPrimitive()) {
            expr->type() = ty;
        } else {
            error(fmt::format("cannot have {} on {}", expr->tok(), ty->toString()));
        }
    }

    void Sema::visit(AST::FuncArgType *) {
    }

    void Sema::visit(AST::DoWhile *aWhile) {
        pushScope();
        for (auto i: *aWhile->body()) {
            i->accept(this);
        }
        aWhile->cond()->accept(this);
        popScope();
    }

    void Sema::visit(AST::StructDecl *decl) {
        size_t size = 0;
        std::vector<Type::StructMember> members;
        auto ty = new Type::StructType();
        ty->name = decl->name;
        VarInfo info(ty->name, ty, 0);
        info.isStruct = true;
        table.addSymbol(info);
        debug("added {}\n", ty->name);
        for (auto i: *decl) {
            for (auto _: *i) {
                auto member = cast<AST::Declaration *>(_);
                member->type()->accept(this);
                auto memberType = member->type()->type;
                Type::StructMember structMember;
                structMember.type = memberType;
                structMember.name = member->identifier()->tok();
                if (memberType->size() >= 8)
                    size = align8(size);
                structMember.offset = size;
                size += memberType->size();
                members.push_back(structMember);
            }
        }

        for (auto &m:members) {
            ty->add(m.name, m);
        }

        ty->setSize(align8(size));
    }

    void Sema::visit(AST::StructType *type) {
        auto info = table.find(type->name);
        if (!info.isStruct) {
            error(fmt::format("{} is not struct", info.name));
        }
        type->type = info.type;
    }


    void Sema::LValueChecker::visit(AST::Identifier *identifier) {
        stack.push_back(1);
    }

    void Sema::LValueChecker::visit(AST::TernaryExpression *expression) {
        expression->part1()->accept(this);
        expression->part2()->accept(this);
        auto r1 = stack.back();
        stack.pop_back();
        auto r2 = stack.back();
        stack.pop_back();
        if (r1 && r2) {
            stack.push_back(1);
        } else {
            stack.push_back(0);
        }
    }

    void Sema::LValueChecker::visit(AST::Number *number) {
        stack.push_back(0);
    }

    void Sema::LValueChecker::visit(AST::CallExpression *expression) {
        stack.push_back(0);
    }

    void Sema::LValueChecker::visit(AST::CastExpression *expression) {
        expression->first()->accept(this);

    }

    void Sema::LValueChecker::visit(AST::Literal *literal) {
        stack.push_back(1);
    }

    void Sema::LValueChecker::visit(AST::BinaryExpression *expression) {
        expression->lhs()->accept(this);
        expression->rhs()->accept(this);
        auto r1 = stack.back();
        stack.pop_back();
        auto r2 = stack.back();
        stack.pop_back();
        if (r1 || r2) {
            stack.push_back(1);
        } else {
            stack.push_back(0);
        }
    }

    void Sema::LValueChecker::visit(AST::UnaryExpression *expression) {
        stack.push_back(0);
    }

    void Sema::LValueChecker::visit(AST::PostfixExpr *expr) {
        stack.push_back(0);
    }
}