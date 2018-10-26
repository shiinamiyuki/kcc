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
        std::set<BasicBlock*> children;
        BasicBlock * parent; // idom
        Edge branchTrue, branchFalse;// for jmps, always take branchTrue
        BasicBlock *idom();
        bool renamed;
        BasicBlock():renamed(false){}
        void computeIdom();
    };

    class CFG {
        std::vector<BasicBlock *> allBlocks;
        std::unordered_map<int, std::set<BasicBlock *>> defSite;
        std::unordered_map<int, int> count;
        std::unordered_map<int, std::stack<int>> stack;
    public:
        friend class IRGenerator;
        CFG(){allBlocks.emplace_back(new BasicBlock());}
        void addBasicBlock(BasicBlock *block) {
            BasicBlock * root = nullptr;
            block->id = (int) allBlocks.size();
            allBlocks.push_back(block);
            if(allBlocks.size() == 2){
                root = allBlocks[0];
                root->id = 0;
                auto edge = Edge(root,allBlocks[1]);
                root->branchTrue = edge;
                allBlocks[1]->in.emplace_back(edge);
            }
        }

        void dump();

        void computeDominator();

        void computeDominanceFrontier();
        void computeDominatorTree();
        void findAOrig();

        void insertPhi();

        void rename();

        void rename(BasicBlock *n);

        void buildSSA();
        void computeUseDef();
        void optimize();
    };
}
#endif //KCC_CFG_H
