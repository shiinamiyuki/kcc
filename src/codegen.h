
#ifndef KCC_CODEGEN_H
#define KCC_CODEGEN_H


#include "visitor.h"
#include "type.hpp"
#include "registers.h"
#include "ir.h"

namespace kcc {

    struct FunctionCode {
        std::string name;
        size_t reservedBytes = 0;
        size_t localVarSize = 0;
        size_t exprStackSize = 0;
        size_t bytesAllocated = 0;
        std::string functionEndLabel;
        std::set<Register> usedPreservedRegisters;
        std::vector<std::string> code;

        std::string generateFunctionCode();
    };

    inline std::string getSuffix(size_t size) {
        std::string suffix;
        if (size == 8) {
            suffix = "q";
        } else if (size == 4) {
            suffix = "l";
        } else if (size == 2) {
            suffix = "w";
        } else if (size == 1) {
            suffix = "b";
        } else {
            KCC_NOT_IMPLEMENTED();
        }
        return suffix;
    }

    class CodeGenerator : public AST::Visitor {
        class LValueEvaluator : public AST::Visitor {
        public:
            std::vector<std::string> stack;
            CodeGenerator &codeGenerator;

            LValueEvaluator(CodeGenerator &codeGenerator) : codeGenerator(codeGenerator) {}

            void visit(AST::Identifier *identifier) override;

            void visit(AST::TernaryExpression *expression) override;

            void visit(AST::CallExpression *expression) override;

            void visit(AST::CastExpression *expression) override;

            void visit(AST::MemberAccessExpression *expression) override;

            void visit(AST::IndexExpression *expression) override;

            void visit(AST::BinaryExpression *expression) override;

            void visit(AST::UnaryExpression *expression) override;
        };

        size_t labelCounter = 0;
        std::vector<std::string> loopStartLabelStack;
        std::vector<std::string> loopEndLabelStack;
        std::vector<int> spStack;
        std::vector<Reg> stack;
        std::vector<FunctionCode> compiledFunctions;
        std::unordered_map<std::string, std::string> literals;
        std::unordered_map<std::string, std::pair<std::string, size_t>> globals;
        FunctionCode currentFunction;
        size_t sp = 0;

        std::string genReg(const Reg &r, size_t);

        // push reg onto stack and returns its stored location
        Reg push(int addr);

        Reg pop();

        void iUnaryOp(const std::string &op, size_t size);

        // eax = eax op ebx
        void iop(const std::string &op, size_t size);

        void fop(char op);

        std::string genAddr(int i) {
            return fmt::format("-{}(%rbp)", i + currentFunction.reservedBytes);
        }

        void move(Register src, Register dst) {
            emit("mov %{}, %{}", toString(src), toString(dst));
        }

        Reg stackTopElement();

    public:
        void visit(AST::MemberAccessExpression *expression) override;

    private:
        void _push(const Reg &reg);

        void push(const std::string &opcode, const std::string &value, size_t);

        void pushRet();

        void pushAddr(int addr);

        void pushIntValue(int addr, size_t size);

        void pushLiteral(const std::string &name);

        void pushImmInt(int value);

        void pushRegister(Register);

        void store(const Reg &src, int dst, size_t size) {
            std::string opcode = fmt::format("mov{}", getSuffix(size));
            if (src.isMachineReg()) {
                emit("{} {}, {}", opcode, genReg(src, size), genAddr(dst));
            } else {
                KCC_NOT_IMPLEMENTED();
            }
        }

        void store(const Reg &src, const std::string &lvalue, size_t size) {
            std::string opcode = fmt::format("mov{}", getSuffix(size));
            if (src.isMachineReg()) {
                emit("{} {}, {}", opcode, genReg(src, size), lvalue);
            } else {
                Reg _rax{true, rax};
                emit("{} {}, {} ", opcode, genReg(src, size), genReg(_rax, size));
                emit("{} {}, {}", opcode, genReg(_rax, size), lvalue);
            }
        }

        void load(int src, const Reg &dst, size_t size) {
            std::string opcode = fmt::format("mov{}", getSuffix(size));
            if (dst.isMachineReg()) {
                emit("{} {}, {}", opcode, genAddr(src), genReg(dst, size));
            } else {
                AssertThrow(false);
            }
        }

        template<class... Args>
        void emit(const char *fmt, Args &&...args) {
            auto inst = fmt::format(fmt, args...);
            debug("emitted   {}\n", inst);
            currentFunction.code.push_back(inst);
        }

        size_t getSp() {
            return sp;
        }

        void setSp(size_t value) {
            sp = value;
            currentFunction.bytesAllocated = std::max(currentFunction.bytesAllocated, sp);
        }

    public:
        void pre(AST::AST *ast) override;

        void post(AST::AST *ast) override;

        void visit(AST::For *aFor) override;

        void visit(AST::Identifier *identifier) override;

        void visit(AST::Break *aBreak) override;

        void visit(AST::Continue *aContinue) override;

        void visit(AST::DoWhile *aWhile) override;

        void visit(AST::While *aWhile) override;

        void visit(AST::Block *block) override;

        void visit(AST::TopLevel *level) override;

        void visit(AST::If *anIf) override;

        void visit(AST::TernaryExpression *expression) override;

        void visit(AST::Number *number) override;

        void visit(AST::Return *aReturn) override;

        void visit(AST::Empty *empty) override;

        void visit(AST::PrimitiveType *type) override;

        void visit(AST::PointerType *type) override;

        void visit(AST::ArrayType *type) override;

        void visit(AST::ArgumentExepressionList *list) override;

        void visit(AST::FuncDefArg *arg) override;

        void visit(AST::FuncDef *def) override;

        void visit(AST::CallExpression *expression) override;

        void visit(AST::CastExpression *expression) override;

        void visit(AST::IndexExpression *expression) override;

        void visit(AST::Declaration *declaration) override;

        void visit(AST::DeclarationList *list) override;

        void visit(AST::Literal *literal) override;

        void visit(AST::BinaryExpression *expression) override;

        void visit(AST::UnaryExpression *expression) override;

        void visit(AST::Enum *anEnum) override;

        void visit(AST::FuncType *type) override;

        void visit(AST::PostfixExpr *expr) override;

        void visit(AST::FuncArgType *type) override;

        std::string generateAsm();

    };
}

#endif //KCC_CODEGEN_H
