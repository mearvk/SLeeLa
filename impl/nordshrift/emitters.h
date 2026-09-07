// ===========================================================================
// emitters.h  --  The three Nordshrift backend emitters (the triple manifold).
//
// Each takes an analyzed Sheet and returns target source as a string.
//   emitJava   -> compilable, runnable Java
//   emitSleela -> .sleela source that runs on the Sleela C core
//   emitC      -> compilable, runnable C
// All three are behaviorally equivalent for the same .sst input.
// ===========================================================================
#ifndef NORDSHRIFT_EMITTERS_H
#define NORDSHRIFT_EMITTERS_H

#include <string>
#include "sst_ast.h"

namespace nordshrift {

std::string emitJava(const Sheet& sheet);
std::string emitSleela(const Sheet& sheet);
std::string emitC(const Sheet& sheet);

} // namespace nordshrift

#endif // NORDSHRIFT_EMITTERS_H
