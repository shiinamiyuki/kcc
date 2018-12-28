//
// Created by xiaoc on 2018/8/9.
//

#include "format.h"
#include <sstream>


void println(const char * fmt){
    printf("%s",fmt);
}
void println(const std::string& fmt){
    printf("%s",fmt.c_str());
}

