#include "sleela/decompiler/decompiler.hpp"
#include "sleela/decompiler/errors.hpp"
#include "sleela/decompiler/output.hpp"
#include "sleela/decompiler/destination.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

namespace {
using sleela::decompiler::DecompilerError;
using sleela::decompiler::ErrorCode;

void usage() {
    std::cerr << "usage:\n"
              << "  sleela-decompiler <inspect|disassemble|cfg|functions> <file> [length] [--thumb|--arm]\n"
              << "  sleela-decompiler decompile <file> --output <java|sleela|c|c++> [--header] [--file <path>] [--to <uri>] [--content-type <t>] [length]\n"
              << "    --to accepts file:/path, ftp://[user:pass@]host[:port]/path, http://host[:port]/path (POST),\n"
              << "         http+put://host[:port]/path, or sftp://[user@]host[:port]/path (IPv4, IPv6 in [brackets], or DNS)\n";
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
            std::string destination_uri;
            std::string content_type = "text/plain";
            std::size_t length = 4096;
            bool want_header = false;
            for (int i = 3; i < argc; ++i) {
                const std::string arg = argv[i];
                if (arg == "--output") {
                    if (++i >= argc) { usage(); return 2; }
                    language = parse_language(argv[i]);
                } else if (arg == "--file") {
                    if (++i >= argc) { usage(); return 2; }
                    output_file = argv[i];
                } else if (arg == "--to") {
                    if (++i >= argc) { usage(); return 2; }
                    destination_uri = argv[i];
                } else if (arg == "--content-type") {
                    if (++i >= argc) { usage(); return 2; }
                    content_type = argv[i];
                } else if (arg == "--header") {
                    want_header = true;
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
            // Begin at the first executable section so the reconstruction describes real code.
            std::uint64_t at = 0;
            for (const auto& s : a.interfaces().sections) {
                const bool exec = (s.flags & 0x4u) != 0 || s.name == ".text" || s.name == "__text";
                if (exec && s.file_offset != 0) { at = s.file_offset; break; }
            }
            auto ins = sleela::decompiler::Decoder{}.decode(a, at, length);
            auto cfg = sleela::decompiler::Analyzer{}.build_cfg(ins);
            auto funcs = sleela::decompiler::Analyzer{}.recover_function_candidates(a, cfg);
            auto module = sleela::decompiler::Lifter{}.lift(a, ins, cfg, funcs);
            sleela::decompiler::DecompilationOptions options{language, "decompiled"};
            auto source = want_header
                ? sleela::decompiler::SourceEmitter::emit_header(a, funcs, options)
                : sleela::decompiler::SourceEmitter::emit(a, ins, cfg, funcs, module, options);
            // Destination precedence: --to (network/local adapter) overrides --file, which
            // overrides stdout. The adapter transmits already-produced output only.
            if (!destination_uri.empty()) {
                sleela::decompiler::Destination dest;
                std::string perr;
                if (!sleela::decompiler::parse_destination(destination_uri, dest, perr))
                    throw DecompilerError(ErrorCode::OutputFailure, "invalid --to destination: " + perr, 0, 0, "output");
                dest.content_type = content_type;
                const auto res = sleela::decompiler::deliver(dest, source);
                std::cerr << "destination: " << sleela::decompiler::destination_kind_name(dest.kind);
                if (!res.resolved_address.empty()) std::cerr << " (" << res.resolved_address << ")";
                std::cerr << "\n" << res.message << "\n";
                if (!res.ok)
                    throw DecompilerError(ErrorCode::OutputFailure, "delivery failed: " + res.message, 0, 0, "output");
            } else if (output_file.empty()) {
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

        if (command == "inspect") {
            std::cout << "architecture=" << static_cast<int>(a.architecture())
                      << "\nartifact_class=" << static_cast<int>(a.artifact_class()) << "\n";
            const auto& ni = a.interfaces();
            std::cout << "sections=" << ni.sections.size()
                      << " symbols=" << ni.symbols.size()
                      << " imports=" << ni.imports.size()
                      << " exports=" << ni.exports.size()
                      << " relocations=" << ni.relocations.size()
                      << " archive_members=" << ni.archive_members.size() << "\n";
            const auto& lm = a.library_metadata();
            if (!lm.soname.empty()) std::cout << "soname=" << lm.soname << "\n";
            for (const auto& n : lm.needed_libraries) std::cout << "needs=" << n << "\n";
            if (!lm.build_id.empty()) std::cout << "build_id=" << lm.build_id << "\n";
            const auto& km = a.kernel_module_metadata();
            if (!km.module_name.empty()) std::cout << "module=" << km.module_name << "\n";
            if (!km.vermagic.empty()) std::cout << "vermagic=" << km.vermagic << "\n";
            if (!km.license.empty()) std::cout << "module_license=" << km.license << "\n";
            return 0;
        }

        if (command == "disassemble" || command == "cfg" || command == "functions") {
            // Optional flags may appear alongside the positional [length]. --thumb selects
            // the ARM Thumb (T32) instruction set; the ARM default remains A32.
            std::size_t length = 64;
            auto mode = sleela::decompiler::DecodeMode::Auto;
            for (int k = 3; k < argc; ++k) {
                const std::string arg = argv[k];
                if (arg == "--thumb") mode = sleela::decompiler::DecodeMode::ArmThumb;
                else if (arg == "--arm") mode = sleela::decompiler::DecodeMode::ArmA32;
                else if (!arg.empty() && arg[0] != '-') length = parse_length(arg);
                else { usage(); return 2; }
            }
            // Prefer the first executable section (SHF_EXECINSTR, or a ".text" name) so the
            // linear sweep starts in real code rather than container-header bytes. Falls back
            // to file offset 0 when no executable section is identified.
            std::uint64_t at = 0;
            for (const auto& s : a.interfaces().sections) {
                const bool exec = (s.flags & 0x4u) != 0 || s.name == ".text" || s.name == "__text";
                if (exec && s.file_offset != 0) { at = s.file_offset; break; }
            }
            auto ins = sleela::decompiler::Decoder{}.decode(a, at, length, mode);
            auto cfg = sleela::decompiler::Analyzer{}.build_cfg(ins);
            if (command == "disassemble")
                for (const auto& i : ins) std::cout << std::hex << i.address << ": " << (i.text.empty() ? i.mnemonic : i.text) << "\n";
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
