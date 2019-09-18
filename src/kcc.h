
#ifndef KCC_H
#define KCC_H

#include <list>
#include <vector>
#include <stack>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <assert.h>
#include <algorithm>
#include <exception>
#include <chrono>
#include <variant>
#include <optional>
#include "fmt/format.h"

using namespace fmt;
inline void __assert(bool x,const char *expr, const char *file, int line) {
	if (!(x)) {
		throw std::runtime_error(fmt::format("Assertion {} failed at {} {}", expr, file, line)); \
	}
}
#define AssertThrow(x) \
    do{\
		__assert(x, #x, __FILE__, __LINE__);\
	}while(0)
namespace kcc {
	template<class T, class U>
	T cast(U ptr) {
		if (!ptr)return nullptr;
		auto p = dynamic_cast<T>(ptr);
		if (p)
			return p;
		throw std::runtime_error(format("Cannot cast from {}* to {}", typeid(*ptr).name(), typeid(T).name()));
	}
#ifndef NDEBUG
    template<class... Args>
    inline void debug(const char * fmt, Args&&... args){
        fmt::print(fmt, args...);
    }

#endif
    inline void panic(const std::string& message) {
        std::cerr << format("Internal Compiler Error: {}", message) << std::endl;
    }

}



#endif /* KCC_H */
