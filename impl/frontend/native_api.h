#ifndef SLEELA_NATIVE_API_H
#define SLEELA_NATIVE_API_H
#include "ast.h"
#include <string>
namespace sleela {
namespace native {
// Validate explicit imports and qualified module names before lowering.
// Built-in module namespaces reserved by the native API contract:
// math, physics, economics, excel, json, crypto, net.
void validateImports(const Program& program);
bool isModuleAvailable(const std::string& module);
// Expand executable math/physics/economics calls into ordinary Sleela Core
// methods. The resulting AST therefore persists as normal Core bytecode in a
// runnable .sleela artifact; the runtime does not need the front end.
void lowerProgram(Program& program);
}
}
#endif
