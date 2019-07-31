#ifndef KCC_SEMA_H
#define KCC_SEMA_H

#include "visitor.h"


namespace kcc {
	class Sema : public AST::Visitor{
	public:
		// Inherited via Visitor
		virtual void visit(AST::For*) override;
		virtual void visit(AST::Identifier*) override;
		virtual void visit(AST::While*) override;
		virtual void visit(AST::Block*) override;
		virtual void visit(AST::TopLevel*) override;
		virtual void visit(AST::If*) override;
		virtual void visit(AST::TernaryExpression*) override;
		virtual void visit(AST::Number*) override;
		virtual void visit(AST::Return*) override;
		virtual void visit(AST::Empty*) override;
		virtual void visit(AST::PrimitiveType*) override;
		virtual void visit(AST::PointerType*) override;
		virtual void visit(AST::ArrayType*) override;
		virtual void visit(AST::ArgumentExepressionList*) override;
		virtual void visit(AST::FuncDefArg*) override;
		virtual void visit(AST::FuncDef*) override;
		virtual void visit(AST::CallExpression*) override;
		virtual void visit(AST::CastExpression*) override;
		virtual void visit(AST::IndexExpression*) override;
		virtual void visit(AST::Declaration*) override;
		virtual void visit(AST::DeclarationList*) override;
		virtual void visit(AST::Literal*) override;
		virtual void visit(AST::BinaryExpression*) override;
		virtual void visit(AST::UnaryExpression*) override;
		virtual void pre(AST::AST*) override;
		virtual void visit(AST::Enum*) override;
		virtual void visit(AST::FuncType*) override;
		virtual void visit(AST::PostfixExpr*) override;
		virtual void visit(AST::FuncArgType*) override;
	};

}


#endif