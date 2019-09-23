#include "compile.h"
#include "type.hpp"


int main(int argc,char** argv) {
//    try {
        kcc::Compiler compiler;
        compiler.compileFile(argv[1]);
        return 0;
//    }
//    catch (std::exception &e) {
//        std::cerr << e.what() << std::endl;
//    } catch (...) {
//        std::cerr << "wtf" << std::endl;
//    }
}