#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
namespace  kcc {
	namespace AST {
		class AST;

		class Visitor {
		public:
			virtual void visit(For*) = 0;

			virtual void visit(Identifier*) = 0;

			virtual void visit(While*) = 0;

			virtual void visit(Block*) = 0;

			virtual void visit(TopLevel*) = 0;

			virtual void visit(If*) = 0;

			virtual void visit(TernaryExpression*) = 0;

			virtual void visit(Number*) = 0;

			virtual void visit(Return*) = 0;

			virtual void visit(Empty*) = 0;

			virtual void visit(PrimitiveType*) = 0;

			virtual void visit(PointerType*) = 0;

			virtual void visit(ArrayType*) = 0;

			virtual void visit(ArgumentExepressionList*) = 0;

			virtual void visit(FuncDefArg*) = 0;

			virtual void visit(FuncDef*) = 0;

			virtual void visit(CallExpression*) = 0;

			virtual void visit(CastExpression*) = 0;

			virtual void visit(IndexExpression*) = 0;

			virtual void visit(Declaration*) = 0;

			virtual void visit(DeclarationList*) = 0;

			virtual void visit(Literal*) = 0;

			virtual void visit(BinaryExpression*) = 0;

			virtual void visit(UnaryExpression*) = 0;

			virtual void pre(AST*) {}

			virtual void post(AST*){}

			virtual void visit(Enum*) = 0;

			virtual void visit(FuncType*) = 0;

			virtual void visit(PostfixExpr*) = 0;

			virtual void visit(FuncArgType*) = 0;

			virtual ~Visitor() = default;
			
		};
	}
}
#endif // VISITOR_H
