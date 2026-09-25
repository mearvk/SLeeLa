#include "sleela/decompiler/decompiler.hpp"
#include <iostream>

using namespace sleela::decompiler;

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        std::cerr << "usage: slecompiler-api-cfg <artifact> [length]\n";
        return 2;
    }

    try {
        const auto artifact = Artifact::open(argv[1]);
        const std::size_t length = argc == 3 ? std::stoull(argv[2]) : 128;
        const auto instructions = Decoder{}.decode(artifact, 0, length);
        const auto cfg = Analyzer{}.build_cfg(instructions);

        std::cout << "instructions: " << instructions.size() << "\n";
        for (const auto& instruction : instructions) {
            std::cout << std::hex << instruction.address << ": "
                      << instruction.mnemonic;
            for (const auto& operand : instruction.operands)
                std::cout << " " << operand.text;
            std::cout << "\n";
        }

        std::cout << "cfg blocks: " << std::dec << cfg.blocks.size()
                  << " edges: " << cfg.edges.size() << "\n";
        for (const auto& edge : cfg.edges)
            std::cout << "  " << edge.from << " -> " << edge.to << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Slecompiler error: " << e.what() << "\n";
        return 1;
    }
}
