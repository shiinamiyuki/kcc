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
        std::vector<std::string> header, out;
        std::unordered_map<std::string, int> strConst;
        unsigned int bytesForLocals;
        int iRegLevel, fRegLevel;
        std::unordered_map<int, int> regMap;
        std::unordered_map<int, bool> regUsage;
        int getReg(int);// get reg and kill temp, since all temp regs in ir is in SSA form
        int iAlloc(int);// alloc int regsiter
        std::string fAlloc(int);// alloc float reg
        std::string getIReg(int);
        std::string getFReg(int);
        bool directMove(int a, int b);
        bool directMove(int a);
        void killReg(int i){regUsage[i] = false;}
        void markReg(int i){regUsage[i] = true;}
        void binOp(const std::string & ,int,int);
        void terOp(const std::string&,int,int,int);
        int findiRegSpillIfNone(int&);
        int findSpare();
        void move(int a, int b);
        int win_x64CalliReg(int i);
    public:
        DirectCodeGen();
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
        template<typename ...Args>
        void emitHeader(const char * s,Args... args){
            header.emplace_back(format(s,args...));
            header.emplace_back("\n");
        }
        void emitHeader(const char * s){
            header.emplace_back(s);
            header.emplace_back("\n");
        }
        void label(int i){
            emit("L{}:",i);
        }
        int addString(const std::string & s){
            if(strConst.find(s) == strConst.end()){
                strConst.insert(std::make_pair(s,strConst.size()));
                emitHeader("SC{}:\n.ascii \"{}\\0\"",strConst.size()-1,s);
            }
            return strConst[s];
        }
        void writeFile(const char * file){
            std::ofstream f(format("{}.s",file).c_str());
            for(auto& i:header){
                f << i;
            }
            for(auto& i: out){
                f << i;
            }
            f<<std::endl;
            f.close();
            system(format("gcc {}.s -o {}.exe",file,file).c_str());
        }
        void clearReg(){
            for(int i =0;i<99999;i++){
                regUsage[i] = false;
            }
        }
    };

}
#endif //KCC_X64_GEN_H
