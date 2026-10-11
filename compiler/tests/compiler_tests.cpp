#include "sleela/compiler.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>

int main() {
    using namespace sleela::compiler;
    int failures = 0;
    const auto check = [&](bool condition, const char* message) {
        if (!condition) {
            std::cerr << "FAIL " << message << '\n';
            ++failures;
        }
    };

    const auto program = compile("let a = 6 * 7; print a; return a + 1;");
    check(program.ok(), "compile arithmetic");
    const auto result = run(program);
    check(result.ok(), "run arithmetic");
    check(result.printed_values.size() == 1 && result.printed_values[0] == 42, "print 42");
    check(result.return_value == 43, "return 43");

    check(!compile("print unknown;").ok(), "unknown variable");
    check(!compile("let a = 1; let a = 2;").ok(), "duplicate variable");
    check(!compile("print 1").ok(), "semicolon required");
    check(!compile("print 9223372036854775808;").ok(), "positive integer literal out of range");
    check(compile("").ok(), "empty source is a valid empty program");

    const auto divisionByZero = run(compile("print 1 / 0;"));
    check(!divisionByZero.ok() && divisionByZero.error == "division by zero", "division by zero");
    const auto precedence = run(compile("print (2 + 3) * -4;"));
    check(precedence.ok() && precedence.printed_values.size() == 1 &&
          precedence.printed_values[0] == -20, "operator precedence");

    const auto addOverflow = run(compile("print 9223372036854775807 + 1;"));
    check(!addOverflow.ok() && addOverflow.error == "addition overflow", "addition overflow");
    const auto subtractOverflow = run(compile("let min = -9223372036854775807 - 1; print min - 1;"));
    check(!subtractOverflow.ok() && subtractOverflow.error == "subtraction overflow", "subtraction overflow");
    const auto negateOverflow = run(compile("let min = -9223372036854775807 - 1; print -min;"));
    check(!negateOverflow.ok() && negateOverflow.error == "negation overflow", "negation overflow");
    const auto multiplyOverflow = run(compile("print 3037000500 * 3037000500;"));
    check(!multiplyOverflow.ok() && multiplyOverflow.error == "multiplication overflow", "multiplication overflow");
    const auto divideOverflow = run(compile("let min = -9223372036854775807 - 1; print min / -1;"));
    check(!divideOverflow.ok() && divideOverflow.error == "division overflow", "division overflow");

    CompileResult invalidOpcode;
    invalidOpcode.code.push_back({static_cast<OpCode>(42), 0});
    check(run(invalidOpcode).error == "invalid opcode", "reject invalid opcode");
    CompileResult invalidLocal;
    invalidLocal.code.push_back({OpCode::load_local, 0});
    check(run(invalidLocal).error == "invalid local slot", "reject invalid local slot");
    CompileResult stackUnderflow;
    stackUnderflow.code.push_back({OpCode::add, 0});
    check(run(stackUnderflow).error == "stack underflow", "reject stack underflow");
    CompileResult stackOverflow;
    stackOverflow.code.assign(65537, {OpCode::push_integer, 1});
    check(run(stackOverflow).error == "stack overflow", "reject stack overflow");

    CompileResult minLiteral;
    minLiteral.code.push_back({OpCode::push_integer, std::numeric_limits<std::int64_t>::min()});
    minLiteral.code.push_back({OpCode::negate, 0});
    minLiteral.code.push_back({OpCode::halt, 0});

    const auto tempRoot = std::filesystem::temp_directory_path() / "sleela-native-compiler-tests";
    std::error_code fsError;
    std::filesystem::remove_all(tempRoot, fsError);
    fsError.clear();
    std::filesystem::create_directories(tempRoot, fsError);
    check(!fsError, "create temporary test directory");
    if (!fsError) {
        const auto nativePath = tempRoot / "generated-test.c";
        std::string nativeError;
        const auto nativeProgram = compile("let answer = 6 * 7; print answer; return answer;");
        check(write_native_source(nativeProgram, nativePath, false, nativeError), "emit C source");

        std::ifstream nativeFile(nativePath);
        const std::string nativeText((std::istreambuf_iterator<char>(nativeFile)), {});
        check(nativeText.find("int main(void)") != std::string::npos, "generated C has entry point");
        check(nativeText.find("case 9:") != std::string::npos, "generated C has print opcode");
        check(nativeText.find("pc>=SL_PROGRAM_COUNT") != std::string::npos,
              "generated C bounds-checks program counter");
        check(nativeText.find("stack overflow") != std::string::npos,
              "generated C diagnoses stack overflow");

        check(!write_native_source(compile("print missing;"), nativePath, false, nativeError),
              "refuse native emission after failed compilation");
        nativeFile.close();
        std::ifstream preservedFile(nativePath);
        const std::string preservedText((std::istreambuf_iterator<char>(preservedFile)), {});
        check(preservedText == nativeText, "failed emission preserves existing output");
        preservedFile.close();

        check(write_native_source(minLiteral, tempRoot / "minimum.c", false, nativeError),
              "emit minimum signed integer");
        std::ifstream minimumFile(tempRoot / "minimum.c");
        const std::string minimumText((std::istreambuf_iterator<char>(minimumFile)), {});
        check(minimumText.find("INT64_MIN") != std::string::npos,
              "minimum signed integer emitted portably");

        const auto impossiblePath = tempRoot / "missing-parent" / "output.c";
        check(!write_native_source(nativeProgram, impossiblePath, false, nativeError),
              "report native output path failure");
        check(!write_bytecode(nativeProgram, impossiblePath, nativeError),
              "report bytecode output path failure");
        check(!write_bytecode(compile("print missing;"), nativePath, nativeError),
              "refuse bytecode emission after failed compilation");
    }
    std::filesystem::remove_all(tempRoot, fsError);

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return EXIT_FAILURE;
    }
    std::cout << "All native compiler tests passed\n";
    return EXIT_SUCCESS;
}
