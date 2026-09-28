#include "../../impl/frontend/lexer.h"
#include "../../impl/frontend/parser.h"
#include "../../impl/frontend/semantic.h"
#include "../../impl/annotation/AnnotationRuntime.hpp"
#include <cassert>
#include <iostream>

int main() {
    const std::string source =
        "@scope system\n"
        "@area network\n"
        "@responsibility transport\n"
        "@provider posix-socket\n"
        "@source impl/network/TcpTransport.cpp\n"
        "@capability network.connect\n"
        "@next system/network/multiplexing\n"
        "class AnnotationAware { void main() { print(\"ok\"); } }\n";

    sleela::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    sleela::Parser parser(std::move(tokens));
    sleela::Program program = parser.parseProgram();

    assert(program.annotations.count("scope") == 1);
    assert(program.annotations.count("next") == 1);
    assert(program.annotations.first("next")->value == "system/network/multiplexing");

    auto semantic = sleela::analyzeSemantics(program, sleela::SyntaxVersion{1,3});
    assert(semantic.ok());

    sleela::annotation::AnnotationRuntime runtime;
    std::string error;
    assert(runtime.install(program.annotations, error));
    assert(runtime.hasForwarding());
    assert(runtime.nexterColony() == "system/network/multiplexing");

    std::cout << "frontend annotation pipeline: PASS\n";
    return 0;
}
