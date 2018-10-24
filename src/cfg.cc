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
        if(!i->phi.empty()){
            s.append(format("phi={}\n",i->phi.size()));
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
                    Y->phi.emplace_back(Phi(Y->in.size()));
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
}
