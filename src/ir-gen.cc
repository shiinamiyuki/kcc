//
// Created by xiaoc on 2018/9/17.
//

#include "ir-gen.h"

using namespace kcc;

void kcc::IRGenerator::visit(kcc::For *aFor) {

}

void kcc::IRGenerator::visit(kcc::Identifier *identifier) {
    emit(Opcode::load,identifier->getReg(),identifier->getAddr());
}

void kcc::IRGenerator::visit(kcc::While *aWhile) {

}

void kcc::IRGenerator::visit(kcc::Block *block) {
    visitAll(block);
}

void kcc::IRGenerator::visit(kcc::TopLevel *level) {
    visitAll(level);
}

void kcc::IRGenerator::visit(kcc::If *anIf) {

}

void kcc::IRGenerator::visit(kcc::TernaryExpression *expression) {

}

void kcc::IRGenerator::visit(kcc::Number *number) {
    if (number->isFloat)
        emit(Opcode::fconst, number->getReg(), std::stod(number->tok()));
    else
        emit(Opcode::iconst, number->getReg(), std::stoi(number->tok()));
}

void kcc::IRGenerator::visit(kcc::Return *aReturn) {

}

void kcc::IRGenerator::visit(kcc::Empty *empty) {

}

void kcc::IRGenerator::visit(kcc::PrimitiveType *type) {

}

void kcc::IRGenerator::visit(kcc::PointerType *type) {

}

void kcc::IRGenerator::visit(kcc::ArrayType *type) {

}

void kcc::IRGenerator::visit(kcc::ArgumentExepressionList *list) {

}

void kcc::IRGenerator::visit(kcc::FuncDefArg *arg) {

}

void kcc::IRGenerator::visit(kcc::FuncDef *def) {
    visitAll(def);
}

void kcc::IRGenerator::visit(kcc::CallExpression *expression) {

}

void kcc::IRGenerator::visit(kcc::CastExpression *expression) {

}

void kcc::IRGenerator::visit(kcc::IndexExpression *expression) {

}

void kcc::IRGenerator::visit(kcc::Declaration *declaration) {

}

void kcc::IRGenerator::visit(kcc::DeclarationList *list) {

}

void kcc::IRGenerator::visit(kcc::Literal *literal) {

}

void kcc::IRGenerator::visit(kcc::BinaryExpression *expression) {
    expression->rhs()->accept(this);
    expression->lhs()->accept(this);
    auto &op = expression->tok();
    if (op == "=") {
        if (expression->lhs()->isFloat && !expression->rhs()->isFloat) {
            emit(Opcode::cvti2f, expression->rhs()->getReg(), expression->rhs()->getReg());
        }
        if (!expression->lhs()->isFloat && expression->rhs()->isFloat) {
            emit(Opcode::cvtf2i, expression->rhs()->getReg(), expression->rhs()->getReg());
        }
        emit(Opcode::store, expression->lhs()->getAddr(), expression->rhs()->getReg());
    } else if (expression->lhs()->isFloat || expression->rhs()->isFloat) {
        if (!expression->lhs()->isFloat) {
            emit(Opcode::cvti2f, expression->lhs()->getReg(),
                 expression->lhs()->getReg());
        }
        if (!expression->rhs()->isFloat) {
            emit(Opcode::cvti2f, expression->rhs()->getReg(),
                 expression->rhs()->getReg());
        }
        if (op == "+") {
            emit(Opcode::fadd, expression->getReg(),
                 expression->lhs()->getReg(),
                 expression->rhs()->getReg());
        } else if (op == "-") {
            emit(Opcode::fsub, expression->getReg(),
                 expression->lhs()->getReg(),
                 expression->rhs()->getReg());
        } else if (op == "*") {
            emit(Opcode::fmul, expression->getReg(),
                 expression->lhs()->getReg(),
                 expression->rhs()->getReg());
        } else if (op == "/") {
            emit(Opcode::fdiv, expression->getReg(),
                 expression->lhs()->getReg(),
                 expression->rhs()->getReg());
        }
    } else {
        if (op == "+") {
            emit(Opcode::iadd, expression->getReg(),
                 expression->lhs()->getReg(),
                 expression->rhs()->getReg());
        } else if (op == "-") {
            emit(Opcode::isub, expression->getReg(),
                 expression->lhs()->getReg(),
                 expression->rhs()->getReg());
        } else if (op == "*") {
            emit(Opcode::imul, expression->getReg(),
                 expression->lhs()->getReg(),
                 expression->rhs()->getReg());
        } else if (op == "/") {
            emit(Opcode::idiv, expression->getReg(),
                 expression->lhs()->getReg(),
                 expression->rhs()->getReg());
        }
    }
}

void kcc::IRGenerator::visit(kcc::UnaryExpression *expression) {

}

void kcc::IRGenerator::pre(kcc::AST *ast) {

}

void kcc::IRGenerator::visit(kcc::Enum *anEnum) {

}

void kcc::IRGenerator::visit(kcc::FuncType *type) {

}

void kcc::IRGenerator::visit(kcc::PostfixExpr *expr) {

}

void kcc::IRGenerator::visit(kcc::FuncArgType *type) {

}


