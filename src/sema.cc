#include "sema.h"
#include "ast.h"
#include "log.h"
#include <mutex>

namespace kcc {
	void Sema::error(const std::string& message) {
		std::cerr << message << std::endl;
	}
	void Sema::visit(AST::For*)
	{
	}
	void Sema::visit(AST::Identifier* identifier) {
		auto iden = identifier->tok();
		identifier->type() = table.find(iden).type;
	}
	void Sema::visit(AST::While* whileStmt)
	{
		whileStmt->cond()->accept(this);
		whileStmt->body()->accept(this);
	}
	void Sema::visit(AST::Block* block) {
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
	void Sema::visit(AST::If* ifStmt)
	{
		ifStmt->cond()->accept(this);
		ifStmt->ifPart()->accept(this);
		ifStmt->elsePart()->accept(this);
	}
	void Sema::visit(AST::TernaryExpression*)
	{
	}
	void Sema::visit(AST::Number* number) {
		auto num = number->tok();
		auto i = std::stoi(num);
		auto d = std::stod(num);
		if (i == d) {
			number->type() = Type::getPrimitiveTypes()[Type::EInt];
		}
		else {
			number->type() = Type::getPrimitiveTypes()[Type::EDouble];
		}
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
			primitiveTypeMap["int"] = Type::getPrimitiveTypes()[Type::EInt];
			primitiveTypeMap["char"] = Type::getPrimitiveTypes()[Type::EChar];
			primitiveTypeMap["float"] = Type::getPrimitiveTypes()[Type::EFloat];
		});
		type->type = primitiveTypeMap.at(type->repr());
	}
	void Sema::visit(AST::PointerType* type) {
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
	void Sema::visit(AST::FuncDefArg* args) {
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
	void Sema::visit(AST::FuncType* func) {
		auto ret = func->ret();
		auto args = func->arg();
		ret->accept(this);
		args->accept(this);
		func->type = convert(func);
		log("func type {}\n", func->str());
	}
	void Sema::visit(AST::FuncDef* func) {
		auto& funcname = func->name();
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
		std::string idenName = iden.tok;
		if (typeid(*decl->getParent()) == typeid(AST::FuncDefArg)) {
			// omitted args
			if (idenName.empty()) {
				idenName = std::string("##").append(std::to_string(intRegCounter));
			}
		}
		log("added {}: {}\n", idenName, type->toString());
		table.addSymbol(iden.tok, createValue(type), type);
	}
	void Sema::visit(AST::DeclarationList* list) {
		for (auto i : *list) {
			i->accept(this);
		}
	}
	void Sema::visit(AST::Literal*)
	{
	}
	void Sema::visit(AST::BinaryExpression* expr) {
		expr->rhs()->accept(this);
		expr->lhs()->accept(this);
		auto ty = Type::checkBinaryExpr(expr->lhs()->type(), expr->rhs()->type(), expr->tok());
		if (!ty) {
			error(format("cannot convert from {} to {}", 
				expr->rhs()->type()->toString(),
				expr->lhs()->type()->toString()));
		}
		expr->type() = ty;
	}
	void Sema::visit(AST::UnaryExpression*expr)	{
		expr->expr()->accept(this);
		auto ty = expr->expr()->type();
		if (expr->tok() == "*") {
			
			if (!ty->isPointer()) {
				error(format("cannot dereference type {}", ty->toString())); 
			}
			else {
				expr->type() = cast<Type::PointerType*>(ty)->baseType();
			}
		}
		else {
			if (ty->isPrimitive()) {
				expr->type() = ty;
			}
			else {
				error(format("cannot have {} on unary operator {}", ty->toString(), expr->tok()));
			}
		}
	}
	void Sema::pre(AST::AST* ast)
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