//
// Created by xiaoc on 2018/8/9.
//
// Better formatting
#ifndef KCC_PRINTLN_H
#define KCC_PRINTLN_H
#include <string>
#include <exception>
#include <sstream>
#include <type_traits>

template<typename T>
struct Formatter{
    using Ty = typename std::remove_reference<T>::type;
    const char * str(const Ty& x){return nullptr;}
};
template<typename T>
const char * printstr(T a){
    return Formatter<T>().str(a);
}

template<>
struct Formatter<const char*>{
    const char *str(const char * s) {
        return s;
    }
};
template<>
struct Formatter<int>{
    const char *str(int i) {
        char *s = new char[16];
        sprintf(s, "%d", i);
        return s;
    }
};

template<>
struct Formatter<char>{
    const char *str(char i) {
        char *s = new char[16];
        sprintf(s, "%c", i);
        return s;
    }
};
template<>
struct Formatter<std::string>{
    const char *str(const std::string& s) {
        return s.c_str();
    }
};
template<>
struct Formatter<double>{
    const char *str(double i) {
        char *s = new char[16];
        sprintf(s, "%lf", i);
        return s;
    }
};
template<>
struct Formatter<float>{
    const char *str(float i) {
        char *s = new char[16];
        sprintf(s, "%f", i);
        return s;
    }
};
template<>
struct Formatter<uint64_t >{
    const char *str(uint64_t i) {
        std::ostringstream out;
        out << i;
        return out.str().c_str();
    }
};
class BadFormatException:public std::exception{
    std::string msg;
public:
    BadFormatException(const std::string&s){msg = s;}
    const char* what()const  noexcept {return msg.c_str();}
};
template<typename S = std::string,typename T>
S format(const char*fmt){
    return std::string(fmt);
}
template<typename S = std::string,typename T>
S format( const char *fmt, T a) {
    S out;
    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] == '{' && fmt[i + 1] == '{') {
            out += fmt[i];
            i++;
        }else if (fmt[i] == '}' && fmt[i + 1] == '}') {
            out += fmt[i];
            i++;
        }
        else if (fmt[i] == '{' && fmt[i + 1] == '}') {
            const char *s = Formatter<decltype(a)>().str(a);
            out.append(s);
            i++;
        } else if (fmt[i] == '{' || fmt[i] == '}') {
            throw BadFormatException(format("stary '{}'",fmt[i]).c_str());
        }else {
            out += fmt[i];
        }

    }
    return out;
}

template<typename S = std::string,typename T, typename... Args>
S format( const char *fmt, T a, Args... args) {
    S out;
    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] == '{' && fmt[i + 1] == '}') {
            const char *s = Formatter<decltype(a)>().str(a);
            out.append(s);
            out.append(format( fmt + i + 2, args...));
            return out;
        } else {
            out += fmt[i];
        }
    }
    return out;
}
void println(const char * fmt);
void println(const std::string& fmt);


template< typename... Args>
void println(const char *fmt, Args... a) {
    auto str = format(fmt, a...);
    printf("%s",str.c_str());
    putchar('\n');
}
template< typename... Args>
void fprintln(FILE*f,const char *fmt, Args... a) {
    auto str = format(fmt, a...);
    fprintf(f,"%s",str.c_str());
    fputc('\n',f);
}
#endif
