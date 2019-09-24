#ifndef VISITOR_H
#define VISITOR_H

#include "ast.h"

namespace kcc {
    namespace AST {
        class AST;

        class Visitor {
        public:
            virtual void visit(For *) {}

            virtual void visit(Identifier *) {}

            virtual void visit(While *) {}

            virtual void visit(DoWhile *) {}

            virtual void visit(Block *) {}

            virtual void visit(TopLevel *) {}

            virtual void visit(If *) {}

            virtual void visit(TernaryExpression *) {}

            virtual void visit(Number *) {}

            virtual void visit(Return *) {}

            virtual void visit(Empty *) {}

            virtual void visit(PrimitiveType *) {}

            virtual void visit(PointerType *) {}

            virtual void visit(ArrayType *) {}

            virtual void visit(ArgumentExepressionList *) {}

            virtual void visit(FuncDefArg *) {}

            virtual void visit(FuncDef *) {}

            virtual void visit(CallExpression *) {}

            virtual void visit(CastExpression *) {}

            virtual void visit(IndexExpression *) {}

            virtual void visit(Declaration *) {}

            virtual void visit(DeclarationList *) {}

            virtual void visit(Literal *) {}

            virtual void visit(BinaryExpression *) {}

            virtual void visit(UnaryExpression *) {}

            virtual void pre(AST *) {}

            virtual void post(AST *) {}

            virtual void visit(Enum *) {}

            virtual void visit(Break *) {}

            virtual void visit(Continue *) {}

            virtual void visit(FuncType *) {}

            virtual void visit(VariadicArgType *) {}

            virtual void visit(PostfixExpr *) {}

            virtual void visit(FuncArgType *) {}

            virtual void visit(StructDecl *) {}

            virtual void visit(StructType *) {}

            virtual void visit(MemberAccessExpression *) {}

            virtual void visit(ForwardStructDecl *) {}

            virtual ~Visitor() = default;

        };
    }
}
#endif // VISITOR_H
