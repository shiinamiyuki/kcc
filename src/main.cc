#include "compile.h"
#include "type.hpp"


int main() {
	try {
		kcc::Compiler compiler;
		compiler.compileFile(R"(test.c)");
		return 0;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}catch(...){
		std::cerr << "wtf" << std::endl;
	}
}