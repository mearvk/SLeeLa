// ===========================================================================
// langin.h -- ingest the JVM language family (Java, Kotlin, Scala, Groovy,
//             Clojure) into Sleela as modern compiled input types.
//
// These five languages are the "brother languages" of the JVM: they all compile
// to JVM bytecode and share one runtime. SLeeLa ingests their *class skeletons*
// -- package, class/interface, modifiers, fields, and method signatures -- and
// reconstructs an equivalent, runnable sleela::Program, exactly the way the
// .xclass loader ingests SecureJDK 28 XML class files. It is a STRUCTURAL
// transpile: method bodies are synthesized skeletons (a default return), not a
// port of the source language's executable logic. From the reconstructed
// Program, SLeeLa can emit .sleela source and/or an .xclass XML descriptor.
//
// Native and dependency-free: a small hand-written scanner per language, no
// external toolchain (no javac/kotlinc/scalac) and no XML/JSON library. Builds
// on all three OSes through the same impl/ toolchain.
//
// See impl/langin/LANGIN.model (LNG-0001) for the normative mapping.
// ===========================================================================
#ifndef SLEELA_LANGIN_H
#define SLEELA_LANGIN_H

#include <string>
#include <vector>

#include "../frontend/ast.h"

namespace sleela {
namespace langin {

// The JVM sibling a source file was ingested from.
enum class Language {
    Unknown = 0,
    Java,       // .java   -- the foundation of the ecosystem
    Kotlin,     // .kt     -- pragmatic, null-safe, 100% Java-interop
    Scala,      // .scala  -- object-oriented + advanced functional
    Groovy,     // .groovy -- flexible/dynamic; drives Gradle
    Clojure,    // .clj    -- a Lisp dialect; data-as-code
};

const char* languageName(Language lang) noexcept;

// Map a file path's extension to a Language (Unknown if unrecognized).
Language languageForPath(const std::string& path);

// Provenance surfaced from one ingested source unit (not behavior-bearing).
struct Meta {
    std::string name;          // primary class/namespace name
    std::string packageName;   // package / namespace, or ""
    Language    language = Language::Unknown;
    std::string sourceFile;    // the path it came from
    bool        isInterface = false;
    bool        isAbstract = false;
    int         fieldCount = 0;
    int         methodCount = 0;
};

// The result of ingesting one or more source units.
struct Loaded {
    Program program;              // reconstructed, runnable
    std::vector<Meta> metas;      // one per ingested unit (parse order)
    std::string emitted;          // reconstructed Sleela source (for --emit-sleela)
};

// Ingest a single source unit (its raw text) of the given language. Appends a
// ClassDecl to `prog` and a Meta to `metas`. Throws std::runtime_error on input
// that yields no recognizable class/namespace.
void loadDocument(const std::string& text, Language lang, const std::string& path,
                  Program& prog, std::vector<Meta>& metas);

// Read + ingest a set of source files into a Loaded bundle, synthesizing an
// entry `main` shim if none of the units defines one, and rendering the
// reconstructed Sleela source into Loaded::emitted.
Loaded loadFiles(const std::vector<std::string>& paths);

// Render the reconstructed Program as an .xclass XML descriptor (one <xclass>
// per class), compiler="langin". Structural only, mirrors XCLASS.model shape.
std::string emitXclass(const Program& prog, const std::vector<Meta>& metas);

// A one-line human summary of an ingested unit (used by --info and the shim).
std::string infoLine(const Meta& m);

} // namespace langin
} // namespace sleela

#endif // SLEELA_LANGIN_H
