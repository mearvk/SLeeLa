#include "sleela/decompiler/output.hpp"
#include <sstream>
#include <cctype>
#include <stdexcept>

namespace sleela::decompiler {
namespace {
std::string sanitize(std::string s) {
    for (char& c : s) if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '_')) c = '_';
    if (s.empty() || std::isdigit(static_cast<unsigned char>(s.front()))) s = "_" + s;
    return s;
}
std::string emit_c_like(const Artifact& a, const std::vector<Instruction>& ins,
                        const std::vector<Symbol>& funcs, bool cpp) {
    std::ostringstream out;
    out << "/* Slecompiler generated source; evidence-derived, not guaranteed original source. */\n";
    out << "/* input: " << a.name() << " sha256: " << a.sha256() << " */\n\n";
    if (cpp) out << "#include <cstdint>\n#include <cstddef>\n\n";
    else out << "#include <stdint.h>\n#include <stddef.h>\n\n";
    for (const auto& f : funcs) {
        out << "/* recovered function: " << f.name << " @ 0x" << std::hex << f.address << std::dec << " */\n";
        out << "void " << sanitize(f.name) << "(void) {\n";
        out << "    /* Native operations are retained as evidence comments until semantic lifting is available. */\n";
        out << "    (void)0;\n";
        out << "}\n\n";
    }
    if (funcs.empty()) {
        out << "void slecompiler_decompiled(void) {\n";
        out << "    /* No function boundary was recovered for the selected analysis window. */\n";
        for (const auto& i : ins) out << "    /* 0x" << std::hex << i.address << ": " << i.mnemonic << " */\n";
        out << "}\n";
    }
    return out.str();
}

std::string emit_java(const Artifact& a, const std::vector<Instruction>& ins,
                      const std::vector<Symbol>& funcs) {
    std::ostringstream out;
    out << "// Slecompiler generated source; evidence-derived, not guaranteed original source.\n";
    out << "// input: " << a.name() << " sha256: " << a.sha256() << "\n\n";
    out << "public final class Decompiled {\n";
    out << "    private Decompiled() {}\n\n";
    for (const auto& f : funcs) {
        out << "    /** Recovered native function at 0x" << std::hex << f.address << std::dec << ". */\n";
        out << "    public static void " << sanitize(f.name) << "() {\n";
        out << "        // Native semantics remain represented as evidence until lifted.\n";
        out << "    }\n\n";
    }
    if (funcs.empty()) {
        out << "    public static void decompiled() {\n";
        out << "        // No function boundary recovered in the selected analysis window.\n";
        for (const auto& i : ins) out << "        // 0x" << std::hex << i.address << ": " << i.mnemonic << "\n";
        out << "    }\n";
    }
    out << "}\n";
    return out.str();
}

std::string emit_sleela(const Artifact& a, const std::vector<Instruction>& ins,
                        const std::vector<Symbol>& funcs) {
    std::ostringstream out;
    out << "// Slecompiler generated Sleela source.\n";
    out << "// input: " << a.name() << " sha256: " << a.sha256() << "\n\n";
    out << "module Decompiled {\n";
    for (const auto& f : funcs) {
        out << "  function " << sanitize(f.name) << "() {\n";
        out << "    // recovered native function @ 0x" << std::hex << f.address << std::dec << "\n";
        out << "    // native operations remain evidence until semantic lifting.\n";
        out << "  }\n\n";
    }
    if (funcs.empty()) {
        out << "  function decompiled() {\n";
        out << "    // no recovered function boundary in selected analysis window\n";
        for (const auto& i : ins) out << "    // 0x" << std::hex << i.address << ": " << i.mnemonic << "\n";
        out << "  }\n";
    }
    out << "}\n";
    return out.str();
}
}

std::string SourceEmitter::language_name(SourceLanguage l) {
    switch (l) {
        case SourceLanguage::Java: return "java";
        case SourceLanguage::Sleela: return "sleela";
        case SourceLanguage::C: return "c";
        case SourceLanguage::CPlusPlus: return "c++";
    }
    return "unknown";
}
std::string SourceEmitter::extension(SourceLanguage l) {
    switch (l) {
        case SourceLanguage::Java: return ".java";
        case SourceLanguage::Sleela: return ".sleela";
        case SourceLanguage::C: return ".c";
        case SourceLanguage::CPlusPlus: return ".cpp";
    }
    return ".txt";
}
std::string SourceEmitter::emit(const Artifact& a, const std::vector<Instruction>& ins,
                                const ControlFlowGraph& cfg, const std::vector<Symbol>& funcs,
                                const DecompilationOptions& options) {
    (void)cfg;
    switch (options.language) {
        case SourceLanguage::Java: return emit_java(a, ins, funcs);
        case SourceLanguage::Sleela: return emit_sleela(a, ins, funcs);
        case SourceLanguage::C: return emit_c_like(a, ins, funcs, false);
        case SourceLanguage::CPlusPlus: return emit_c_like(a, ins, funcs, true);
    }
    throw std::invalid_argument("unsupported source output language");
}
} // namespace sleela::decompiler
