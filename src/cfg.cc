//
// Created by xiaoc on 2018/9/18.
//

#include "cfg.h"
#include "format.h"

using namespace kcc;
void CFG::dump() {
    std::ofstream out("flow.md");
    out <<"st=>start: Start\ne=>end: End\n";
    auto getId = [](BasicBlock* i){return i->id;};
    for(auto i:allBlocks){
        auto id = getId(i);
        std::string s;
        for(const auto& stmt:i->block){
            s.append(stmt.dump()).append("\n");
        }
        if(i->branchFalse.empty())
            out << format("n{}=>operation: {}\n", id, s);
        else
            out << format("n{}=>condition: {}\n", id, s);
    }
    out <<format("st->n{}\n",allBlocks[0]->id);
    for(auto i:allBlocks){
        auto id = getId(i);
        for(auto e:i->in){
            out << format("n{}->n{}\n",getId(e.from),getId(e.to));
        }
        if(!i->branchFalse.empty()){
            out << format("n{}(yes)->n{}\n", id,getId(i->branchTrue.to));
            out << format("n{}(no)->n{}\n", id,getId(i->branchFalse.to));
        }else{
            if(!i->branchTrue.empty()){
                out << format("n{}->n{}\n", id,getId(i->branchTrue.to));
            }
        }
    }
    out.close();
}
