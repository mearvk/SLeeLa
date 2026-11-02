#pragma once
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace sleela::decompiler {

enum class Format { Unknown, Raw, PE, ELF, MachO, GNUArchive };
enum class Architecture { Unknown, X86, X86_64, ARM, ARM64 };
enum class ArtifactClass { Unknown, Executable, DynamicLibrary, StaticArchive, RelocatableObject, KernelModule, Driver, RawArtifact };

struct Provenance { std::uint64_t file_offset{}; std::uint64_t virtual_address{}; std::string source; double confidence{1.0}; };
struct Operand { enum class Kind { Invalid, Register, Immediate, Memory, Relative, Symbol }; Kind kind{Kind::Invalid}; std::string text; std::int64_t value{}; };
struct Instruction {
    std::uint64_t address{};
    std::vector<std::uint8_t> bytes;
    std::string mnemonic;
    std::string text;                 // fully rendered form, e.g. "mov rbp, rsp" (operand-level recovery)
    std::vector<Operand> operands;
    Provenance provenance;
    bool is_branch{};
    bool is_conditional{};
    bool is_call{};
    bool is_return{};
    bool is_indirect{};
    bool is_tail_call{};
    bool is_relocated{};
    std::string relocation_type;
    std::string relocation_symbol;
    std::vector<std::uint64_t> branch_targets;
};
struct Section { std::string name; std::uint64_t file_offset{}; std::uint64_t file_size{}; std::uint64_t virtual_address{}; std::uint64_t virtual_size{}; std::uint32_t flags{}; };
struct ProgramSegment { std::uint32_t type{}; std::uint32_t flags{}; std::uint64_t file_offset{}; std::uint64_t virtual_address{}; std::uint64_t file_size{}; std::uint64_t memory_size{}; std::uint64_t alignment{}; };
struct Relocation { std::uint64_t address{}; std::string type; std::string symbol; std::string section{}; };
struct Symbol { std::string name; std::uint64_t address{}; bool external{}; Provenance provenance; };
struct Import { std::string library; std::string name; std::uint64_t address{}; };
struct Export { std::string name; std::uint64_t address{}; };

struct LibraryMetadata {
    std::string soname;
    std::vector<std::string> needed_libraries;
    std::vector<std::string> symbol_versions;
    std::vector<std::string> required_symbol_versions;
    std::string build_id;
    bool position_independent{};
    bool pie{};
    bool tls_present{};
    bool relro_present{};
    bool bind_now{};
    bool nx_stack{};
    std::vector<std::string> init_functions;
    std::vector<std::string> fini_functions;
    std::uint64_t plt_address{};
    std::uint64_t got_address{};
};

struct KernelModuleMetadata {
    std::string module_name;
    std::string vermagic;
    std::string license;
    std::string author;
    std::string description;
    std::vector<std::string> aliases;
    std::vector<std::string> dependencies;
};

struct ArchiveMember { std::string name; std::uint64_t header_offset{}; std::uint64_t data_offset{}; std::uint64_t size{}; Format format{Format::Unknown}; };

// ARM ELF mapping symbols ($a/$t/$d) partition executable sections into regions of A32
// code, Thumb code, and inline data. Recording them lets the decoder choose the correct
// instruction state per region instead of guessing a single state for the whole file.
enum class CodeKind { Arm, Thumb, Data };
struct CodeMapping { std::uint64_t address{}; CodeKind kind{CodeKind::Arm}; };

struct NativeInterfaces {
    std::vector<Section> sections;
    std::vector<ProgramSegment> segments;
    std::vector<ArchiveMember> archive_members;
    std::vector<Symbol> symbols;
    std::vector<Import> imports;
    std::vector<Export> exports;
    std::vector<Relocation> relocations;
    std::vector<CodeMapping> code_mappings;   // ARM $a/$t/$d, sorted by address
};

class Artifact {
public:
    static Artifact open(const std::string& path);
    static Artifact from_bytes(std::span<const std::uint8_t> bytes, std::string name = {});
    Format format() const noexcept;
    Architecture architecture() const noexcept;
    ArtifactClass artifact_class() const noexcept;
    std::uint64_t size() const noexcept;
    const std::vector<std::uint8_t>& bytes() const noexcept;
    const std::string& sha256() const noexcept;
    const std::string& name() const noexcept;
    const LibraryMetadata& library_metadata() const noexcept;
    const KernelModuleMetadata& kernel_module_metadata() const noexcept;
    const NativeInterfaces& interfaces() const noexcept;
private:
    std::string name_;
    std::vector<std::uint8_t> bytes_;
    std::string sha256_;
    Format format_{Format::Unknown};
    Architecture architecture_{Architecture::Unknown};
    ArtifactClass artifact_class_{ArtifactClass::Unknown};
    LibraryMetadata library_metadata_;
    KernelModuleMetadata kernel_module_metadata_;
    NativeInterfaces interfaces_;
};

// Decoding mode selects the instruction set for architectures that have more than one
// encoding at the same machine type. ARM, in particular, has A32 (fixed 32-bit) and
// Thumb/T32 (variable 16/32-bit) states. Auto preserves the historical default (A32 for
// ARM); callers that know the state — e.g. from an odd entry point or $t mapping symbols —
// can request ArmThumb explicitly.
enum class DecodeMode { Auto, ArmA32, ArmThumb };

class Decoder {
public:
    std::vector<Instruction> decode(const Artifact&, std::uint64_t address, std::size_t length) const;
    std::vector<Instruction> decode(const Artifact&, std::uint64_t address, std::size_t length, DecodeMode mode) const;
};

class ControlFlowGraph {
public:
    struct Edge { std::size_t from{}, to{}; };
    struct Block { std::size_t id{}; std::vector<Instruction> instructions; };
    std::vector<Block> blocks;
    std::vector<Edge> edges;
};

class Analyzer {
public:
    ControlFlowGraph build_cfg(const std::vector<Instruction>&) const;
    std::vector<Symbol> recover_function_candidates(const ControlFlowGraph&) const;
    std::vector<Symbol> recover_function_candidates(const Artifact&, const ControlFlowGraph&) const;
};

} // namespace sleela::decompiler
