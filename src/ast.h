/*
 * ast.h
 *
 *  Created on: 2018/7/6
 *      Author: xiaoc
 */

#ifndef AST_H_
#define AST_H_

#include "kcc.h"
#include "lex.h"
#include "value.hpp"
#include "registers.h"

namespace kcc {
    struct Token;


    struct SourcePos {
        int line;
        int col;
        const char *filename;

        SourcePos() = default;

        SourcePos(const char *_filename, int a, int b) {
            line = a;
            col = b;
            filename = _filename;
        }
    };
    struct Reg {
        bool isInt = true;
        int index;
        bool isMachineReg()const{
            if(isInt){
                return index <= r14;
            }
            AssertThrow(false);
        }
    };

    namespace Type {
        class IType;
    }
    namespace AST {
        class Visitor;

        class AST {
        protected:
            std::vector<AST *> children;
            Token content;
            AST *parent;

            virtual void linkRec();

        public:
            bool isFloat;
            bool isGlobal;
            unsigned int scale;


            SourcePos pos;

            AST();

            void setContent(const Token &t) {
                content = t;
            }

            virtual std::string str(int depth = 0) const;

            virtual std::string info() const;

            virtual const std::string kind() const { return std::string(); };

            inline AST *first() const {
                return children.at(0);
            }

            inline AST *second() const {
                return children.at(1);
            }

            inline AST *third() const {
                return children.at(2);
            }

            inline AST *forth() const {
                return children.at(3);
            }

            inline void add(AST *t) {
                AssertThrow(t != nullptr);
                children.push_back(t);
            }

            inline int size() const {
                return children.size();
            }

            inline const Token &getToken() const {
                return content;
            }

            inline std::vector<AST *>::reverse_iterator rbegin() {
                return children.rbegin();
            }

            inline std::vector<AST *>::reverse_iterator rend() {
                return children.rend();
            }

            inline std::vector<AST *>::iterator begin() {
                return children.begin();
            }

            inline std::vector<AST *>::iterator end() {
                return children.end();
            }

            void set(int i, AST *ast) {
                children[i] = ast;
            }

            AST *get(int i) {
                return children.at(i);
            }

            virtual ~AST();

            virtual void accept(Visitor *vis);

            virtual void link();

            AST *getParent() const { return parent; }

            const std::string &tok() const { return getToken().tok; }

            std::string getPos() const {
                return format("{}:{}:{}", pos.filename, pos.line, pos.col);
            }
        };

        const char *printstr(AST *ast);

        class Expression : public AST {

            kcc::Type::IType *_type = nullptr;
            Reg _reg;
        public:
            Value value;

            Reg &reg() {
                return _reg;
            }

            const Reg &reg() const {
                return _reg;
            }

            kcc::Type::IType *type() const {
                return _type;
            }

            kcc::Type::IType *&type() {
                return _type;
            }

            std::string info() const override;

            void assignType(kcc::Type::IType *ty) { _type = ty; }
        };

        class BinaryExpression : public Expression {
        public:
            explicit BinaryExpression(const Token &t) {
                content = t;
                scale = 1;
            }

            BinaryExpression() { scale = 1; }

            const std::string kind() const override { return "BinaryExpression"; }

            void accept(Visitor *) override;

            Expression *lhs() const { return cast<Expression *>(first()); }

            Expression *rhs() const { return cast<Expression *>(second()); }
        };

        class PostfixExpr : public Expression {
        public:
            explicit PostfixExpr(const Token &t) { content = t; }

            PostfixExpr() = default;

            const std::string kind() const override { return "PostfixExpr"; }

            void accept(Visitor *) override;
        };

        class UnaryExpression : public Expression {
        public:
            explicit UnaryExpression(const Token &t) { content = t; }

            UnaryExpression() = default;

            const std::string kind() const override { return "UnaryExpression"; }

            void accept(Visitor *) override;

            Expression *expr() const { return cast<Expression *>(first()); }
        };

        class TernaryExpression : public Expression {
        public:
            TernaryExpression() = default;

            const std::string kind() const override { return "TernaryExpression"; }

            void accept(Visitor *) override;
        };

        class Identifier : public Expression {
        public:
            size_t addr;
            Identifier() = default;

            explicit Identifier(const Token &t) { content = t; }

            const std::string kind() const override { return "Identifier"; }

            void accept(Visitor *) override;

            std::string info()const override;
        };

        class Number : public Expression {
        public:
            Number() {}

            explicit Number(const Token &t) { content = t; }

            const std::string kind() const override { return "Number"; }

            void accept(Visitor *) override;

            int getInt() {
                std::istringstream in(tok());
                int i;
                in >> i;
                return i;
            }

            double getFloat() {
                std::istringstream in(tok());
                double i;
                in >> i;
                return i;
            }
        };

        class Literal : public Expression {
        public:
            explicit Literal(const Token &t) { content = t; }

            const std::string kind() const override { return "Literal"; }

            void accept(Visitor *) override;
        };

        class CastExpression : public Expression {
        public:
            CastExpression() = default;

            const std::string kind() const override { return "CastExpression"; }

            void accept(Visitor *) override;
        };

