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

    void Sema::visit(AST::For *) {
    }

    void Sema::visit(AST::Identifier *identifier) {
        auto iden = identifier->tok();
        identifier->type() = table.find(iden).type;
        identifier->addr = table.find(iden).addr;
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
        ifStmt->elsePart()->accept(this);
    }

    void Sema::visit(AST::TernaryExpression *) {
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
        if(aReturn->size() == 1){
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
            auto decl = cast<AST::Declaration *>(i);
            AssertThrow(decl);
            AssertThrow(decl->type());
            AssertThrow(decl->type()->type);
            args.emplace_back(decl->type()->type);
        }
        return new Type::FunctionType(type->ret()->type, args);
    }

    void Sema::visit(AST::FuncType *func) {

        auto ret = func->ret();
        auto args = func->arg();
        ret->accept(this);
        args->accept(this);
        func->type = convert(func);
        debug("func type {}\n", func->str());
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
        table.addSymbol(funcname, createValue(funcType), funcType);
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
        debug("checking call expression");
        auto _func_type = func->type();
        if (_func_type->isFunction()) {
            auto func_type = cast<Type::FunctionType *>(_func_type);
            auto expected = func_type->args.size();
            auto actual = arg->size();
            if (expected != actual) {
                error(fmt::format("expected {} arguments but have {}", expected, actual));
            }
            for (int i = 0; i < arg->size(); i++) {
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

    void Sema::visit(AST::CastExpression *) {
    }

    void Sema::visit(AST::IndexExpression *) {
    }

    Value Sema::createValue(Type::IType *type) {
        if (type->isFloat()) {
            return Value::CreateFloatReg(floatRegCounter++);
        } else {
            return Value::CreateIntReg(intRegCounter++);
        }
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
                error(fmt::format("cannot assign {} with {}", init->type()->toString(), type->toString()));
            }
        }

        table.addSymbol(iden.tok, createValue(type), type);
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

    void Sema::visit(AST::BinaryExpression *expr) {
        expr->rhs()->accept(this);
        expr->lhs()->accept(this);
        debug("checking type\n");
        auto ty = Type::checkBinaryExpr(expr->lhs()->type(), expr->rhs()->type(), expr->tok());
        if (!ty) {
            error(format("cannot convert from {} to {}",
                         expr->rhs()->type()->toString(),
                         expr->lhs()->type()->toString()));
        }
        expr->type() = ty;
    }

    void Sema::visit(AST::UnaryExpression *expr) {
        expr->expr()->accept(this);
        auto ty = expr->expr()->type();
        if (expr->tok() == "*") {

            if (!ty->isPointer()) {
                error(format("cannot dereference type {}", ty->toString()));
            } else {
                expr->type() = cast<Type::PointerType *>(ty)->baseType();
            }
        } else {
            if (ty->isPrimitive()) {
                expr->type() = ty;
            } else {
                error(format("cannot have {} on unary operator {}", ty->toString(), expr->tok()));
            }
        }
    }

    void Sema::pre(AST::AST *ast) {
        debug("visiting {}\b", ast->str());
        sourcePosStack.push_back(ast->pos);
    }

    void Sema::post(AST::AST *ast) {
        sourcePosStack.pop_back();
    }

    void Sema::visit(AST::Enum *) {
    }

    void Sema::visit(AST::PostfixExpr *) {
    }

    void Sema::visit(AST::FuncArgType *) {
    }


}