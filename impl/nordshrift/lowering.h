// ===========================================================================
// lowering.h -- Shared Sleela AST -> Nordshrift intermediate representation.
// ===========================================================================
// Nordshrift 2.2-dev introduces a target-neutral lowering boundary. The
// resulting IR is deliberately small and deterministic: it records program
// structure and operations without selecting Java, Sleela, or C syntax.
// ===========================================================================
#ifndef NORDSHRIFT_LOWERING_H
#define NORDSHRIFT_LOWERING_H

#include <string>
#include <vector>
#include "../frontend/ast.h"

namespace nordshrift {

struct LoweredProgram {
    std::vector<std::string> ops;
    std::string error;

    bool ok() const { return error.empty(); }
};

// Lower the complete Sleela AST into the shared, target-neutral IR.
LoweredProgram lowerProgram(const sleela::Program& program);

// Stable serialization used for diagnostics/tests and future cache keys.
std::string serializeLoweredProgram(const LoweredProgram& lowered);

} // namespace nordshrift

#endif // NORDSHRIFT_LOWERING_H
