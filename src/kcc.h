
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
#include "fmt/format.h"

using namespace fmt;
#define AssertThrow(x) \
    do{if(!(x)){ \
        throw std::runtime_error(fmt::format("Assertion {} failed at {} {}", #x, __FILE__, __LINE__));\
    }}while(0)



#endif /* KCC_H */
