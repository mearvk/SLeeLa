#include "sleela/decompiler/decompiler.hpp"
#include "sleela/decompiler/errors.hpp"
#include "sleela/decompiler/output.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

namespace {
using sleela::decompiler::DecompilerError;
using sleela::decompiler::ErrorCode;

void usage() {
    std::cerr << "usage:\n"
              << "  sleela-decompiler <inspect|disassemble|cfg|functions> <file> [length]\n"
              << "  sleela-decompiler decompile <file> --output <java|sleela|c|c++> [--file <path>] [length]\n";
}

std::size_t parse_length(const std::string& text) {
    if (text.empty()) throw DecompilerError(ErrorCode::InvalidArtifact, "length cannot be empty", 0, 0, "cli");
    std::size_t pos = 0;
    unsigned long long value = 0;
    try {
        value = std::stoull(text, &pos, 0);
    } catch (const std::exception&) {
        throw DecompilerError(ErrorCode::InvalidArtifact, "invalid length: " + text, 0, 0, "cli");
    }
    if (pos != text.size() || value > std::numeric_limits<std::size_t>::max()) {
        throw DecompilerError(ErrorCode::InvalidArtifact, "invalid length: " + text, 0, 0, "cli");
    }
    return static_cast<std::size_t>(value);
}

sleela::decompiler::SourceLanguage parse_language(const std::string& s) {
    using L = sleela::decompiler::SourceLanguage;
    if (s == "java") return L::Java;
    if (s == "sleela" || s == "sl") return L::Sleela;
    if (s == "c") return L::C;
    if (s == "c++" || s == "cpp") return L::CPlusPlus;
    throw DecompilerError(ErrorCode::OutputFailure, "unsupported output language: " + s, 0, 0, "cli");
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
                const std::string arg = argv[i];
                if (arg == "--output") {
                    if (++i >= argc) { usage(); return 2; }
                    language = parse_language(argv[i]);
                } else if (arg == "--file") {
                    if (++i >= argc) { usage(); return 2; }
                    output_file = argv[i];
                } else if (arg == "--help") {
                    usage();
                    return 0;
                } else if (arg == "--version") {
                    std::cout << "Slecompiler 1.0.0\n";
                    return 0;
                } else if (arg.size() && arg[0] != '-') {
                    length = parse_length(arg);
                } else {
                    usage();
                    return 2;
                }
            }
            auto ins = sleela::decompiler::Decoder{}.decode(a, 0, length);
            auto cfg = sleela::decompiler::Analyzer{}.build_cfg(ins);
            auto funcs = sleela::decompiler::Analyzer{}.recover_function_candidates(a, cfg);
            sleela::decompiler::DecompilationOptions options{language, "decompiled"};
            auto source = sleela::decompiler::SourceEmitter::emit(a, ins, cfg, funcs, options);
            if (output_file.empty()) {
                std::cout << source;
            } else {
                std::ofstream out(output_file, std::ios::binary);
                if (!out) throw DecompilerError(ErrorCode::OutputFailure, "cannot open output file: " + output_file, 0, 0, "output");
                out << source;
                if (!out) throw DecompilerError(ErrorCode::OutputFailure, "cannot write output file: " + output_file, 0, 0, "output");
            }
            return 0;
        }

        if (command == "--help" || command == "-h") {
            usage();
            return 0;
        }
        if (command == "--version") {
            std::cout << "Slecompiler 1.0.0\n";
            return 0;
        }

        std::cout << "name=" << a.name() << "\nsize=" << a.size()
                  << "\nformat=" << static_cast<int>(a.format())
                  << "\nsha256=" << a.sha256() << "\n";

        if (command == "disassemble" || command == "cfg" || command == "functions") {
            const std::size_t length = argc > 3 ? parse_length(argv[3]) : 64;
            auto ins = sleela::decompiler::Decoder{}.decode(a, 0, length);
            auto cfg = sleela::decompiler::Analyzer{}.build_cfg(ins);
            if (command == "disassemble")
                for (const auto& i : ins) std::cout << std::hex << i.address << ": " << i.mnemonic << "\n";
            else if (command == "cfg")
                for (const auto& e : cfg.edges) std::cout << e.from << " -> " << e.to << "\n";
            else
                for (const auto& s : sleela::decompiler::Analyzer{}.recover_function_candidates(a, cfg))
                    std::cout << s.name << " @ " << std::hex << s.address << "\n";
            return 0;
        }

        usage();
        return 2;
    } catch (const DecompilerError& e) {
        std::cerr << error_code_name(e.code()) << ": " << e.what();
        if (!e.stage().empty()) std::cerr << " [stage=" << e.stage() << "]";
        std::cerr << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "InternalError: " << e.what() << "\n";
        return 1;
    }
}
