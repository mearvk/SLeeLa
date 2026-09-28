// ===========================================================================
// artifact.h -- Sleelvac compiler artifact boundary.
// ===========================================================================
#ifndef SLEELA_ARTIFACT_H
#define SLEELA_ARTIFACT_H

#include "ast.h"
#include "version.h"
#include "../catalog/sheet_catalog.h"

namespace sleela {

// Compile the parsed Sleela program into a persistent .sleela runnable
// artifact. Native math/physics/economics modules are lowered before Core
// bytecode is emitted, so the runtime loads the resulting artifact directly.
int compileToArtifact(Program& prog,
                      const std::string& outputPath,
                      const catalog::Catalog* cat = nullptr,
                      const SyntaxVersion& syntax = SyntaxVersion{1, 0});

} // namespace sleela

#endif // SLEELA_ARTIFACT_H
