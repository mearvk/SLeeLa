#include "sleela/decompiler/decompiler.hpp"
#include <unordered_map>
#include <unordered_set>
namespace sleela::decompiler {
ControlFlowGraph Analyzer::build_cfg(const std::vector<Instruction>& ins) const {
    ControlFlowGraph g; if(ins.empty()) return g;
    std::unordered_set<std::uint64_t> starts; starts.insert(ins.front().address);
    std::unordered_map<std::uint64_t,std::size_t> by_address;
    for(std::size_t i=0;i<ins.size();++i) by_address[ins[i].address]=i;
    for(std::size_t i=0;i<ins.size();++i){
        const auto& x=ins[i];
        for(auto t:x.branch_targets) if(by_address.count(t)) starts.insert(t);
        if((x.is_branch||x.is_return||x.is_call)&&i+1<ins.size()) starts.insert(ins[i+1].address);
    }
    std::vector<std::size_t> boundaries;
    for(std::size_t i=0;i<ins.size();++i) if(starts.count(ins[i].address)) boundaries.push_back(i);
    for(std::size_t n=0;n<boundaries.size();++n){
        const auto begin=boundaries[n], end=n+1<boundaries.size()?boundaries[n+1]:ins.size();
        ControlFlowGraph::Block block; block.id=n; block.instructions.assign(ins.begin()+begin,ins.begin()+end); g.blocks.push_back(std::move(block));
    }
    std::unordered_map<std::uint64_t,std::size_t> block_by_address;
    for(const auto& b:g.blocks) if(!b.instructions.empty()) block_by_address[b.instructions.front().address]=b.id;
    for(std::size_t n=0;n<g.blocks.size();++n){
        const auto& b=g.blocks[n]; if(b.instructions.empty()) continue; const auto& last=b.instructions.back();
        for(auto t:last.branch_targets) if(block_by_address.count(t)) g.edges.push_back({n,block_by_address[t]});
        const bool stop=last.is_return||(last.is_branch&&!last.is_conditional);
        if(!stop&&n+1<g.blocks.size()) g.edges.push_back({n,n+1});
    }
    return g;
}
std::vector<Symbol> Analyzer::recover_function_candidates(const ControlFlowGraph& g) const {
    std::vector<Symbol> out; std::unordered_set<std::uint64_t> seen;
    for(const auto& b:g.blocks) if(!b.instructions.empty()){
        const auto addr=b.instructions.front().address;
        if(seen.insert(addr).second) out.push_back({"sub_"+std::to_string(addr),addr,false,b.instructions.front().provenance});
    }
    return out;
}
}
