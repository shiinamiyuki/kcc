//
// Created by xiaoc on 2018/9/11.
//

#include "sema.h"

using namespace kcc;

void kcc::Sema::visit(For *aFor) {
    aFor->init()->accept(this);
    aFor->cond()->accept(this);
    aFor->step()->accept(this);
    aFor->body()->accept(this);
}

void kcc::Sema::visit(Identifier *identifier) {
    auto info = getVarInfo(identifier);
    identifier->setType(info.ty);
    identifier->setAddr(info.addr);
    identifier->setReg(alloc());
}

void kcc::Sema::visit(While *aWhile) {
    aWhile->cond()->accept(this);
    aWhile->body()->accept(this);
}

void kcc::Sema::visit(Block *block) {
    for (auto i:*block) {
        i->accept(this);
    }

}

void kcc::Sema::visit(TopLevel *topLevel) {
    for (auto i:*topLevel) {
        i->accept(this);
    }
}

void kcc::Sema::visit(If *anIf) {
    anIf->cond()->accept(this);
    anIf->body()->accept(this);
    if (anIf->size() == 3)
        anIf->elsePart()->accept(this);
}

void kcc::Sema::visit(TernaryExpression *expression) {
    expression->first()->accept(this);
    expression->second()->accept(this);
    expression->third()->accept(this);
    expression->setType(expression->third()->getType());
}

void kcc::Sema::visit(Number *number) {
    if (number->getToken().type == Token::Type::Int) {
        number->setType(makePrimitiveType("int"));
        number->isFloat = false;
    } else {
        number->isFloat = true;
        number->setType(makePrimitiveType("float"));
    }
    number->setReg(alloc());
}

void kcc::Sema::visit(Return *aReturn) {

}

void kcc::Sema::visit(Empty *empty) {

}

void kcc::Sema::visit(PrimitiveType *type) {

}

void kcc::Sema::visit(PointerType *type) {

}

void kcc::Sema::visit(ArrayType *type) {

}

void kcc::Sema::visit(ArgumentExepressionList *list) {
    for (auto i:*list) {
        i->accept(this);
    }
}

void kcc::Sema::visit(FuncDefArg *arg) {
    for (auto i:*arg) {
        i->accept(this);
    }
}

void kcc::Sema::visit(FuncDef *def) {
    stackFrame.reset();
    Type *ty = def->extractCallSignature();
    addGlobalSymbol(def->name(), ty);
    pushScope();
    def->arg()->accept(this);
    def->block()->accept(this);
    popScope();
    def->frameSize = stackFrame.bytesAllocated;
}

void kcc::Sema::visit(CallExpression *expression) {
    expression->callee()->accept(this);
    expression->arg()->accept(this);
    auto ty = expression->callee()->getType();
    if (ty->kind() != FuncType().kind()) {
        error(expression, "function expected but found '{}'", getTypeRepr(ty));
        expression->setType(nullptr);
        return;
    }
    auto f = (FuncType *) ty;
    auto ret = f->ret();
    auto arg = f->arg();
    if (expression->arg()->size() != arg->size()) {
        error(expression, "expecting {} arguments but found {}",
              arg->size(),
              expression->arg()->size());
        expression->setType(nullptr);
    }
    for (int i = 0; i < expression->arg()->size() && i < arg->size(); i++) {
        auto t = expression->arg()->get(i)->getType();
        if (!t) {

            expression->setType(nullptr);
            return;
        }
        if (!isSameType(t, (Type *) arg->get(i))) {
            error(expression, "expecting type '{}' at {}th argument but found '{}'",
                  getTypeRepr((Type *) arg->get(i)),
                  i + 1,
                  getTypeRepr(t));
            expression->setType(nullptr);
            return;
        }
    }
    expression->setType(ret);
}

void kcc::Sema::visit(CastExpression *expression) {
    expression->second()->accept(this);
    auto cast = expression->second()->getType();
    if (!cast) {
        expression->setType(nullptr);
        return;
    }

    auto type = (Type *) expression->first();
    if (checkTypeCastable(expression, cast, type)) {
        expression->setType(type);
    } else {
        error(expression, "cannot cast type from '{}' to '{}'",
              getTypeRepr(type),
              getTypeRepr(cast));
    }

}

