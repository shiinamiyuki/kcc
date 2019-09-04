#ifndef KCC_SEMA_H
#define KCC_SEMA_H

#include "visitor.h"
#include "value.hpp"
#include "type.hpp"

namespace kcc {
	struct VarInfo {
		std::string name;
		Value value;
		Type::IType* type = nullptr;
		VarInfo()=default;
		VarInfo(const std::string&name, const Value & v, Type::IType* t):name(name),value(v),type(t){}
	};
	struct ScopedSymbolTable {
		std::unordered_map<std::string, VarInfo> infos;
		ScopedSymbolTable* parentScope = nullptr;
		void addSymbol(const std::string& name, Value value, Type::IType* type) {
			AssertThrow(type);
			VarInfo info(name, value, type);
			infos[name] = info;
		}
		const VarInfo& find(const std::string& name)const {
			auto iter = infos.find(name);
			if (iter != infos.end())
				return iter->second;
			if (parentScope) {
				return parentScope->find(name);
			}
			else {
				throw std::runtime_error(format("No var named {}", name));
			}
		}
	};
	struct SymbolTable {
		ScopedSymbolTable* symTable;
		SymbolTable() {
			symTable = new ScopedSymbolTable();
		}
		void addSymbol(const std::string& name, Value value, Type::IType* type) {
			AssertThrow(type);
			symTable->addSymbol(name, std::move(value), type);
		}
		const VarInfo& find(const std::string& name)const {
			return symTable->find(name);
		}
		void pushScope() {
			auto tmp = new ScopedSymbolTable();
			tmp->parentScope = symTable;
			symTable = tmp;
		}
		void popScope() {
			auto tmp = symTable;
			symTable = symTable->parentScope;
			delete tmp;
		}
	};
	class Sema : public AST::Visitor {
		SymbolTable table;
		Value createValue(Type::IType* type);
		size_t floatRegCounter = 0;
		size_t intRegCounter = 0;
	public:
		void error(const std::string& message);
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