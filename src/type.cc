#include "type.hpp"
namespace kcc {
	namespace Type {
		PrimitiveType* typeInt = new PrimitiveType("int",4);
		PrimitiveType* typeLong = new PrimitiveType("long", 4);
		PrimitiveType* typeLongLong = new PrimitiveType("long long", 8);
		PrimitiveType* typeShort = new PrimitiveType("short", 2);
		PrimitiveType* typeChar = new PrimitiveType("char", 1);
	}
}