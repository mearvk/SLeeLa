// ===========================================================================
// artifact.h -- Sleelvac compiler artifact boundary.
// ===========================================================================
#ifndef SLEELA_ARTIFACT_H
#define SLEELA_ARTIFACT_H

#include "ast.h"
#include "version.h"
#include "../catalog/sheet_catalog.h"

namespace sleela {

// Compile the already-parsed Sleela program into a persistent .sleela
// runnable artifact. The output contains Sleela Core bytecode and can be
// loaded directly by the runtime; it is not source text requiring another
// front-end compilation pass.
int compileToArtifact(const Program& prog,
                      const std::string& outputPath,
                      const catalog::Catalog* cat = nullptr,
                      const SyntaxVersion& syntax = SyntaxVersion{1, 0});

} // namespace sleela

#endif // SLEELA_ARTIFACT_H
