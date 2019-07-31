#include "compile.h"
#include "type.hpp"


int main() {
	try {
		kcc::Compiler compiler;
		compiler.compileFile(R"(C:\Users\xiaoc\source\repos\shiinamiyuki\kcc\test.c)");
		return 0;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}