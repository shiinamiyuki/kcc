//
// Created by xiaoc on 2018/10/22.
//

#include "x64-gen.h"

void kcc::DirectCodeGen::generateFunc(Function &function) {
    for (int i = 0; i < function.ir.size(); i++) {
        auto &node = function.ir[i];
        switch (node.op) {
        }
    }
}
