
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


#define AssertThrow(x) \
    do{if(!(x)){ \
        throw std::runtime_error("Assertion" #x " failed at " __FILE__ __LINE__);\
    }}while(0)



#endif /* KCC_H */