void kcc::Sema::visit(IndexExpression *expression) {

}

void kcc::Sema::visit(Declaration *declaration) {
    auto iden = declaration->identifier();
    auto ty = declaration->type();
    addSymbol(iden->tok(), ty);
}

void kcc::Sema::visit(DeclarationList *list) {
    for (auto i:*list) {
        i->accept(this);
    }
}

void kcc::Sema::visit(Literal *literal) {
    literal->setType(makePointerType(makePrimitiveType("char")));
}

static bool skipCheckIfNull(kcc::BinaryExpression *expr, kcc::Type *ty1, kcc::Type *ty2) {
    if (!ty1 || !ty2) {
        expr->setType(nullptr);
        return true;
    } else {
        return false;
    }
}

void Sema::binaryExpressionAutoPromote(
        BinaryExpression *expression,
        Type *ty1,
        Type *ty2,
        bool intOnly,
        bool retInt) {
    auto op = expression->tok();
    auto err = [&]() {
        error(expression, "invalid operands of types '{}' and '{}' to binary operator '{}'",
              getTypeRepr(ty1),
              getTypeRepr(ty2),
              op);
    };
    if (intOnly) {
        if (isInt(ty1) && isInt(ty2)) {
            expression->setType(ty1);
            expression->setReg(alloc());
        } else {
            err();
        }
    }
    if (isArithmetic(ty1) && isArithmetic(ty2)) {
        if (isInt(ty1) && isInt(ty2)) {
            expression->setType(ty1);
            expression->setReg(alloc());
        } else {
            if (!retInt)
                expression->setType(isFloat(ty1) ? ty1 : ty2);
            else
                expression->setType(isFloat(ty1) ? ty2 : ty1);
            expression->setReg(alloc());
        }
    } else {
        err();
    }

}

void kcc::Sema::visit(BinaryExpression *expression) {
    static std::set<std::string> intOnly = {
            ">>", "<<", "%", "|", "&", "^"
    };
    static std::set<std::string> retInt = {
            "&&", "||", "<", ">", "<=", ">=", "!=", "=="
    };

    expression->lhs()->accept(this);
    expression->rhs()->accept(this);
    auto ty1 = expression->lhs()->getType();
    auto ty2 = expression->rhs()->getType();
    if (skipCheckIfNull(expression, ty1, ty2))
        return;
    const auto &op = expression->tok();
    bool a = intOnly.find(op) != intOnly.end();
    bool b = retInt.find(op) != retInt.end();
    if (op == "+") {
        if (isPointer(ty1)) {
            if (!isInt(ty2)) {
                error(expression, "invalid pointer arithmetic with '{}' and '{}'",
                      getTypeRepr(ty1),
                      getTypeRepr(ty2));
                expression->setType(nullptr);
            } else {
                expression->scale = getTypeSize(removeReference(ty1));
                expression->setType(ty1);
                expression->setReg(alloc());
            }
        } else if (isPointer(ty2)) {
            if (!isInt(ty1)) {
                error(expression, "invalid pointer arithmetic with '{}' and '{}'",
                      getTypeRepr(ty1),
                      getTypeRepr(ty2));
                expression->setType(nullptr);
            } else {
                expression->scale = getTypeSize(removeReference(ty2));
                expression->setType(ty2);
                expression->setReg(alloc());
            }
        } else {
            binaryExpressionAutoPromote(expression, ty1, ty2, a, b);
        }
    } else if (op == "-") {
        if (isPointer(ty1) || isPointer(ty2)) {
            if (isPointer(ty1) && isPointer(ty2)) {
                expression->setType(makePrimitiveType("int"));
            } else {
                error(expression, "invalid pointer arithmetic with {} and {}",
                      getTypeRepr(ty1),
                      getTypeRepr(ty2));
                expression->setType(nullptr);
            }
        } else {
            binaryExpressionAutoPromote(expression, ty1, ty2, a, b);
        }
    } else {
        binaryExpressionAutoPromote(expression, ty1, ty2, a, b);
    }
    expression->isFloat = isFloat(expression->getType());
}

