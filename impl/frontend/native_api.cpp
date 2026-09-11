#include "native_api.h"
#include <stdexcept>
#include <set>
namespace sleela { namespace native {
bool isModuleAvailable(const std::string& module) {
    static const std::set<std::string> modules = {
        "math", "physics", "economics", "excel", "json", "crypto", "net"
    };
    return modules.count(module) != 0;
}
void validateImports(const Program& program) {
    std::set<std::string> seen;
    for (const auto& module : program.imports) {
        if (!isModuleAvailable(module))
            throw std::runtime_error("Semantic error: unknown native module '" + module + "'");
        if (!seen.insert(module).second)
            throw std::runtime_error("Semantic error: duplicate import '" + module + "'");
    }
}
}}
