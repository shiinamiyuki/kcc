#include "type.hpp"
#include <array>
#include <mutex>

namespace kcc {
    namespace Type {
        bool equal(IType *a, IType *b);

        const std::array<PrimitiveType *, ETotalPrimitiveType> &getPrimitiveTypes() {
            static std::array<PrimitiveType *, ETotalPrimitiveType> arr;
            static std::once_flag flag;
            std::call_once(flag, [&]() {
                for (int i = 0; i < ETotalPrimitiveType; i++) {
                    arr[i] = new PrimitiveType(EPrimitiveType(i));
                }

            });
            return arr;
        }

        static std::set<std::string> intArithmeticOps = {
                "+", "-", "*", "/", "%", "<<", ">>", "&", "|", "^"
        };
        static std::set<std::string> floatArithmeticOps = {
                "+", "-", "*", "/",
        };
        static std::set<std::string> logicArithmeticOps = {
                "&&", "||",
        };
        static std::set<std::string> comparisonOps = {
                "==", "!=", ">=", "<=", "<", ">"
        };
        static std::set<std::string> assignOps = {
                "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "|=", "^="
        };


        PrimitiveType *floatPromote(PrimitiveType *left, PrimitiveType *right) {
            AssertThrow(left->isFloat() && right->isFloat());
            if (left == right)return left;
            if (left->type() == EDouble) {
                return left;
            }
            if (right->type() == EDouble) {
                return right;
            }
            return left;
        }

        PrimitiveType *intPromote(PrimitiveType *left, PrimitiveType *right) {
            AssertThrow(left->isInt() && right->isInt());
            if (left == right)return left;
            if (left->type() < right->type()) {
                return right;
            } else {
                return left;
            }
            return left;
        }

        PointerType *checkBinaryExpr(PointerType *left, PrimitiveType *right, const std::string &op) {
            if (op == "+" || op == "-" || op == "+=" || op == "-=") {
                if (right->isInt()) {
                    return left;
                }
            }
            return nullptr;
        }

        PointerType *checkBinaryExpr(PrimitiveType *left, PointerType *right, const std::string &op) {
            if (op == "+") {
                if (left->isInt()) {
                    return right;
                }
            }
            return nullptr;
        }

        ArrayType *checkBinaryExpr(ArrayType *left, PrimitiveType *right, const std::string &op) {
            if (op == "+" || op == "-") {
                if (right->isInt()) {
                    return left;
                }
            }
            return nullptr;
        }

        ArrayType *checkBinaryExpr(PrimitiveType *left, ArrayType *right, const std::string &op) {
            if (op == "+") {
                if (left->isInt()) {
                    return right;
                }
            }
            return nullptr;
        }

        IType *checkBinaryExpr(PointerType *left, PointerType *right, const std::string &op) {
            if (op == "-") {
                return getPrimitiveTypes()[EULongLong];
            }
            if (left->baseType()->isPrimitive() && cast<PrimitiveType *>(left->baseType())->type() == EVoid) {
                return left;
            }
            return equal(left, right) ? left : nullptr;
        }

        PrimitiveType *checkBinaryExpr(PrimitiveType *left, PrimitiveType *right, const std::string &op) {
            if (left->isFloat() && right->isFloat()) {
                if (floatArithmeticOps.find(op) != floatArithmeticOps.end()) {
                    return floatPromote(left, right);
                }
                auto opRemovedAssign = op.substr(0, op.size() - 1);
                if (floatArithmeticOps.find(opRemovedAssign) != floatArithmeticOps.end()) {
                    return floatPromote(left, right);
                }
                return nullptr;
            }
            if (left->isInt() && right->isInt()) {
                if (intArithmeticOps.find(op) != intArithmeticOps.end()) {
                    return intPromote(left, right);
                }
                if (comparisonOps.find(op) != comparisonOps.end()) {
                    return intPromote(left, right);
                }
                auto opRemovedAssign = op.substr(0, op.size() - 1);
                if (intArithmeticOps.find(opRemovedAssign) != intArithmeticOps.end()) {
                    return intPromote(left, right);
                }
                return nullptr;
            }
            // now we have both int and float
            if (comparisonOps.find(op) != comparisonOps.end()) {
                return getPrimitiveTypes()[EInt];
            }
            if (floatArithmeticOps.find(op) != floatArithmeticOps.end()) {
                if (left->isFloat()) {
                    return left;
                }
                AssertThrow(right->isFloat());
                return right;
            }
            return nullptr;
        }

