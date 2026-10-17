#ifndef SLEELA_SEMANTIC_H
#define SLEELA_SEMANTIC_H
#include "ast.h"
#include "version.h"
#include <string>
#include <vector>
namespace sleela {
struct SemanticResult {
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    bool ok() const { return errors.empty(); }
};
// Preserve the original two-argument symbol for existing callers and objects.
SemanticResult analyzeSemantics(const Program& program, const SyntaxVersion& syntax = SyntaxVersion{1,0});
// Library/SST validation may opt out of the executable-entry-point requirement.
SemanticResult analyzeSemantics(const Program& program, const SyntaxVersion& syntax, bool requireMain);
}
#endif
