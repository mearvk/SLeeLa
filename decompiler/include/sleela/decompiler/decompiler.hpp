#pragma once
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace sleela::decompiler {

enum class Format { Unknown, Raw, PE, ELF, MachO };
enum class Architecture { Unknown, X86, X86_64, ARM, ARM64 };

struct Provenance {
    std::uint64_t file_offset{};
    std::uint64_t virtual_address{};
    std::string source;
    double confidence{1.0};
};

struct Operand {
    enum class Kind { Invalid, Register, Immediate, Memory, Relative, Symbol };
    Kind kind{Kind::Invalid};
    std::string text;
    std::int64_t value{};
};

struct Instruction {
    std::uint64_t address{};
    std::vector<std::uint8_t> bytes;
    std::string mnemonic;
    std::vector<Operand> operands;
    Provenance provenance;
};

struct Section {
    std::string name;
    std::uint64_t file_offset{};
    std::uint64_t file_size{};
    std::uint64_t virtual_address{};
    std::uint64_t virtual_size{};
    std::uint32_t flags{};
};

struct Symbol {
    std::string name;
    std::uint64_t address{};
    bool external{};
    Provenance provenance;
};

struct Import { std::string library; std::string name; std::uint64_t address{}; };
struct Export { std::string name; std::uint64_t address{}; };
struct Relocation { std::uint64_t address{}; std::string type; };

class Artifact {
public:
    static Artifact open(const std::string& path);
    static Artifact from_bytes(std::span<const std::uint8_t> bytes, std::string name = {});
    Format format() const noexcept;
    Architecture architecture() const noexcept;
    std::uint64_t size() const noexcept;
    const std::vector<std::uint8_t>& bytes() const noexcept;
    const std::string& sha256() const noexcept;
    const std::string& name() const noexcept;
private:
    std::string name_;
    std::vector<std::uint8_t> bytes_;
    std::string sha256_;
    Format format_{Format::Unknown};
    Architecture architecture_{Architecture::Unknown};
};

class Decoder {
public:
    std::vector<Instruction> decode(const Artifact&, std::uint64_t address,
                                    std::size_t length) const;
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
};

} // namespace sleela::decompiler