        class IndexExpression : public Expression {
        public:
            const std::string kind() const override { return "IndexExpression"; }

            void accept(Visitor *) override;
        };

        class ArgumentExepressionList;

        class CallExpression : public Expression {
        public:
            const std::string kind() const override { return "CallExpression"; }

            void accept(Visitor *) override;

            AST *callee() const { return first(); }

            ArgumentExepressionList *arg() const { return (ArgumentExepressionList *) second(); }
        };

        class ArgumentExepressionList : public AST {
        public:
            const std::string kind() const override { return "ArgumentExepressionList"; }

            void accept(Visitor *) override;
        };

        class TypeDeclaration : public AST {
        public:
            Type::IType *type = nullptr;

            virtual bool isPrimitive() const { return false; }

            virtual bool isArray() const { return false; }

            virtual bool isPointer() const { return false; }

            virtual std::string repr() const { return std::string(); }
        };

        class PrimitiveType : public TypeDeclaration {
        public:
            explicit PrimitiveType(const Token &t) { content = t; }

            const std::string kind() const override { return "PrimitiveType"; }

            void accept(Visitor *) override;

            bool isPrimitive() const override { return true; }

            std::string repr() const { return tok(); }
        };

        class PointerType : public TypeDeclaration {
        public:
            explicit PointerType() {}

            const std::string kind() const override { return "PointerType"; }

            void accept(Visitor *) override;

            bool isPointer() const override { return true; }

            TypeDeclaration *ptrTo() const { return (TypeDeclaration *) first(); }

            std::string repr() const { return ((TypeDeclaration *) first())->repr().append("*"); }
        };

        class ArrayType : public TypeDeclaration {
            int arrSize;
        public:
            explicit ArrayType(int size = -1) {
                arrSize = size;
            }

            const std::string kind() const override { return "ArrayType"; }

            std::string info() const override;

            void accept(Visitor *) override;
        };


        class While : public AST {
        public:
            const std::string kind() const override { return "While"; }

            void accept(Visitor *) override;

            Expression *cond() const { return cast<Expression*>(first()); }

            AST *body() const { return second(); }
        };

        class If : public AST {
        public:
            const std::string kind() const override { return "If"; }

            void accept(Visitor *) override;

            Expression *cond() const { return cast<Expression*>(first()); }

            AST *ifPart() const { return second(); }

            AST *elsePart() const { return third(); }
        };

        class Block : public AST {
        public:
            const std::string kind() const override { return "Block"; }

            void accept(Visitor *) override;
        };

        class TopLevel : public AST {
        public:
            const std::string kind() const override { return "TopLevel"; }

            void accept(Visitor *) override;
        };

        class DeclarationList : public AST {
        public:
            const std::string kind() const override { return "DeclarationList"; }

            void accept(Visitor *) override;
        };

        class Declaration : public AST {
        public:
            const std::string kind() const override { return "Declaration"; }

            void accept(Visitor *) override;

            TypeDeclaration *type() const { return cast<TypeDeclaration *>(first()); }

            Identifier *identifier() const { return cast<Identifier *>(second()); }

            Expression *init() const { return cast<Expression *>(third()); }

        };

        class FuncArgType;

        class FuncDefArg : public AST {
        public:

            const std::string kind() const override { return "FuncDefArg"; }

            void accept(Visitor *) override;
        };

        class FuncType;

        class FuncDef : public AST {
        public:
           size_t localAllocatedSize =0;

            const std::string kind() const override { return "FuncDef"; }

            void accept(Visitor *) override;

            FuncType *extractCallSignature();

            const std::string &name() const { return second()->tok(); }

            TypeDeclaration *ret() const {
                return cast<TypeDeclaration *>(first());
            }

            FuncDefArg *arg() const {
                return cast<FuncDefArg *>(third());
            }

            Block *block() const {
                return cast<Block *>(forth());
            }
        };

        class FuncArgType;

        class FuncArgType : public TypeDeclaration {
        public:
            explicit FuncArgType() {}

            const std::string kind() const override { return "FuncArgType"; }

            void accept(Visitor *) override;


        };

        class FuncType : public TypeDeclaration {
        public:
            explicit FuncType() {}

            const std::string kind() const override { return "FuncType"; }

            void accept(Visitor *) override;

            TypeDeclaration *ret() const { return cast<TypeDeclaration *>(first()); }

            FuncDefArg *arg() const { return cast<FuncDefArg *>(second()); }
        };

        class Return : public AST {
        public:
            const std::string kind() const override { return "Return"; }

            void accept(Visitor *) override;
        };

        class For : public AST {
        public:
            const std::string kind() const override { return "For"; }

            void accept(Visitor *) override;

            AST *init() const { return first(); }

            AST *cond() const { return second(); }

            AST *step() const { return third(); }

            AST *body() const { return forth(); }
        };

        class Empty : public AST {
        public:
            const std::string kind() const override { return "Empty"; }

            void accept(Visitor *) override;
        };

        class Enum : public AST {
        public:
            const std::string kind() const override { return "Enum"; }

            void accept(Visitor *) override;
        };
    }
}


#endif /* AST_H_ */
