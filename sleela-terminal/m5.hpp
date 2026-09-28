#pragma once

#include "core.hpp"

#include <functional>
#include <string>

namespace sleela::sh {

// M5 integration layer. It handles syntax that sits above the M1-M4 parser:
// select loops, process substitution, and signal traps. The normal shell
// runner is supplied as a callback so M5 remains a thin L6 orchestration layer.
using M5Runner = std::function<int(const std::string&, Environment&)>;

// Run a script through the M5 layer. Returns true when the M5 layer consumed
// or transformed the script; status receives the resulting exit status.
// Scripts with no M5 syntax return false and are left to the normal pipeline.
bool runM5(const std::string& source, Environment& env,
           const M5Runner& runner, int& status);

// Returns true when a path contains process-substitution syntax: <(...) or >(...).
bool hasProcessSubstitution(const std::string& source);

} // namespace sleela::sh