        IType *checkBinaryExpr(IType *left, IType *right, const std::string &op) {
            if (op == "=") {
                if (left->isArray()) {
                    return nullptr;
                }
                if (convertible(right, left))
                    return left;
                return nullptr;
            }
            if (left->isStruct()) {
                return nullptr;
            }
            if (right->isStruct()) {
                return nullptr;
            }
            if (left->isArray() && right->isPrimitive()) {
                return checkBinaryExpr(cast<ArrayType *>(left), cast<PrimitiveType *>(right), op);
            }
            if (left->isPrimitive() && right->isArray()) {
                return checkBinaryExpr(cast<PrimitiveType *>(left), cast<ArrayType *>(right), op);
            }
            if (left->isPrimitive() && right->isPrimitive()) {
                return checkBinaryExpr(cast<PrimitiveType *>(left), cast<PrimitiveType *>(right), op);
            }
            if (left->isPrimitive() && right->isPointer()) {
                return checkBinaryExpr(cast<PrimitiveType *>(left), cast<PointerType *>(right), op);
            }
            if (left->isPointer() && right->isPrimitive()) {
                return checkBinaryExpr(cast<PointerType *>(left), cast<PrimitiveType *>(right), op);
            }
            if (left->isPointer() && right->isPointer()) {
                return checkBinaryExpr(cast<PointerType *>(left), cast<PointerType *>(right), op);
            }
            return nullptr;
        }

        bool equal(IType *a, IType *b) {
            if (a == b)return true;
            if (a->isPrimitive() && b->isPrimitive()) {
                return cast<PrimitiveType *>(a)->type() == cast<PrimitiveType *>(b)->type();
            }
            if (a->isPointer() && b->isInt()) {
                return false;
            }
            if (a->isInt() && b->isPointer()) {
                return false;
            }
            if (a->isPointer() && b->isPointer()) {
                return equal(cast<PointerType *>(a)->baseType(), cast<PointerType *>(b)->baseType());
            }
            if (a->isStruct()) {
                if (!b->isStruct()) {
                    return false;
                }
                return cast<StructType *>(a)->name == cast<StructType *>(b)->name;
            }
            if (a->isArray()) {
                if (!b->isArray()) {
                    return false;
                }
                auto arrA = cast<ArrayType *>(a);
                auto arrB = cast<ArrayType *>(b);
                return equal(arrA->baseType(), arrB->baseType()) && arrA->length() == arrB->length();
            }
            debug("uncovered case {} {}\n", a->toString(), b->toString());
            return false;
        }

        bool convertible(IType *from, IType *to) {
            if (from == to) {
                return true;
            }
            if (from->isPrimitive() && to->isPrimitive()) {
                return true;
            }
            if (from->isStruct() || to->isStruct()) {
                if (from != to) {
                    return false;
                }
            }
            if (from->isArray() || to->isArray()) {
                return equal(from, to);
            }
            if (from->isPointer() && to->isInt()) {
                return true;
            }
            if (from->isInt() && to->isPointer()) {
                return true;
            }
            if (from->isPointer() && to->isPointer()) {
                if (equal(from, to)) {
                    return true;
                }
                return convertible(cast<PointerType *>(from)->baseType(), cast<PointerType *>(to)->baseType());
            }
            if (from->isPointer() && to->isFloat()) {
                return false;
            }
            if (from->isFloat() && to->isPointer()) {
                return false;
            }
            if (from->isStruct()) {
                if (!to->isStruct()) {
                    return false;
                }
                return cast<StructType *>(from)->name == cast<StructType *>(to)->name;
            }
            debug("uncovered case {} {}\n", from->toString(), to->toString());
            return false;
        }

        bool canCast(IType *from, IType *to) {
            if (from->isPrimitive() && to->isPrimitive()) {
                return convertible(from, to);
            }
            if (from->isPointer() && to->isPointer()) {
                return true;
            }
            if (from->isInt() && to->isPointer()) {
                return true;
            }
            if (from->isPointer() && to->isInt()) {
                return true;
            }
            return false;
        }

        std::string PrimitiveType::toString() const {
            switch (type()) {
                case EVoid:
                    return "void";
                case EChar:
                    return "char";
                case EUChar:
                    return "unsigned char";
                case EShort:
                    return "unsigned short";
                case EUShort:
                    return "unsigned short";
                case EInt:
                    return "int";
                case EUInt:
                    return "unsigned int";
                case ELong:
                    return "long";
                case EULong:
                    return "unsigned long";
                case ELongLong:
                    return "long long";
                case EULongLong:
                    return "unsigned long long";
                case EFloat:
                    return "float";
                case EDouble:
                    return "double";
            }
            return "[unknown]";
        }
    }
}