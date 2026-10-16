// ===========================================================================
// xlt_resolve.h  --  Ordered, orthogonal, congruent resolution of an XltModel.
//
// Resolution is the native counterpart of lib/xlt/SLXltCompiler: each declared
// part is checked, in document order, against a producer registry (does it
// resolve?) and against the rest of the design (is it agreeable?). The result
// is exactly one of:
//   - SUCCESS + an ordered list of what was built, or
//   - a CONGRUENCE report of why not: a missing part (unregistered ref) or a
//     part not agreeable to design (e.g. memory channels exceeding the board).
// ===========================================================================
#ifndef SLEELA_XLT_RESOLVE_H
#define SLEELA_XLT_RESOLVE_H

#include <string>
#include <vector>

#include "xlt_model.h"
#include "xlt_diagnostics.h"

namespace xlt {

struct ResolveResult {
    bool built = false;
    std::vector<std::string> builtLines;   // what was built (success path)
    std::vector<std::string> reasons;      // why not (congruence path)
};

// Resolve a parsed model. Parse-phase diagnostics in `diags` are folded into the
// congruence reasons so a single report covers lex/parse/resolve.
ResolveResult resolve(const XltModel& m, const DiagnosticBag& diags);

// Render the result in the output contract format (success list or congruence).
std::string renderResult(const std::string& modelName, const ResolveResult& r);

} // namespace xlt

#endif // SLEELA_XLT_RESOLVE_H
