#pragma once
// ===========================================================================
// keyword_supporter.h -- supporter-document linkage for the five control-flow
// keywords that carry developer-authored semantics: try, catch, throw,
// synchronized, super.
//
// Each USE of one of these keywords in a .sleela source is given a stable,
// reproducible hash and recorded as a language annotation -- `@supporter(...)`
// -- through the existing annotation mechanism (see annotation_pipeline.cpp,
// where `supporter` is a registered, repeatable annotation name). Every hash
// maps to a relative folder under docs/keyword-supporters/<keyword>/<hash>/
// containing a SUPPORT.md stub the developer fills out to document the intended
// behavior of that specific occurrence.
//
// The hash is computed over keyword | relative-source-path | Class::method |
// ordinal-within-method, so it is deterministic across runs and does not shift
// when unrelated lines move. It does NOT depend on source line/col (the AST
// does not carry positions), which keeps a given occurrence's folder stable as
// long as the enclosing method and the order of its keyword uses are unchanged.
// ===========================================================================
#include "ast.h"
#include <string>
#include <vector>

namespace sleela::frontend {

// One record per keyword occurrence found in a program.
struct KeywordSupporterRecord {
    std::string keyword;        // "try" | "catch" | "throw" | "synchronized" | "super"
    std::string hash;           // short, stable hex digest identifying this occurrence
    std::string enclosingClass; // owning class name (or "<file>" if outside any method)
    std::string enclosingMethod;// owning method name (or "<init>"/"<file>")
    int ordinal = 0;            // 0-based index of this keyword within the method
    std::string relSourcePath;  // repo-relative source path the occurrence came from
};

// The five keywords this facility tracks. Exposed so tooling/tests can share it.
const std::vector<std::string>& supporterKeywords() noexcept;

// Walk a parsed program and return one record per try/catch/throw/synchronized/
// super occurrence. `relSourcePath` is recorded in each record and folded into
// the hash; pass the repository-relative path of the source being compiled.
std::vector<KeywordSupporterRecord>
collectKeywordSupporters(const Program& prog, const std::string& relSourcePath);

// Record each occurrence as a `@supporter("<keyword>:<hash>")` language
// annotation on the program's document annotations, using the real annotation
// model. Safe to call once per compile; duplicates are not added.
void attachSupporterAnnotations(Program& prog,
                                const std::vector<KeywordSupporterRecord>& records);

// Create the relative supporter folders and a SUPPORT.md stub for each record
// under `<docsRoot>/keyword-supporters/<keyword>/<hash>/`, plus a top-level
// index. Existing SUPPORT.md files are NEVER overwritten (developer content is
// preserved); only missing folders/stubs are created. Returns true on success.
bool materializeSupporterFolders(const std::vector<KeywordSupporterRecord>& records,
                                 const std::string& docsRoot);

} // namespace sleela::frontend
