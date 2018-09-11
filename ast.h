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

namespace  kcc {
    struct Token;

    class Visitor;

    struct SourcePos {
        int line;
        int col;

        SourcePos() = default;

        SourcePos(int a, int b) {
            line = a;
            col = b;
        }
    };

    class Type;

    struct Record {
        Type *type;
        int addr;
        int reg;
        bool isGlobal;
    };

    class AST {
    protected:
        std::vector<AST *> children;
        Token content;
        AST *parent;
        Record record;

        virtual void linkRec();

    public:
        SourcePos pos;

        AST();

        void setContent(const Token &t) {
            content = t;
        }

        virtual std::string str(int depth = 0) const;

        virtual std::string info() const;

        virtual const std::string type() const { return std::string(); };

        inline AST *first()const {
            return children.at(0);
        }

        inline AST *second()const {
            return children.at(1);
        }

        inline AST *third()const {
            return children.at(2);
        }

        inline AST *forth()const {
            return children.at(3);
        }

        inline void add(AST *t) {
            children.push_back(t);
        }

        inline int size()const {
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

        virtual ~AST();

        virtual void accept(Visitor *vis);

        virtual void link();

        AST *getParent() const { return parent; }

        const std::string &tok() const { return getToken().tok; }
    };

    const char *printstr(AST *ast);

    class BinaryExpression : public AST {
    public:
        explicit BinaryExpression(const Token &t) { content = t; }

        BinaryExpression() = default;

        const std::string type() const override { return "BinaryExpression"; }

        void accept(Visitor *) override;
        AST * lhs()const{return first();}
        AST * rhs()const{return second();}
    };

    class PostfixExpr : public AST {
    public:
        explicit PostfixExpr(const Token &t) { content = t; }

        PostfixExpr() = default;

        const std::string type() const override { return "PostfixExpr"; }

        void accept(Visitor *) override;
    };

    class UnaryExpression : public AST {
    public:
        explicit UnaryExpression(const Token &t) { content = t; }

        UnaryExpression() = default;

        const std::string type() const override { return "UnaryExpression"; }

        void accept(Visitor *) override;
    };

    class TernaryExpression : public AST {
    public:
        TernaryExpression() = default;

        const std::string type() const override { return "TernaryExpression"; }

        void accept(Visitor *) override;
    };

    class Identifier : public AST {
    public:
        Identifier() = default;

        explicit Identifier(const Token &t) { content = t; }

        const std::string type() const override { return "Identifier"; }

        void accept(Visitor *) override;
    };

    class Number : public AST {
    public:
        explicit Number(const Token &t) { content = t; }

        const std::string type() const override { return "Number"; }

        void accept(Visitor *) override;
    };

    class Literal : public AST {
    public:
        explicit Literal(const Token &t) { content = t; }

        const std::string type() const override { return "Literal"; }

        void accept(Visitor *) override;
    };

    class CastExpression : public AST {
    public:
        CastExpression() = default;

        const std::string type() const override { return "CastExpression"; }

        void accept(Visitor *) override;
    };

    class IndexExpression : public AST {
    public:
        const std::string type() const override { return "IndexExpression"; }

        void accept(Visitor *) override;
    };

    class CallExpression : public AST {
    public:
        const std::string type() const override { return "CallExpression"; }

        void accept(Visitor *) override;
    };

    class ArgumentExepressionList : public AST {
    public:
        const std::string type() const override { return "ArgumentExepressionList"; }

        void accept(Visitor *) override;
    };

    class Type : public AST {
    public:
        virtual bool isPrimitive() const { return false; }

        virtual bool isArray() const { return false; }

        virtual bool isPointer() const { return false; }
    };

    class PrimitiveType : public Type {
    public:
        explicit PrimitiveType(const Token &t) { content = t; }

        const std::string type() const override { return "PrimitiveType"; }

        void accept(Visitor *) override;

        bool isPrimitive() const override { return true; }
    };

    class PointerType : public Type {
    public:
        explicit PointerType() {}

        const std::string type() const override { return "PointerType"; }

        void accept(Visitor *) override;

        bool isPointer() const override { return true; }
    };

    class ArrayType : public Type {
        int arrSize;
    public:
        explicit ArrayType(int size = -1) {
            arrSize = size;
        }

        const std::string type() const override { return "ArrayType"; }

        std::string info() const override;

        void accept(Visitor *) override;
    };

    class FuncType : public Type {
    public:
        explicit FuncType() {}

        const std::string type() const override { return "FuncType"; }

        void accept(Visitor *) override;
    };

    class FuncArgType : public Type {
    public:
        explicit FuncArgType() {}

        const std::string type() const override { return "FuncArgType"; }

        void accept(Visitor *) override;
    };

    class While : public AST {
    public:
        const std::string type() const override { return "While"; }

        void accept(Visitor *) override;
    };

    class If : public AST {
    public:
        const std::string type() const override { return "If"; }

        void accept(Visitor *) override;
    };

    class Block : public AST {
    public:
        const std::string type() const override { return "Block"; }

        void accept(Visitor *) override;
    };

    class TopLevel : public AST {
    public:
        const std::string type() const override { return "TopLevel"; }

        void accept(Visitor *) override;
    };

    class DeclarationList : public AST {
    public:
        const std::string type() const override { return "DeclarationList"; }

        void accept(Visitor *) override;
    };

    class Declaration : public AST {
    public:
        const std::string type() const override { return "Declaration"; }

        void accept(Visitor *) override;
    };

    class FuncDefArg : public AST {
    public:
        const std::string type() const override { return "FuncDefArg"; }

        void accept(Visitor *) override;

        FuncArgType *extractArgType();
    };

    class FuncDef : public AST {
    public:
        const std::string type() const override { return "FuncDef"; }

        void accept(Visitor *) override;

        FuncType *extractCallSignature();
        const std::string& name()const{return second()->tok();}
        FuncDefArg * arg()const{
            return (FuncDefArg*)third();
        }
        Block * block()const{
            return (Block*)forth();
        }
    };

    class Return : public AST {
    public:
        const std::string type() const override { return "Return"; }

        void accept(Visitor *) override;
    };

    class For : public AST {
    public:
        const std::string type() const override { return "For"; }

        void accept(Visitor *) override;
    };

    class Empty : public AST {
    public:
        const std::string type() const override { return "Empty"; }

        void accept(Visitor *) override;
    };

    class Enum : public AST {
    public:
        const std::string type() const override { return "Enum"; }

        void accept(Visitor *) override;
    };
}
#endif /* AST_H_ */
