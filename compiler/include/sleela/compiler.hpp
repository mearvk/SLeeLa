#pragma once
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>
namespace sleela::compiler {
inline constexpr std::string_view version="0.1.0";
enum class OpCode:std::uint8_t{push_integer=1,load_local=2,store_local=3,add=4,subtract=5,multiply=6,divide=7,negate=8,print=9,return_value=10,halt=255};
struct Instruction{OpCode opcode{OpCode::halt};std::int64_t operand{0};};
struct Diagnostic{std::size_t line{1},column{1};std::string message;};
struct CompileResult{std::vector<Instruction> code;std::vector<std::string> local_names;std::vector<Diagnostic> diagnostics;[[nodiscard]] bool ok()const noexcept{return diagnostics.empty();}};
struct RunResult{std::int64_t return_value{0};std::vector<std::int64_t> printed_values;std::string error;[[nodiscard]] bool ok()const noexcept{return error.empty();}};
[[nodiscard]] CompileResult compile(std::string_view source);
[[nodiscard]] RunResult run(const CompileResult& program);
[[nodiscard]] std::string disassemble(const CompileResult& program);
[[nodiscard]] bool write_bytecode(const CompileResult& program,const std::filesystem::path& output,std::string& error);
[[nodiscard]] std::string read_text_file(const std::filesystem::path& path,std::string& error);
}
