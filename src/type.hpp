#ifndef KCC_TYPE_H
#define KCC_TYPE_H

#include "kcc.h"
namespace kcc {
	namespace Type {
		class Qualifier {

		};

		class IType {
		public:
			virtual bool isFloat()const = 0;

			virtual bool isInt()const = 0;

			virtual bool isPrimitive()const = 0;
			virtual bool isStruct()const = 0;
			virtual std::string toString()const = 0;
			bool isPointer()const { return !isPrimitive() && !isStruct(); }

		};
		class PrimitiveType;
		class PrimitiveType : public IType {
			Qualifier _qualifier;
			std::string _name;
			size_t _size;
		public:
			PrimitiveType(const std::string& name,
				size_t size,
				const Qualifier& qualifier = Qualifier())
				:_name(name), _size(size), _qualifier(qualifier) {}

			const std::string& name()const { return _name; }

			bool isFloat()const {
				return _name == "float" || _name == "double";
			}

			bool isInt()const {
				return _name == "int" ||
					_name == "long" ||
					_name == "long long" ||
					_name == "short" ||
					_name == "char";
			}

			bool isPrimitive()const {
				return isFloat() || isInt();
			}
			bool isStruct()const {
				return false;
			}
			std::string toString()const {
				return _name;
			}
		};

		class PointerType : public IType {
			IType* _baseType;
		public:
			PointerType(IType* base) :_baseType(base) {}
			bool isFloat()const {
				return false;
			}

			bool isInt()const {
				return false;
			}

			bool isPrimitive()const {
				return false;
			}
			bool isStruct()const {
				return false;
			}
			std::string toString()const {
				return format("{} *",_baseType->toString());
			}
		};
		class FunctionType : public IType {
		public:
			IType* ret = nullptr;
			std::vector<IType*> args;
			FunctionType(IType* ret, std::vector<IType*> args)
				:args(std::move(args)), ret(ret) {
				AssertThrow(ret);
				for (auto i : this->args) {
					AssertThrow(i);
				}
			}
			bool isFloat()const {
				return false;
			}

			bool isInt()const {
				return false;
			}

			bool isPrimitive()const {
				return false;
			}
			bool isStruct()const {
				return false;
			}
			std::string toString()const {
				std::string s = "(";
				for (auto i : args) {
					s.append(i->toString()).append(", ");
				}
				s.append(")");
				return format("{}->{}", s, ret->toString());
			}
		};
		extern PrimitiveType* typeInt;
		extern PrimitiveType* typeLong;
		extern PrimitiveType* typeLongLong;
		extern PrimitiveType* typeShort;
		extern PrimitiveType* typeChar;
	}
}

#endif