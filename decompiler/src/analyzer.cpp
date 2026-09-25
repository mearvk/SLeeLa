#include "sleela/decompiler/decompiler.hpp"
namespace sleela::decompiler {
ControlFlowGraph Analyzer::build_cfg(const std::vector<Instruction>& ins) const {
    ControlFlowGraph g;
    for (std::size_t n = 0; n < ins.size(); ++n)
        g.blocks.push_back({n, {ins[n]}});
    for (std::size_t n = 1; n < g.blocks.size(); ++n)
        g.edges.push_back({n - 1, n});
    return g;
}
std::vector<Symbol> Analyzer::recover_function_candidates(const ControlFlowGraph& g) const {
    std::vector<Symbol> out;
    for (const auto& b : g.blocks)
        if (!b.instructions.empty())
            out.push_back({"sub_" + std::to_string(b.instructions.front().address),
                           b.instructions.front().address, false,
                           b.instructions.front().provenance});
    return out;
}
}
