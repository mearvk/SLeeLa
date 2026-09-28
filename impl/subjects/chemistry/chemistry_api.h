#ifndef SLEELA_CHEMISTRY_API_H
#define SLEELA_CHEMISTRY_API_H

#include "ast.h"

namespace sleela {
namespace chemistry {

// Lower qualified chemistry calls into ordinary Sleela Core methods.
// The numerical API is deliberately evidence-oriented: callers supply
// normalized observations, bond/valence support, similarity, signals and
// symmetry. The 0..24 result is a model-support scale, not a truth claim.
void lowerProgram(Program& program);

} // namespace chemistry
} // namespace sleela

#endif
