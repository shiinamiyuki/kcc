//
// Created by xiaoc on 2018/9/18.
//

#ifndef KCC_CFG_H
#define KCC_CFG_H

#include "ir.h"

namespace kcc {
    class IRGenerator;
    struct BasicBlock;
    struct Edge {
        BasicBlock *from, *to;
    };
    struct BasicBlock {
        std::vector<Edge> in;
        std::vector<IRNode> block;
        Edge branchTrue, branchFalse;
    };

    class CFG {
        std::vector<BasicBlock *> allBlocks;
    public:
        CFG(const IRGenerator&); // construct CFG from IR
    };
}
#endif //KCC_CFG_H
