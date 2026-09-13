#ifndef SLEELA_NATIVE_API_H
#define SLEELA_NATIVE_API_H
#include "ast.h"
#include <string>
namespace sleela {
namespace native {
// Validate explicit imports and qualified module names before lowering.
// Built-in module namespaces reserved by the native API contract:
// math, physics, economics, chemistry, excel, json, crypto, net.
void validateImports(const Program& program);
bool isModuleAvailable(const std::string& module);
// Expand executable math/physics/economics calls into ordinary Sleela Core
// methods. Chemistry has an independent compiled library and a declarative
// norm model; its frontend lowering is staged separately so inferred chemistry
// remains distinguishable from measured properties.
void lowerProgram(Program& program);
}
}
#endif
