#ifndef KCC_SEMA_H
#define KCC_SEMA_H

#include "visitor.h"
#include "type.hpp"

namespace kcc {
    struct VarInfo {
        std::string name;
        Type::IType *type = nullptr;
        size_t addr;
        bool isGlobal = false;
        bool isTypedef = false;
        bool isStruct = false;

        VarInfo() = default;

        VarInfo(const std::string &name, Type::IType *t, size_t addr)
                : name(name), type(t), addr(addr) {}
    };

    struct ScopedSymbolTable {
        std::unordered_map<std::string, VarInfo> infos;
        ScopedSymbolTable *parentScope = nullptr;
        size_t localOffset = 0;

        void addSymbol(const std::string &name, Type::IType *type) {
            AssertThrow(type);
            localOffset += type->size();
            VarInfo info(name, type, localOffset);
            if (!parentScope) {
                info.isGlobal = true;
            }
            infos[name] = info;
        }

        const VarInfo &find(const std::string &name) const {
            auto iter = infos.find(name);
            if (iter != infos.end())
                return iter->second;
            if (parentScope) {
                return parentScope->find(name);
            } else {
                throw std::runtime_error(format("No var named {}", name));
            }
        }
    };

    struct SymbolTable {
        ScopedSymbolTable *symTable;

        SymbolTable() {
            symTable = new ScopedSymbolTable();
            symTable->localOffset = 0;
        }

        void addSymbol(const std::string &name, Type::IType *type) {
            AssertThrow(type);
            symTable->addSymbol(name, type);
        }

        void addSymbol(const VarInfo &info) {
            symTable->infos[info.name] = info;
        }

        const VarInfo &find(const std::string &name) const {
            return symTable->find(name);
        }

        void pushScope() {
            auto tmp = new ScopedSymbolTable();
            tmp->parentScope = symTable;
            if (!underGlobal())
                tmp->localOffset = tmp->parentScope->localOffset;
            symTable = tmp;

        }

        void popScope() {
            auto tmp = symTable;
            symTable = symTable->parentScope;
            delete tmp;
        }

        bool underGlobal() const {
            return symTable->parentScope == nullptr;
        }
    };

    class Sema : public AST::Visitor {
        SymbolTable table;

        class LValueChecker : public AST::Visitor {
            Sema * sema;
        public:
            std::vector<int> stack;


            LValueChecker(Sema * sema):sema(sema){}

            void visit(AST::Identifier *identifier) override;

            void visit(AST::TernaryExpression *expression) override;

            void visit(AST::Number *number) override;

            void visit(AST::CallExpression *expression) override;

            void visit(AST::CastExpression *expression) override;

            void visit(AST::Literal *literal) override;

            void visit(AST::BinaryExpression *expression) override;

            void visit(AST::UnaryExpression *expression) override;

            void visit(AST::PostfixExpr *expr) override;

        };

        size_t floatRegCounter = 0;
        size_t intRegCounter = 0;
        size_t funcLocalAllocatedSize = 0;
    public:
        void visit(AST::DoWhile *aWhile) override;

    private:
        std::vector<SourcePos> sourcePosStack;

        void pushScope();

        void popScope();

    public:
        void error(const std::string &message);

        // Inherited via Visitor
        virtual void visit(AST::For *) override;

        virtual void visit(AST::Identifier *) override;

        virtual void visit(AST::While *) override;

        void visit(AST::MemberAccessExpression *expression) override;

        virtual void visit(AST::Block *) override;

        virtual void visit(AST::TopLevel *) override;

        virtual void visit(AST::If *) override;

        virtual void visit(AST::TernaryExpression *) override;

        virtual void visit(AST::Number *) override;

        virtual void visit(AST::Return *) override;

        virtual void visit(AST::Empty *) override;

        void visit(AST::StructDecl *decl) override;

        virtual void visit(AST::PrimitiveType *) override;

        virtual void visit(AST::PointerType *) override;

        virtual void visit(AST::ArrayType *) override;

        virtual void visit(AST::ArgumentExepressionList *) override;

        virtual void visit(AST::FuncDefArg *) override;

        virtual void visit(AST::FuncDef *) override;

        virtual void visit(AST::CallExpression *) override;

        virtual void visit(AST::CastExpression *) override;

        virtual void visit(AST::IndexExpression *) override;

        virtual void visit(AST::Declaration *) override;

        virtual void visit(AST::DeclarationList *) override;

        virtual void visit(AST::Literal *) override;

        virtual void visit(AST::BinaryExpression *) override;

        virtual void visit(AST::UnaryExpression *) override;

        virtual void pre(AST::AST *) override;

        virtual void post(AST::AST *) override;

        virtual void visit(AST::Enum *) override;

        virtual void visit(AST::FuncType *) override;

        virtual void visit(AST::PostfixExpr *) override;

        void visit(AST::StructType *type) override;

        virtual void visit(AST::FuncArgType *) override;
    };
}


#endif