//
// Created by xiaoc on 2018/9/11.
//

#ifndef KCC_SEMA_H
#define KCC_SEMA_H

#include "visitor.h"
namespace  kcc {
    struct VarInfo{
        Type * ty;
        int addr;
        bool isGlobal;
        bool isTypedef;
        VarInfo(){
            ty = nullptr;
            addr = false;
            isGlobal = false;
            isTypedef = false;
        }
        VarInfo(Type * _ty,int _addr,bool _isGlobal,bool _isTypedef = false){
            ty = _ty;
            addr = _addr;
            isGlobal = _isGlobal;
            isTypedef = _isTypedef;
        }
    };
    struct Scope: public std::unordered_map<std::string,VarInfo>{

    };
    struct SymbolTable : public std::vector<Scope>{

    };
    class Sema : public Visitor {
        SymbolTable symbolTable;
        void pushScope(){
            symbolTable.emplace_back(Scope());
        }
        void popScope(){
            symbolTable.pop_back();
        }
    public:
        Sema();
        void addGlobalSymbol(const std::string&,Type*);
        void visit(For *aFor) override;

        void visit(Identifier *identifier) override;

        void visit(While *aWhile) override;

        void visit(Block *block) override;

        void visit(TopLevel *root) override;

        void visit(If *anIf) override;

        void visit(TernaryExpression *expression) override;

        void visit(Number *number) override;

        void visit(Return *aReturn) override;

        void visit(Empty *empty) override;

        void visit(PrimitiveType *type) override;

        void visit(PointerType *type) override;

        void visit(ArrayType *type) override;

        void visit(ArgumentExepressionList *list) override;

        void visit(FuncDefArg *arg) override;

        void visit(FuncDef *def) override;

        void visit(CallExpression *expression) override;

        void visit(CastExpression *expression) override;

        void visit(IndexExpression *expression) override;

        void visit(Declaration *declaration) override;

        void visit(DeclarationList *list) override;

        void visit(Literal *literal) override;

        void visit(BinaryExpression *expression) override;

        void visit(UnaryExpression *expression) override;

        void pre(AST *ast) override;

        void visit(Enum *anEnum) override;

        void visit(FuncType *type) override;

        void visit(PostfixExpr *expr) override;

        void visit(FuncArgType *type) override;

        ~Sema() override = default;
    };

}
#endif //KCC_SEMA_H
