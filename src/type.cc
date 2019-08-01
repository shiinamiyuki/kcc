#include "type.hpp"
#include <array>
#include <mutex>
namespace kcc {
	namespace Type {
		size_t getSize(EPrimitiveType type) {
			switch (type) {
			case EVoid:
				return 0;
			case EChar:
				return sizeof(char);
			case EUChar:
				return sizeof(unsigned char);
			case EShort:
				return sizeof(short);
			case EUShort:
				return sizeof(unsigned short);
			case EInt:
				return sizeof(int);
			case EUInt:
				return sizeof(unsigned int);
			case ELong:
				return sizeof(long);
			case EULong:
				return sizeof(unsigned long);
			case ELongLong:
				return sizeof(long long);
			case EULongLong:
				return sizeof(unsigned long long);
			case EFloat:
				return sizeof(float);
			case EDouble:
				return sizeof(double);
			}
			return -1;
		}

		const std::array<PrimitiveType*, ETotalPrimitiveType>& getPrimitiveTypes() {
			static std::array<PrimitiveType*, ETotalPrimitiveType> arr;
			static std::once_flag flag;
			std::call_once(flag, [&]() {
				for (int i = 0; i < ETotalPrimitiveType; i++) {
					arr[i] = new PrimitiveType(EPrimitiveType(i), getSize((EPrimitiveType)i));
				}
			
			});
			return arr;
		}

		static std::set<std::string> intArithmeticOps = {
				"+","-","*","/","%","<<",">>","&","|","^"
		};
		static std::set<std::string> floatArithmeticOps = {
			"+","-","*","/",
		};
		static std::set<std::string> logicArithmeticOps = {
			"&&","||",
		};
		static std::set<std::string> comparisonOps = {
			"==","!=",">=","<=","<",">"
		};
		static std::set<std::string> assignOps = {
			"+=","-=","*=","/=","%=","<<=",">>=","&=","|=","^="
		};

		IType* checkBinaryExpr(IType* left, IType* right, const std::string& op) {
			if (op == "=") {
				if (convertible(right, left))
					return left;
				return nullptr;
			}
			if (left->isPrimitive() && right->isPrimitive()) {
				return checkBinaryExpr(left, right, op);
			}
			if (left->isPrimitive() && right->isPointer()) {
				return checkBinaryExpr(left, cast<PointerType*>(right), op);
			}
			if (left->isPointer() && right->isPrimitive()) {
				return checkBinaryExpr(cast<PointerType*>(left), right, op);
			}
			if (left->isPointer() && right->isPointer()) {
				return checkBinaryExpr(cast<PointerType*>(left), cast<PointerType*>(right), op);
			}
			return nullptr;
		}
		PrimitiveType* floatPromote(PrimitiveType* left, PrimitiveType* right) {
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
		PrimitiveType* intPromote(PrimitiveType* left, PrimitiveType* right) {
			AssertThrow(left->isInt() && right->isInt());
			if (left == right)return left;
			if (left->type() < right->type()) {
				return right;
			}
			else {
				return left;
			}
			return left;
		}
		PointerType* checkBinaryExpr(PointerType* left, PrimitiveType* right, const std::string& op) {
			if (op == "+" || op == "-" || op == "+=" || op == "-=") {
				if (right->isInt()) {
					return left;
				}
			}
			return nullptr;
		}
		PointerType* checkBinaryExpr(PrimitiveType* left, PointerType* right, const std::string& op) {
			if (op == "+" ) {
				if (left->isInt()) {
					return right;
				}
			}
			return nullptr;
		}
		PrimitiveType* checkBinaryExpr(PointerType* left, PointerType* right, const std::string& op) {
			if (op == "-") {
				return getPrimitiveTypes()[EULongLong];
			}
			return nullptr;
		}
		PrimitiveType* checkBinaryExpr(PrimitiveType* left, PrimitiveType* right, const std::string& op) {
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
					return intPromote(left,right);
				}
				auto opRemovedAssign = op.substr(0, op.size() - 1);
				if (intArithmeticOps.find(opRemovedAssign) != intArithmeticOps.end()) {
					return  intPromote(left, right);
				}
				return nullptr;
			}
			// now we have both int and float
			if (left->isFloat()) {
				return checkBinaryExpr(right, right, op);
			}
			else {
				return checkBinaryExpr(left, left, op);
			}
		}

		bool convertible(IType* from, IType* to) {
			if (from->isPrimitive() && to->isPrimitive()) {
				return true;
			}
			if (from->isStruct() || to->isStruct()) {
				if (from != to) {
					return false;
				}
			}
			if (from->isPointer() && to->isInt()) {
				return true;
			}
			if (from->isInt() && to->isPointer()) {
				return true;
			}
			return false;
		}
		std::string PrimitiveType::toString()const {
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