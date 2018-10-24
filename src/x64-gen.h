//
// Created by xiaoc on 2018/10/22.
// Naive code generator
// No optimization at all
//

#ifndef KCC_X64_GEN_H
#define KCC_X64_GEN_H
#include "kcc.h"
#include "ir.h"
#include "format.h"
namespace kcc {
    class DirectCodeGen {
        std::vector<std::string> out;
    public:
        DirectCodeGen(){}
        void generateFunc(Function & func);
        void printAssembly(){for(auto& i:out)println("{}",i);}
        template<typename ...Args>
        void emit(const char * s,Args... args){
            out.emplace_back(format(s,args...));
            out.emplace_back("\n");
        }
        void emit(const char * s){
            out.emplace_back(s);
            out.emplace_back("\n");
        }
        void label(int i){
            emit("L{}:",i);
        }
    };

}
#endif //KCC_X64_GEN_H
