#pragma once
#include "decompiler.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace sleela::decompiler {

enum class OpCode { Nop, Const, Move, Add, Sub, Mul, Div, Load, Store, Compare, Branch, Call, Return, Phi, Trap };
struct Value { std::uint32_t id{}; std::string type{"unknown"}; };
struct Operation { OpCode opcode{OpCode::Nop}; std::vector<Value> inputs; std::vector<Value> outputs; std::int64_t immediate{}; Provenance provenance; };
struct SlirBlock { std::uint32_t id{}; std::vector<Operation> operations; std::vector<std::uint32_t> successors; };
struct Function { std::string name; std::uint64_t address{}; std::vector<SlirBlock> blocks; std::vector<Provenance> provenance; };
struct Module { std::string name; Format format{Format::Unknown}; Architecture architecture{Architecture::Unknown}; ArtifactClass artifact_class{ArtifactClass::Unknown}; LibraryMetadata library_metadata; KernelModuleMetadata kernel_module_metadata; std::vector<Function> functions; std::vector<Section> sections; };

} // namespace sleela::decompiler
