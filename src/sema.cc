#include "sema.h"
#include "ast.h"
#include "log.h"
#include <mutex>

namespace kcc {
	void Sema::visit(AST::For*)
	{
	}
	void Sema::visit(AST::Identifier*)
	{
	}
	void Sema::visit(AST::While*)
	{
	}
	void Sema::visit(AST::Block*block)	{
		for (auto i : *block) {
			i->accept(this);
		}
	}
	void Sema::visit(AST::TopLevel* top)
	{
		for (auto i : *top) {
			i->accept(this); 
		}
	}
	void Sema::visit(AST::If*)
	{
	}
	void Sema::visit(AST::TernaryExpression*)
	{
	}
	void Sema::visit(AST::Number*)
	{
	}
	void Sema::visit(AST::Return*)
	{
	}
	void Sema::visit(AST::Empty*)
	{
	}
	void Sema::visit(AST::PrimitiveType* type) {
		static std::unordered_map<std::string, Type::IType*> primitiveTypeMap;
		static std::once_flag flag;
		std::call_once(flag, [&]() {
			primitiveTypeMap["int"] = Type::typeInt;
			primitiveTypeMap["char"] = Type::typeChar;
		});
		type->type = primitiveTypeMap.at(type->repr());
	}
	void Sema::visit(AST::PointerType*type)	{
		auto baseType = type->ptrTo();
		baseType->accept(this);
		type->type = new Type::PointerType(baseType->type);
	}
	void Sema::visit(AST::ArrayType*)
	{
	}
	void Sema::visit(AST::ArgumentExepressionList*)
	{
	}
	void Sema::visit(AST::FuncDefArg*args)	{
		for (auto i : *args) {
			i->accept(this);
		}
	}
	Type::FunctionType* convert(AST::FuncType* type) {
		std::vector<Type::IType*> args;
		for (auto i : *type->arg()) {
			auto decl = cast<AST::Declaration*>(i);
			AssertThrow(decl);
			AssertThrow(decl->type());
			AssertThrow(decl->type()->type);
			args.emplace_back(decl->type()->type);
		}
		return new Type::FunctionType(type->ret()->type, args);
	}
	void Sema::visit(AST::FuncType *func)	{
		auto ret = func->ret();
		auto args = func->arg();
		ret->accept(this);
		args->accept(this);
		func->type = convert(func);
		log("func type {}\n", func->str());
	}
	void Sema::visit(AST::FuncDef* func)	{
		auto& funcname = func->name();
		log("func {}\n", funcname);
		auto ret = func->ret();
		auto args = func->arg();	
		ret->accept(this);
		args->accept(this);
		auto type = func->extractCallSignature();
		log("type:{}\n", type->str());
		auto funcType = convert(type);
		log("added func {}: {}\n", funcname, funcType->toString());
		table.addSymbol(funcname, createValue(funcType), funcType);
		func->block()->accept(this);
	}
	void Sema::visit(AST::CallExpression*)
	{
	}
	void Sema::visit(AST::CastExpression*)
	{
	}
	void Sema::visit(AST::IndexExpression*)
	{
	}

	Value Sema::createValue(Type::IType* type) {
		if (type->isFloat()) {
			return Value::CreateFloatReg(floatRegCounter++);
		}
		else {
			return Value::CreateIntReg(intRegCounter++);
		}
	}
	void Sema::visit(AST::Declaration* decl) {
		auto typeNode = decl->type();
		auto identifier = decl->identifier();
		typeNode->accept(this);
		auto iden = identifier->getToken();
		auto type = typeNode->type;
		log("added {}: {}\n", iden.tok, type->toString());
		table.addSymbol(iden.tok, createValue(type), type);
	}
	void Sema::visit(AST::DeclarationList*list)	{
		for (auto i : *list) {
			i->accept(this);
		}
	}
	void Sema::visit(AST::Literal*)
	{
	}
	void Sema::visit(AST::BinaryExpression*)
	{
	}
	void Sema::visit(AST::UnaryExpression*)
	{
	}
	void Sema::pre(AST::AST*ast)
	{
	//	log("visiting {}\b", ast->str());
	}
	void Sema::visit(AST::Enum*)
	{
	}
	
	void Sema::visit(AST::PostfixExpr*)
	{
	}
	void Sema::visit(AST::FuncArgType*)
	{
	}

	
}