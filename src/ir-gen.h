//
// Created by xiaoc on 2018/9/17.
//

#ifndef KCC_IR_GEN_H
#define KCC_IR_GEN_H

#include "visitor.h"
#include "ir.h"
#include "cfg.h"

;
namespace kcc {
    class DirectCodeGen;
    class IRGenerator : public Visitor {
        std::vector<Function> funcs;


    public:

        void visit(For *aFor) override;

        void visit(Identifier *identifier) override;

        void visit(While *aWhile) override;

        void visit(Block *block) override;

        void visit(TopLevel *level) override;

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

        ~IRGenerator() override = default;

        std::vector<IRNode> &ir() { return funcs.back().ir; }

        template<typename ...Args>
        void emit(Opcode op, Args... args) {
            ir().emplace_back(IRNode(op, args...));
        }

        template<typename ...Args>
        void patch(int idx, Opcode op, Args... args) {
            ir()[idx] = (IRNode(op, args...));
        }

        void printIR() {
            int cnt = 0;
            for (auto i: ir()) {
                println("{}: {}", cnt, i.dump());
                cnt++;
            }
        }

        void buildSSA();

        void gen(DirectCodeGen &generator);
    };


}

#endif //KCC_IR_GEN_H
