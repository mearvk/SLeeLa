/*
 * lib/compiler/src/sleela_langc.cpp
 * SLeeLa Compiler — multi-language front-end C++ orchestration facade (TU).
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * The facade is header-only (sleela_langc.hpp); this translation unit exists so
 * the C++ side participates in the package build and is covered by the
 * standalone translation-unit audit. It includes the facade so the header is
 * compiled on its own, and provides nothing beyond that.
 */

#include "../include/sleela_langc.hpp"

namespace sleela {
namespace compiler {

/* Compile-time congruence check: the C++ Phase/Target enums must track the C
 * ABI codes exactly. If a code drifts, this fails the build rather than
 * silently diverging. */
static_assert(static_cast<uint32_t>(Phase::Source)   == SLEELA_LANG_PHASE_SOURCE,   "phase drift");
static_assert(static_cast<uint32_t>(Phase::Codegen)  == SLEELA_LANG_PHASE_CODEGEN,  "phase drift");
static_assert(static_cast<uint32_t>(Target::SLVM)    == SLEELA_LANG_TARGET_SLVM,    "target drift");
static_assert(static_cast<uint32_t>(Target::Native)  == SLEELA_LANG_TARGET_NATIVE,  "target drift");

} // namespace compiler
} // namespace sleela
