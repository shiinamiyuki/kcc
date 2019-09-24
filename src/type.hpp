#ifndef KCC_TYPE_H
#define KCC_TYPE_H

#include "kcc.h"

namespace kcc {
    namespace Type {
        class Qualifier {
        public:
            enum Type {
                Extern,
                Static,
                Const,
                Volatile
            };
            std::set<Type> q;

            bool is(Type t) {
                return q.find(t) != q.end();
            }
        };

        class IType {
        public:

            virtual size_t size() const = 0;

            virtual bool isArray() const = 0;

            virtual bool isFloat() const = 0;

            virtual bool isInt() const = 0;

            virtual bool isPrimitive() const = 0;

            virtual bool isStruct() const = 0;

            virtual bool isFunction() const = 0;

            virtual std::string toString() const = 0;

            virtual bool isPointer() const { return !isPrimitive() && !isStruct() && !isArray(); }

        };

        class PrimitiveType;

        enum EPrimitiveType {
            EVoid,
            EChar,
            EUChar,
            EShort,
            EUShort,
            EInt,
            EUInt,
            ELong,
            EULong,
            ELongLong,
            EULongLong,
            EFloat,
            EDouble,
            ETotalPrimitiveType
        };

        class PrimitiveType : public IType {
            Qualifier _qualifier;
            EPrimitiveType _type;

            friend const std::array<PrimitiveType *, ETotalPrimitiveType> &getPrimitiveTypes();

            PrimitiveType(EPrimitiveType type,
                          const Qualifier &qualifier = Qualifier())
                    : _type(type), _qualifier(qualifier) {}

        public:
            EPrimitiveType type() const { return _type; }

            bool isFloat() const {
                return type() == EFloat || type() == EDouble;
            }

            bool isInt() const {
                return EVoid < type() && type() < EFloat;
            }

            bool isPrimitive() const {
                return isFloat() || isInt();
            }

            bool isStruct() const {
                return false;
            }

            bool isFunction() const {
                return false;
            }

            bool isArray() const override {
                return false;
            }

            std::string toString() const;

            size_t size() const override {
                switch (_type) {
                    case EChar:
                    case EUChar:
                        return 1;
                    case EShort:
                    case EUShort:
                        return 2;
                    case EInt:
                    case EUInt:
                    case ELong:
                    case EULong:
                        return 4;

                    case ELongLong:
                    case EULongLong:
                        return 8;
                    default:
                        return 8;
                }
            }
        };

        class VariadicType : public IType {
        public:
            size_t size() const override {
                return 0;
            }

            bool isFloat() const override {
                return false;
            }

            bool isInt() const override {
                return false;
            }

            bool isPrimitive() const override {
                return false;
            }

            bool isStruct() const override {
                return false;
            }

            bool isFunction() const override {
                return false;
            }

            bool isArray() const override {
                return false;
            }

            std::string toString() const override {
                return "...";
            }

        public:
        };

        class PointerType : public IType {
            IType *_baseType;
        public:
            IType *baseType() const {
                return _baseType;
            };

            PointerType(IType *base) : _baseType(base) {}

            bool isFloat() const {
                return false;
            }

            bool isInt() const {
                return false;
            }

            bool isPrimitive() const {
                return false;
            }

            bool isStruct() const {
                return false;
            }

            std::string toString() const {
                return format("{} *", _baseType->toString());
            }

            bool isFunction() const {
                return false;
            }

            size_t size() const override {
                return 8;
            }

            bool isArray() const override {
                return false;
            }

        };

        class ArrayType : public IType {
            IType *_baseType;
            size_t _length;
        public:
            IType *baseType() const {
                return _baseType;
            };

            size_t length() const {
                return _length;
            };

            ArrayType(IType *base, size_t N) : _baseType(base), _length(N) {}

            bool isFloat() const override {
                return false;
            }

            bool isInt() const override {
                return false;
            }

            bool isPrimitive() const override {
                return false;
            }

            bool isStruct() const override {
                return false;
            }

            std::string toString() const override {
                return format("{} [{}}", _baseType->toString(), _length);
            }

            bool isFunction() const override {
                return false;
            }

            size_t size() const override {
                return _baseType->size() * _length;
            }

            bool isArray() const override {
                return true;
            }

        };

        class FunctionType : public IType {
        public:
            IType *ret = nullptr;
            std::vector<IType *> args;

            FunctionType(IType *ret, std::vector<IType *> args)
                    : args(std::move(args)), ret(ret) {
                AssertThrow(ret);
                for (auto i : this->args) {
                    AssertThrow(i);
                }
            }

            bool isFloat() const {
                return false;
            }

            bool isInt() const {
                return false;
            }

            bool isPrimitive() const {
                return false;
            }

            bool isStruct() const {
                return false;
            }

            std::string toString() const {
                std::string s = "(";
                for (auto i : args) {
                    s.append(i->toString()).append(", ");
                }
                s.append(")");
                return format("{}->{}", s, ret->toString());
            }

            bool isFunction() const {
                return true;
            }

            size_t size() const override {
                return 8;
            }

            bool isArray() const override {
                return false;
            }

        };

        struct StructMember {
            std::string name;
            size_t offset;
            IType *type = nullptr;
        };

        class StructType : public IType {
            size_t _size;
        public:
            std::string name;
            std::unordered_map<std::string, StructMember> members;

            bool contains(const std::string &s) {
                return members.find(s) != members.end();
            }

            StructMember &get(const std::string &s) {
                return members.at(s);
            }

            void add(const std::string &s, const StructMember &m) {
                members[s] = m;
            }

            StructType(size_t size = -1) : _size(size) {}

            void setSize(size_t s) { _size = s; }

            size_t size() const override {
                return _size;
            }

            bool isFloat() const override {
                return false;
            }

            bool isInt() const override {
                return false;
            }

            bool isPrimitive() const override {
                return false;
            }

            bool isStruct() const override {
                return true;
            }

            bool isFunction() const override {
                return false;
            }

            std::string toString() const override {
                return name;
            }

            bool isArray() const override {
                return false;
            }


        public:
        };

        const std::array<PrimitiveType *, ETotalPrimitiveType> &getPrimitiveTypes();

        bool convertible(IType *from, IType *to);

        IType *checkBinaryExpr(IType *left, IType *right, const std::string &op);

        bool canCast(IType *from, IType *to);
    }
}

#endif