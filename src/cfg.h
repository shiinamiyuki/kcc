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

        Edge() : from(nullptr), to(nullptr) {}

        Edge(BasicBlock *_from, BasicBlock *_to) :
                from(_from), to(_to) {}

        bool empty() const { return from == nullptr || to == nullptr; }
    };

    struct BasicBlock {
        int id;
        std::vector<Phi> phi;
        std::vector<Edge> in;
        std::vector<IRNode> block;
        std::set<int> AOrig;
        std::set<int> Aphi;
        std::vector<BasicBlock *> DF, dom;
        Edge branchTrue, branchFalse;// for jmps, always take branchTrue
        BasicBlock *idom();
    };

    class CFG {
        std::vector<BasicBlock *> allBlocks;
        std::unordered_map<int, std::set<BasicBlock *>> defSite;
        std::unordered_map<int, int> count;
        std::unordered_map<int, std::stack<int>> stack;
    public:
        friend class IRGenerator;

        void addBasicBlock(BasicBlock *block) {
            block->id = (int) allBlocks.size();
            allBlocks.push_back(block);
        }

        void dump();

        void computeDominator();

        void computeDominanceFrontier();

        void findAOrig();

        void insertPhi();

        void rename();

        void rename(BasicBlock *n);

        void buildSSA();
    };
}
#endif //KCC_CFG_H
