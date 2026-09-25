#include "sleela/decompiler/decompiler.hpp"
#include "sleela/decompiler/output.hpp"
#include <fstream>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <string>

namespace {
void usage() {
    std::cerr << "usage:\n"
              << "  sleela-decompiler <inspect|disassemble|cfg|functions> <file> [length]\n"
              << "  sleela-decompiler decompile <file> --output <java|sleela|c|c++> [--file <path>] [length]\n";
}
sleela::decompiler::SourceLanguage parse_language(const std::string& s) {
    using L = sleela::decompiler::SourceLanguage;
    if (s == "java") return L::Java;
    if (s == "sleela" || s == "sl") return L::Sleela;
    if (s == "c") return L::C;
    if (s == "c++" || s == "cpp") return L::CPlusPlus;
    throw std::invalid_argument("unsupported output language: " + s);
}
}

int main(int argc, char** argv) {
    if (argc < 3) { usage(); return 2; }
    try {
        const std::string command = argv[1];
        auto a = sleela::decompiler::Artifact::open(argv[2]);
        if (command == "decompile") {
            sleela::decompiler::SourceLanguage language = sleela::decompiler::SourceLanguage::CPlusPlus;
            std::string output_file;
            std::size_t length = 4096;
            for (int i = 3; i < argc; ++i) {
                std::string arg = argv[i];
                if (arg == "--output" && i + 1 < argc) language = parse_language(argv[++i]);
                else if (arg == "--file" && i + 1 < argc) output_file = argv[++i];
                else if (!arg.empty() && std::isdigit(static_cast<unsigned char>(arg.front()))) length = std::stoull(arg);
                else { usage(); return 2; }
            }
            auto ins = sleela::decompiler::Decoder{}.decode(a, 0, length);
            auto cfg = sleela::decompiler::Analyzer{}.build_cfg(ins);
            auto funcs = sleela::decompiler::Analyzer{}.recover_function_candidates(a, cfg);
            sleela::decompiler::DecompilationOptions options{language, "decompiled"};
            auto source = sleela::decompiler::SourceEmitter::emit(a, ins, cfg, funcs, options);
            if (output_file.empty()) std::cout << source;
            else { std::ofstream out(output_file, std::ios::binary); if (!out) throw std::runtime_error("cannot open output file: " + output_file); out << source; }
            return 0;
        }
        std::cout << "name=" << a.name() << "\nsize=" << a.size()
                  << "\nformat=" << static_cast<int>(a.format())
                  << "\nsha256=" << a.sha256() << "\n";
        if (command == "disassemble" || command == "cfg" || command == "functions") {
            auto ins = sleela::decompiler::Decoder{}.decode(a, 0, argc > 3 ? std::stoull(argv[3]) : 64);
            auto cfg = sleela::decompiler::Analyzer{}.build_cfg(ins);
            if (command == "disassemble")
                for (const auto& i : ins) std::cout << std::hex << i.address << ": " << i.mnemonic << "\n";
            else if (command == "cfg")
                for (const auto& e : cfg.edges) std::cout << e.from << " -> " << e.to << "\n";
            else
                for (const auto& s : sleela::decompiler::Analyzer{}.recover_function_candidates(a, cfg)) std::cout << s.name << " @ " << std::hex << s.address << "\n";
        }
        return 0;
    } catch (const std::exception& e) { std::cerr << e.what() << "\n"; return 1; }
}
