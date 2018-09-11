//
// Created by xiaoc on 2018/9/11.
//

#include "sema.h"

void kcc::Sema::visit(For *aFor) {

}

void kcc::Sema::visit(Identifier *identifier) {
    auto info = getVarInfo(identifier);
    identifier->setType(info.ty);
}

void kcc::Sema::visit(While *aWhile) {

}

void kcc::Sema::visit(Block *block) {
    for(auto i:*block){
        i->accept(this);
    }

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
    if(number->getToken().type == Token::Type::Int){
        number->setType(makePrimitiveType("int"));
    }else{
        number->setType(makePrimitiveType("float"));
    }
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
    stackFrame.reset();
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
    auto iden = declaration->identifier();
    auto ty = declaration->type();
    addSymbol(iden->tok(),ty);
}

void kcc::Sema::visit(DeclarationList *list) {
    for(auto i:*list){
        i->accept(this);
    }
}

void kcc::Sema::visit(Literal *literal) {

}

void kcc::Sema::visit(BinaryExpression *expression) {
    expression->lhs()->accept(this);
    expression->rhs()->accept(this);
    auto ty1 = expression->getType();
    auto ty2 = expression->getType();
    const auto& op = expression->tok();
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
    addTypeSize("int",4);
    addTypeSize("unsigned int",4);
    addTypeSize("float",4);
    addTypeSize("double",8);
    addTypeSize("char",4);
}

void kcc::Sema::addSymbol(const std::string &v, kcc::Type *ty,bool isTypedef) {
    auto sz = getTypeSize(ty);
    VarInfo var(ty,stackFrame.bytesAllocated,false,isTypedef);
    symbolTable.back()[v] = var;
    stackFrame.add(sz);
}

void kcc::Sema::addTypeSize(const std::string &s, unsigned int sz) {
    typeSize[s] = sz;
}

unsigned int kcc::Sema::getTypeSize(kcc::Type *ty) {
    if(ty->isPrimitive()){
        auto iter = typeSize.find(ty->tok());
        if(iter == typeSize.end()){
            return 0;
        }else{
            return iter->second;
        }
    }else{
        return KCC_POINTER_SIZE;
    }
}

kcc::VarInfo kcc::Sema::getVarInfo(Identifier * iden) {
    auto s = iden->tok();
    for(auto iter = symbolTable.rbegin();iter!=symbolTable.rend();iter++){
        auto& scope = *iter;
        auto it = scope.find(s);
        if(it != scope.end()){
            return it->second;
        }
    }
    error(iden,"undeclared variable {}",s);
    return VarInfo();
}