void kcc::Sema::visit(UnaryExpression *expression) {
    auto op = expression->tok();
    expression->expr()->accept(this);
    auto ty = expression->expr()->getType();
    if (!ty) {
        expression->setType(nullptr);
        return;
    }
    if (op == "+" || op == "-") {
        if (isArithmetic(ty))
            expression->setType(ty);
        else {
            error(expression, "wrong type argument '{}' to unary '{}'", getTypeRepr(ty), op);
        }
    }
    if (op == "*") {
        if (isPointer(ty)) {
            expression->setType((Type *) ty->first());
        } else {
            error(expression, "no match for 'operator {}' (operand type is '{}')",
                  op,
                  getTypeRepr(ty));
        }
    }
}

void kcc::Sema::pre(AST *ast) {

}

void kcc::Sema::visit(Enum *anEnum) {

}

void kcc::Sema::visit(FuncType *type) {

}

void kcc::Sema::visit(PostfixExpr *expr) {

}

void kcc::Sema::visit(FuncArgType *type) {

}

void kcc::Sema::addGlobalSymbol(const std::string &s, kcc::Type *ty) {
    symbolTable[0][s] = VarInfo(ty, 0, true);
}

kcc::Sema::Sema() {
    tCount = 0;
    pushScope();
    addTypeSize("int", 4);
    addTypeSize("unsigned int", 4);
    addTypeSize("float", 4);
    addTypeSize("double", 8);
    addTypeSize("char", 4);
}

void kcc::Sema::addSymbol(const std::string &v, kcc::Type *ty, bool isTypedef) {
    auto sz = getTypeSize(ty);
    VarInfo var(ty, stackFrame.bytesAllocated, false, isTypedef);
    //  println("symbol '{}' added, at offset {}",v,(int)stackFrame.bytesAllocated);
    symbolTable.back()[v] = var;
    stackFrame.add(sz);
}

void kcc::Sema::addTypeSize(const std::string &s, unsigned int sz) {
    typeSize[s] = sz;
}

unsigned int kcc::Sema::getTypeSize(kcc::Type *ty) {
    if (ty->isPrimitive()) {
        auto iter = typeSize.find(ty->tok());
        if (iter == typeSize.end()) {
            return 0;
        } else {
            return iter->second;
        }
    } else {
        return KCC_POINTER_SIZE;
    }
}

kcc::VarInfo kcc::Sema::getVarInfo(Identifier *iden) {
    auto s = iden->tok();
    for (auto iter = symbolTable.rbegin(); iter != symbolTable.rend(); iter++) {
        auto &scope = *iter;
        auto it = scope.find(s);
        if (it != scope.end()) {
            return it->second;
        }
    }
    error(iden, "undeclared variable '{}'", s);
    return VarInfo();
}

bool kcc::Sema::isInt(kcc::Type *ty) {
    return ty && ty->isPrimitive() &&
           (ty->tok() == "int"
            || ty->tok() == "char");
}

bool kcc::Sema::isFloat(kcc::Type *ty) {
    return ty && ty->isPrimitive() && ty->tok() == "float";
}

bool kcc::Sema::isPointer(kcc::Type *ty) {
    return ty &&ty->isPointer();
}

std::string kcc::Sema::getTypeRepr(kcc::Type *ty) const {
    return ty->repr();
}

bool Sema::isArithmetic(Type *ty) {
    return isInt(ty) || isFloat(ty);
}

bool Sema::isSameType(Type *ty1, Type *ty2) {
    if (ty1 == ty2) { return true; }
    else {
        if (isPointer(ty1) || isPointer(ty2)) {
            if (isPointer(ty2) && isPointer(ty1)) {
                return isSameType(removeReference(ty1),
                                  removeReference(ty2));
            } else {
                return false;
            }
        } else {
            if (isArithmetic(ty1) || isArithmetic(ty2)) {
                return true;
            } else {
                return ty1->tok() == ty2->tok();
            }
        }
    }
}

bool Sema::checkTypeCastable(CastExpression *expression, Type *from, Type *to) {
    if (isSameType(from, to)) {
        return true;
    }
    if (isPointer(from) && isInt(to)) {
        return true;
    }
    if (isPointer(to) && isInt(from)) {
        return true;
    }
    return false;
}

Type *Sema::removeReference(Type *ty) {
    return (Type *) ty->first();
}

