#ifndef SLEELA_NATIVE_ADD_H
#define SLEELA_NATIVE_ADD_H
// -----------------------------------------------------------------------------
// Internal declarations for the per-subject native class builders.
//
// Each function appends one synthesized native class (__NativeMath,
// __NativePhysics, __NativeEconomics) to the program. The definitions live in
// the individual subject translation units:
//   subjects/math/math_native.cpp        -> addMath
//   subjects/physics/physics_native.cpp  -> addPhysics
//   subjects/economics/economics_native.cpp -> addEconomics
// The dispatcher in native_api.cpp calls them based on the program's imports.
// -----------------------------------------------------------------------------
#include "native_api.h"

namespace sleela { namespace native {

void addMath(Program& p);
void addPhysics(Program& p);
void addEconomics(Program& p);
void addInference(Program& p);
void addSociology(Program& p);

}} // namespace sleela::native

#endif // SLEELA_NATIVE_ADD_H
