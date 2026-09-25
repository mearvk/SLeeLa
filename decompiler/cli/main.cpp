#include "sleela/decompiler/decompiler.hpp"
#include <iostream>
int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "usage: sleela-decompiler <inspect|disassemble|cfg|functions> <file> [length]\n";
        return 2;
    }
    try {
        auto a = sleela::decompiler::Artifact::open(argv[2]);
        std::cout << "name=" << a.name() << "\nsize=" << a.size()
                  << "\nformat=" << static_cast<int>(a.format())
                  << "\nsha256=" << a.sha256() << "\n";
        if (std::string(argv[1]) == "disassemble" || std::string(argv[1]) == "cfg" || std::string(argv[1]) == "functions") {
            auto ins = sleela::decompiler::Decoder{}.decode(a, 0, argc > 3 ? std::stoull(argv[3]) : 64);
            auto cfg = sleela::decompiler::Analyzer{}.build_cfg(ins);
            if (std::string(argv[1]) == "disassemble")
                for (const auto& i : ins) std::cout << std::hex << i.address << ": " << i.mnemonic << "\n";
            else if (std::string(argv[1]) == "cfg")
                for (const auto& e : cfg.edges) std::cout << e.from << " -> " << e.to << "\n";
            else
                for (const auto& s : sleela::decompiler::Analyzer{}.recover_function_candidates(cfg)) std::cout << s.name << " @ " << std::hex << s.address << "\n";
        }
        return 0;
    } catch (const std::exception& e) { std::cerr << e.what() << "\n"; return 1; }
}
