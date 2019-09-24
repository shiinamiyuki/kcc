
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

    template<class... Args>
    inline void debug(const char * fmt, Args&&... args){
#ifndef NDEBUG
        fmt::print(fmt, args...);
#endif
    }


    inline void panic(const std::string& message) {
        std::cerr << format("Internal Compiler Error: {}", message) << std::endl;
    }
    inline size_t align8(size_t x){
        return (x + 7) & (~7);
    }
    inline size_t align16(size_t x){
        return (x + 15) & (~15);
    }

#define KCC_NOT_IMPLEMENTED()  throw std::runtime_error(fmt::format("not implemented {}:{}",__FILE__, __LINE__))

}



#endif /* KCC_H */
