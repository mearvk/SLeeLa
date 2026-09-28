#include "sleela/decompiler/decompiler.hpp"
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

namespace sleela::decompiler {
ControlFlowGraph Analyzer::build_cfg(const std::vector<Instruction>& ins) const {
    ControlFlowGraph g;if(ins.empty())return g;
    std::unordered_set<std::uint64_t> starts{ins.front().address};std::unordered_map<std::uint64_t,std::size_t> by;
    for(std::size_t i=0;i<ins.size();++i)by[ins[i].address]=i;
    for(std::size_t i=0;i<ins.size();++i){const auto& x=ins[i];for(auto t:x.branch_targets)if(by.count(t))starts.insert(t);if(i+1<ins.size()&&(x.is_branch||x.is_return||x.is_call))starts.insert(ins[i+1].address);}
    std::vector<std::size_t> b;for(std::size_t i=0;i<ins.size();++i)if(starts.count(ins[i].address))b.push_back(i);
    for(std::size_t n=0;n<b.size();++n){auto begin=b[n],end=n+1<b.size()?b[n+1]:ins.size();ControlFlowGraph::Block x;x.id=n;x.instructions.assign(ins.begin()+begin,ins.begin()+end);g.blocks.push_back(std::move(x));}
    std::unordered_map<std::uint64_t,std::size_t> block;
    for(const auto& x:g.blocks)if(!x.instructions.empty())block[x.instructions.front().address]=x.id;
    for(std::size_t n=0;n<g.blocks.size();++n){const auto& x=g.blocks[n];if(x.instructions.empty())continue;const auto& last=x.instructions.back();
        for(auto t:last.branch_targets)if(block.count(t))g.edges.push_back({n,block[t]});
        const bool stop=last.is_return||(last.is_branch&&!last.is_conditional);if(!stop&&n+1<g.blocks.size())g.edges.push_back({n,n+1});
    }
    return g;
}
std::vector<Symbol> Analyzer::recover_function_candidates(const ControlFlowGraph& g) const {
    std::vector<Symbol> out;std::unordered_set<std::uint64_t> seen;
    for(const auto& b:g.blocks)if(!b.instructions.empty()){auto a=b.instructions.front().address;if(seen.insert(a).second)out.push_back({"sub_"+std::to_string(a),a,false,b.instructions.front().provenance});}
    return out;
}
std::vector<Symbol> Analyzer::recover_function_candidates(const Artifact& a,const ControlFlowGraph& g) const {
    auto out=recover_function_candidates(g);std::unordered_set<std::uint64_t> seen;for(const auto& s:out)seen.insert(s.address);
    for(const auto& s:a.interfaces().symbols)if(!s.external&&s.address&&seen.insert(s.address).second)out.push_back(s);
    for(const auto& e:a.interfaces().exports)if(e.address&&seen.insert(e.address).second)out.push_back({e.name,e.address,false,{0,e.address,"stage4-export",0.90}});
    std::sort(out.begin(),out.end(),[](const Symbol& x,const Symbol& y){return x.address<y.address;});return out;
}
}
