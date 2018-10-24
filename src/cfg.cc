//
// Created by xiaoc on 2018/9/18.
//

#include "cfg.h"
#include "format.h"

using namespace kcc;

void CFG::dump() {
    std::ofstream out("flow.md");
    out << "```flow\nst=>start: Start\ne=>end: End\n";
    auto getId = [](BasicBlock *i) { return i->id; };
    for (auto i:allBlocks) {
        auto id = getId(i);
        std::string s;
        s.append(format("id={}\n", id));
        if (!i->phi.empty()) {
            for(auto phi:i->phi) {
                s.append(phi.dump()).append("\n");
            }
        }
        for (const auto &stmt:i->block) {
            s.append(stmt.dump()).append("\n");
        }

        if (!i->dom.empty()) {
            s.append("dom=");
            for (const auto f : i->dom) {
                s.append(format("{} ", getId(f)));
            }
            s.append("\n");
        }
        if (!i->DF.empty()) {
            s.append("DF=");
            for (const auto f : i->DF) {
                s.append(format("{} ", getId(f)));
            }
            s.append("\n");
        }
        if (i->branchFalse.empty())
            out << format("n{}=>operation: {}\n", id, s);
        else
            out << format("n{}=>condition: {}\n", id, s);

    }
    out << format("st->n{}\n", allBlocks[0]->id);
    for (auto i:allBlocks) {
        auto id = getId(i);
        for (auto e:i->in) {
            out << format("n{}->n{}\n", getId(e.from), getId(e.to));
        }
        if (!i->branchFalse.empty()) {
            out << format("n{}(yes)->n{}\n", id, getId(i->branchTrue.to));
            out << format("n{}(no)->n{}\n", id, getId(i->branchFalse.to));
        } else {
            if (!i->branchTrue.empty()) {
                out << format("n{}->n{}\n", id, getId(i->branchTrue.to));
            }
        }
    }
    out << "```" << std::endl;
    out.close();
}

template<typename T>
std::vector<T> Intersection(const std::vector<T> &A, const std::vector<T> &B) {
    std::vector<T> result;
    for (auto i: A) {
        auto iter = std::find(B.begin(), B.end(), i);
        if (iter != B.end()) {
            result.emplace_back(i);
        }
    }
    return result;
}

template<typename T>
std::vector<T> Union(const std::vector<T> &A, const std::vector<T> &B) {
    std::vector<T> result = B;
    for (auto i: A) {
        auto iter = std::find(B.begin(), B.end(), i);
        if (iter == B.end()) {
            result.emplace_back(i);
        }
    }
    return result;
}

//see https://en.wikipedia.org/wiki/Dominator_(graph_theory)#Algorithms
void CFG::computeDominator() {
    auto n0 = allBlocks[0];
    for (auto n:allBlocks) {
        if (n != n0) {
            n->dom = allBlocks; //set all nodes as dominator
        }
    }
    auto change = true;
    while (change) {
        change = false;
        for (auto n:allBlocks) {
            if (n != n0) {
                std::vector<BasicBlock *> dom;
                dom.emplace_back(n);
                std::vector<BasicBlock *> I;
                bool f = false;
                for (auto pred:n->in) {
                    auto p = pred.from;
                    if (!f) {
                        f = true;
                        I = p->dom;
                    } else {
                        I = Intersection(I, p->dom);
                    }
                }
                dom = Union(dom, I);
                if (dom != n->dom) {
                    change = true;
                    n->dom = dom;
                }
            }
        }
    }
}

//see https://en.wikipedia.org/wiki/Static_single_assignment_form#Computing_minimal_SSA_using_dominance_frontiers
void CFG::computeDominanceFrontier() {
    for (auto b:allBlocks) {
        if (b->in.size() >= 2) {
            for (auto i:b->in) {
                auto p = i.from;
                auto runner = p;
                while (runner && runner != b->idom()) {
                    runner->DF.emplace_back(b);
                    runner = b->idom();
                }
            }
        }
    }
}

BasicBlock *BasicBlock::idom() {
    for (auto i: dom) {
        if (!i->branchFalse.empty()) {
            if (i->branchFalse.to == this)
                return i;
        }
        if (!i->branchTrue.empty()) {
            if (i->branchTrue.to == this)
                return i;
        }
    }
    return nullptr;
}

void CFG::findAOrig() {
    for (auto i:allBlocks) {
        for (auto stmt:i->block) {
            if (stmt.op == Opcode::store) {
                i->AOrig.insert(stmt.a);
            } else if (stmt.op == Opcode::load) {
                i->AOrig.insert(stmt.b);
            }
        }
    }
}

void CFG::insertPhi() {
    for (auto n: allBlocks) {
        for (auto a:n->AOrig) {
            if (defSite.find(a) == defSite.end()) {
                defSite[a] = {n};
            } else {
                defSite[a].insert(n);
            }
        }
    }
    for (auto var:defSite) {
        auto &a = var.first;
        std::set<BasicBlock *> W = defSite[a];
        while (!W.empty()) {
            auto n = *W.begin();
            W.erase(W.begin());
            for (auto Y:n->DF) {
                if (Y->Aphi.empty() || Y->Aphi.find(a) == Y->Aphi.end()) {
                    //insert phi nodes
                    Y->phi.emplace_back(Phi(a,Y->in.size()));
                    Y->Aphi.insert(a);
                    if (n->AOrig.empty() || n->AOrig.find(a) == n->AOrig.end()) {
                        W.insert(Y);
                    }
                }
            }
        }
    }
}

void CFG::buildSSA() {
    computeDominator();
    computeDominanceFrontier();
    findAOrig();
    insertPhi();
    rename();
}

void CFG::rename() {
    for(auto& var:defSite){
        auto a = var.first;
        count[a] = 0;
        stack[a] = {};
        stack[a].push(0);
    }
    rename(allBlocks[0]);
}

void CFG::rename(BasicBlock *n) {
    if(n->renamed)return;
    n->renamed = true;
    int i;
    for(auto& S:n->phi){
        auto a = S.result.addr;
        count[a]++;
        i = count[a];
        stack[a].push(i);
        S.result.ver = i;
    }
    for(auto& S:n->block){
        //S is not phi
        int a = -1;
        if(S.op == Opcode::load){
            i = stack[S.b].top();
            S.version = i;
            a = S.b;
        }else if(S.op == Opcode::store){
            a = S.a;
            count[a]++;
            i = count[a];
            stack[a].push(i);
            S.version = i;
        }
    }
    std::vector<BasicBlock*> succ;
    if(!n->branchTrue.empty()){
        succ.emplace_back(n->branchTrue.to);
    }
    if(!n->branchFalse.empty()){
        succ.emplace_back(n->branchFalse.to);
    }
    for(BasicBlock* Y:succ) {
        int j;
        for (j = 0; j < Y->in.size(); j++) {
            if (Y->in[j].from == n) {
                break;
            }
        }
        assert(j != Y->in.size());
        for (auto& p:Y->phi) {
            assert(j < p.param.size());
            i = stack[p.param[j].addr].top();
            p.param[j].ver = i;
        }
    }
    for(auto X:succ){
        rename(X);
    }

    for(auto& S:n->phi){
        auto a = S.result.addr;
        stack[a].pop();
    }
    for(auto& S:n->block){
        //S is not phi
        int a = -1;
        if(S.op == Opcode::store){
            a = S.a;
            stack[a].pop();
        }
    }
}
