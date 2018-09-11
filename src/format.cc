//
// Created by xiaoc on 2018/8/9.
//

#include "format.h"
#include <sstream>
const char *printstr(const char * s) {
    return s;
}

const char *printstr(const std::string & s) {
    return s.c_str();
}

const char *printstr(int i) {
    char *s = new char[16];
    sprintf(s, "%d", i);
    return s;
}


const char *printstr(char i) {
    char *s = new char[16];
    sprintf(s, "%c", i);
    return s;
}


const char *printstr(double i) {
    char *s = new char[16];
    sprintf(s, "%lf", i);
    return s;
}


const char *printstr(float i) {
    char *s = new char[16];
    sprintf(s, "%f", i);
    return s;
}
void println(const char * fmt){
    printf("%s",fmt);
}
void println(const std::string& fmt){
    printf("%s",fmt.c_str());
}

const char *printstr(uint64_t i) {
    std::ostringstream out;
    out << i;
    return out.str().c_str();
}
