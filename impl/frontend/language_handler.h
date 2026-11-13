// ===========================================================================
// language_handler.h -- the neutral interface between the SLeeLa compiler and
// a source-language handler module.
// ===========================================================================
// The compiler (parser, semantic analysis, code generation, driver) knows
// nothing about any foreign language. A language such as Java is supported by a
// HANDLER MODULE that implements this interface and registers itself at load
// time (see frontend/java/java_handler.cpp). The compiler asks the registry
// at a small number of fixed hook points and acts on the answer; it never
// names a foreign library, marker, or naming convention itself.
//
// Removing a handler's object file from FE_SRC in impl/Makefile removes that
// language's support entirely and leaves the SLeeLa language, the VM and the
// runtime unchanged. With no handler registered every hook is a no-op:
// references resolve by exact name, there are no document markers, and a
// program that needs a handler-provided library reports it as not loaded.
//
// Spec: Java Specific Section, lib/java/JAVA.SPEC.md (handler architecture).
// Review: impl/frontend/COMPILER-JAVA-REVIEW.md.
// ===========================================================================
#ifndef SLEELA_LANGUAGE_HANDLER_H
#define SLEELA_LANGUAGE_HANDLER_H
#include "ast.h"
#include "version.h"
#include <map>
#include <string>
#include <vector>
namespace sleela {
using ClassIndexMap = std::map<std::string,const ClassDecl*>;

// A contextual Preonem document marker a handler claims (e.g. `spark;`).
struct DocumentMarker {
    std::string word;
    SyntaxVersion minSyntax;   // the marker needs #sleela >= minSyntax
};

class LanguageHandler {
public:
    virtual ~LanguageHandler() {}
    virtual const char* name() const = 0;

    // Preonem markers this handler claims; the parser records them in
    // Program::markers and semantic analysis enforces minSyntax.
    virtual std::vector<DocumentMarker> documentMarkers() const { return {}; }

    // Resolve a declared type reference against the loaded classes. Return the
    // class name it denotes, or "" for "no opinion" (the caller keeps `declared`).
    virtual std::string resolveReference(const ClassIndexMap&, const std::string&) const { return ""; }

    // Erase a declared signature type to a type the VM supports. `typeVars` is
    // the parser's stack of declared type-variable scopes. Return "" for no opinion.
    virtual std::string eraseSignatureType(const std::string&, const std::vector<std::vector<std::string>>&) const { return ""; }

    // Library class that provides instance methods on a String receiver (""
    // when the handler provides none) and the document syntax from which the
    // facility is available.
    virtual std::string stringMethodLibrary() const { return ""; }

    // Library function ("Class::method", static, two parameters: kind, message)
    // that builds the exception a VM fault throws inside a try ("" for none).
    virtual std::string faultFunction() const { return ""; }

    // Library classes the program needs loaded, given its parsed form, its
    // source text and the classes already present. Used by the driver's seeding loop.
    virtual std::vector<std::string> libraryNeeds(const Program&, const std::string& /*source*/) const { return {}; }

    // Narrow the library units that share the simple name `name`. `paths` are the
    // candidates (library-relative paths) already restricted to the preferred
    // import roots; return the subset to keep. Used for families that ship
    // several kinds of unit for one name (real sources, generated reference
    // files, per-edition trees). Default: keep all.
    virtual std::vector<std::string> filterLibraryCandidates(const std::string& /*name*/,const std::vector<std::string>& paths,const std::vector<std::string>& /*preferredRoots*/) const { return paths; }

    // Execution host: offer the arguments of `sleela run <args...>` (or a single
    // file path) to the handler. Return true when the handler ran them, with
    // the process exit status in `rc`; false leaves the arguments to the core.
    // A handler that launches an external runtime implements this in its own
    // host object so the compile-time module has no runtime dependency.
    virtual bool runHost(const std::vector<std::string>& /*args*/, int& /*rc*/) const { return false; }

    // Validate handler-owned metadata on the parsed program; throw on violation.
    virtual void validateProgram(const Program&) const {}
};

namespace lang {
// Registry (function-local static: safe during static initialization).
void registerHandler(LanguageHandler* h);
const std::vector<LanguageHandler*>& handlers();

// Self-registration helper: a handler defines one static Registrar.
struct Registrar { explicit Registrar(LanguageHandler* h) { registerHandler(h); } };

// Dispatch helpers: ask every handler in registration order.
std::vector<DocumentMarker> documentMarkers();
bool isDocumentMarker(const std::string& word);
const DocumentMarker* findDocumentMarker(const std::string& word);
// Resolve a reference; returns `declared` unchanged when no handler claims it.
std::string resolveReference(const ClassIndexMap& byName, const std::string& declared);
std::string eraseSignatureType(const std::string& source, const std::vector<std::vector<std::string>>& typeVars);
std::string stringMethodLibrary();
std::string faultFunction();
std::vector<std::string> libraryNeeds(const Program&, const std::string& source);
void validateProgram(const Program&);
std::vector<std::string> filterLibraryCandidates(const std::string& name,const std::vector<std::string>& paths,const std::vector<std::string>& preferredRoots);
// Offer `run` arguments to each handler host; true when one ran them.
bool runHost(const std::vector<std::string>& args,int& rc);
}
}
#endif
