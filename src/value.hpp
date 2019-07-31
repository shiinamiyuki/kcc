#ifndef KCC_VALUE_H
#define KCC_VALUE_H

#include "kcc.h"

namespace kcc {
	enum ValueType {
		ENone = 0,
		EInt = 1,
		EFloat = 2,
		EReg = 8,
		EImm = 16
	};
	class Value {
		ValueType _type;
		union {
			int64_t asInt;
			double asFloat;
		};
		size_t regId;

	public:
		Value() :_type(ENone) {}
		ValueType type()const { return _type; }
		double toFloat()const {
			AssertThrow(_type & EFloat);
			AssertThrow(_type & EImm);
			return asFloat;
		}
		int64_t toInt()const {
			AssertThrow(_type & EInt);
			AssertThrow(_type & EImm); 
			return asInt;
		}
		double floatRegId()const {
			AssertThrow(_type & EFloat);
			AssertThrow(_type & EReg);
			return regId;
		}
		int64_t intRegId()const {
			AssertThrow(_type & EInt);
			AssertThrow(_type & EReg);
			return regId;
		}
		static Value CreateIntReg(int id) {
			Value v;
			v._type = ValueType(EInt | EReg);
			v.regId = id;
			return v;
		}
		static Value CreateFloatReg(int id) {
			Value v;
			v._type = ValueType(EFloat | EReg);
			v.regId = id;
			return v;
		}
		static Value CreateIntImm(int id) {
			Value v;
			v._type = ValueType(EInt | EImm);
			v.regId = id;
			return v;
		}
		static Value CreateFloatImm(int id) {
			Value v;
			v._type = ValueType(EFloat | EImm);
			v.regId = id;
			return v;
		}
	};
}

#endif