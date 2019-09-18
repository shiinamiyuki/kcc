#ifndef KCC_TYPE_H
#define KCC_TYPE_H

#include "kcc.h"

namespace kcc {
    namespace Type {
        class Qualifier {

        };

        class IType {
        public:
            virtual bool isFloat() const = 0;

            virtual bool isInt() const = 0;

            virtual bool isPrimitive() const = 0;

            virtual bool isStruct() const = 0;

            virtual bool isFunction()const = 0;

            virtual std::string toString() const = 0;

            bool isPointer() const { return !isPrimitive() && !isStruct(); }

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
            size_t _size;

            friend const std::array<PrimitiveType *, ETotalPrimitiveType> &getPrimitiveTypes();

            PrimitiveType(EPrimitiveType type,
                          size_t size,
                          const Qualifier &qualifier = Qualifier())
                    : _type(type), _size(size), _qualifier(qualifier) {}

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
            bool isFunction()const{
                return false;
            }

            std::string toString() const;
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
            bool isFunction()const{
                return false;
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
            bool isFunction()const{
                return true;
            }
        };

        const std::array<PrimitiveType *, ETotalPrimitiveType> &getPrimitiveTypes();

        bool convertible(IType *from, IType *to);

        IType *checkBinaryExpr(IType *left, IType *right, const std::string &op);
    }
}

#endif