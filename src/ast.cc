/*
 * ast.cc
 *
 *  Created on: 2018/7/6
 *      Author: xiaoc
 */


#include "ast.h"
#include "fmt/format.h"
#include "visitor.h"
#include "type.hpp"

kcc::AST::AST::AST() {
    isFloat = false;
    isGlobal = false;
}

void kcc::AST::AST::linkRec() {
    for (auto i : children) {
        i->parent = this;
        i->linkRec();
    }
}

kcc::AST::AST::~AST() {
    for (auto i:children) {
        delete i;
    }
    children.clear();
}

void kcc::AST::AST::link() {
    parent = nullptr;
    linkRec();
}

std::string kcc::AST::AST::info() const {
    return format("{}[{}]\n", kind(), content.tok);
}

void kcc::AST::AST::accept(kcc::AST::Visitor *) {}

std::string kcc::AST::AST::str(int depth) const {
    std::string s = "";
    for (int i = 0; i < depth; i++)
        s.append("  ");
    s.append(info());
    for (auto i : children) {
        if (i) {
            s.append(i->str(depth + 1));
        }
    }
    return s;
}

std::string kcc::AST::Expression::info() const {

    if (_type != nullptr) {
        return fmt::format("{}[{}] type: {}\n", kind(), content.tok, _type->toString());
    }
     return AST::info();
}

std::string kcc::AST::ArrayType::info() const {
    return format("{}[{}]\n", kind(), arrSize);
}

const char *printstr(kcc::AST::AST *ast) {
    return ast->str().c_str();
}

#define AST_ACCEPT(classname) void kcc::AST::classname::accept(kcc::AST::Visitor*vis){vis->pre(this);vis->visit(this);}

AST_ACCEPT(Identifier)

AST_ACCEPT(Number)

AST_ACCEPT(BinaryExpression)

AST_ACCEPT(UnaryExpression)

AST_ACCEPT(TernaryExpression)

AST_ACCEPT(CastExpression)

AST_ACCEPT(IndexExpression)

AST_ACCEPT(ArgumentExepressionList)

AST_ACCEPT(CallExpression)

AST_ACCEPT(Block)

AST_ACCEPT(If)

AST_ACCEPT(While)

AST_ACCEPT(TopLevel)

AST_ACCEPT(Declaration)

AST_ACCEPT(PrimitiveType)

AST_ACCEPT(FuncDef)

AST_ACCEPT(FuncDefArg)


AST_ACCEPT(Return)

AST_ACCEPT(Literal)

AST_ACCEPT(ArrayType)

AST_ACCEPT(PointerType)

AST_ACCEPT(DeclarationList)

AST_ACCEPT(For)

AST_ACCEPT(Empty)

AST_ACCEPT(Enum)

AST_ACCEPT(FuncType)

AST_ACCEPT(PostfixExpr)

AST_ACCEPT(FuncArgType)

kcc::AST::FuncType *kcc::AST::FuncDef::extractCallSignature() {
    auto f = new FuncType();
    f->add(first());
    f->add(cast<kcc::AST::FuncDefArg *>(third()));
    return f;
}