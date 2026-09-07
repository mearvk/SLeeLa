// ===========================================================================
// sst_sema.h  --  Semantic analysis over the Nordshrift component graph.
//
// Validates and orders the sheet before emission:
//   - every `uses:` target and every `call C.f()` target must exist
//   - exactly one component must define the `main` attach (the entry)
//   - `uses` edges must be acyclic; components are topologically ordered so a
//     component's dependencies are activated before it
//   - thread/repeat annotations are validated (non-negative)
// On success, Sheet.order is filled (dependency-first).
// ===========================================================================
#ifndef NORDSHRIFT_SST_SEMA_H
#define NORDSHRIFT_SST_SEMA_H

#include "sst_ast.h"

namespace nordshrift {

// Throws std::runtime_error on any semantic violation.
void analyze(Sheet& sheet);

} // namespace nordshrift

#endif // NORDSHRIFT_SST_SEMA_H
