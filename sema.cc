//
// Created by xiaoc on 2018/9/11.
//

#include "sema.h"

void kcc::Sema::visit(For *aFor) {

}

void kcc::Sema::visit(Identifier *identifier) {

}

void kcc::Sema::visit(While *aWhile) {

}

void kcc::Sema::visit(Block *block) {

}

void kcc::Sema::visit(TopLevel *topLevel) {
    for(auto i:*topLevel){
        i->accept(this);
    }
}

void kcc::Sema::visit(If *anIf) {

}

void kcc::Sema::visit(TernaryExpression *expression) {

}

void kcc::Sema::visit(Number *number) {

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

}

void kcc::Sema::visit(FuncDefArg *arg) {

}

void kcc::Sema::visit(FuncDef *def) {
    Type * ty = def->extractCallSignature();
    addGlobalSymbol(def->name(),ty);
    pushScope();
    def->arg()->accept(this);
    def->block()->accept(this);
    popScope();
}

void kcc::Sema::visit(CallExpression *expression) {

}

void kcc::Sema::visit(CastExpression *expression) {

}

void kcc::Sema::visit(IndexExpression *expression) {

}

void kcc::Sema::visit(Declaration *declaration) {

}

void kcc::Sema::visit(DeclarationList *list) {

}

void kcc::Sema::visit(Literal *literal) {

}

void kcc::Sema::visit(BinaryExpression *expression) {

}

void kcc::Sema::visit(UnaryExpression *expression) {

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
    symbolTable[0][s] = VarInfo(ty,0,true);
}

kcc::Sema::Sema() {
    pushScope();
}
