#ifndef SLEELA_NATIVE_API_H
#define SLEELA_NATIVE_API_H
#include "ast.h"
#include <string>
namespace sleela {
namespace native {
// Validate explicit imports and qualified module names before lowering.
// Built-in module namespaces currently reserved by the native API contract:
// math, physics, economics, excel, json, crypto, net.
void validateImports(const Program& program);
bool isModuleAvailable(const std::string& module);
}
}
#endif
