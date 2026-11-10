#ifndef NORDSHRIFT_NATIVE_METHOD_EMIT_H
#define NORDSHRIFT_NATIVE_METHOD_EMIT_H

#include <string>
#include "../frontend/ast.h"

namespace nordshrift {
/* Emit a standalone typed-ABI plugin containing one independent C entry body
 * for each supported @native static method. Throws when any method is outside
 * the deliberately small ABI-v1 subset. */
std::string emitNativeMethods(const sleela::Program& program);
}

#endif
