// ===========================================================================
// xclass_loader.h -- ingest SecureJDK 28 .xclass (XML class) files into Sleela.
//
// Parses the .xclass structural format emitted by the SecureJDK 28 `xmc`
// compiler and reconstructs an equivalent, runnable sleela::Program (classes,
// fields, typed method skeletons). See impl/xclass/XCLASS.model (XCI-0001) for
// the normative mapping. The base build needs no XML library — a small
// dependency-free scanner is used.
// ===========================================================================
#ifndef SLEELA_XCLASS_LOADER_H
#define SLEELA_XCLASS_LOADER_H

#include <string>
#include <vector>

#include "../frontend/ast.h"

namespace sleela {
namespace xclass {

// Security + provenance metadata surfaced from an .xclass (not behavior-bearing).
struct Meta {
    std::string name;
    std::string superclass;      // "(none)" or a class name
    bool isAbstract = false;
    bool isInterface = false;
    bool isPublic = true;
    int  methodCount = 0;
    int  fieldCount = 0;
    int  weight = 0;
    std::vector<std::string> interfaces;

    int  trustGrade = 0;
    int  classloadGrade = 0;

    std::string edition;
    std::string compiler;
    std::string compilerVersion;
    std::string sourceFile;
    std::string signer;
    std::string signatureAlg;
    std::string signatureHex;
    bool signed_ = false;
};

// The result of ingesting one or more .xclass files.
struct Loaded {
    Program program;             // reconstructed, runnable
    std::vector<Meta> metas;     // one per ingested class (parse order)
    std::string emitted;         // reconstructed Sleela source (for --emit)
};

// Parse one .xclass document (its raw text). Appends a ClassDecl to `prog` and
// a Meta to `metas`. Throws std::runtime_error on malformed input.
void loadDocument(const std::string& xmlText, Program& prog, std::vector<Meta>& metas);

// Convenience: read + ingest a set of .xclass files into a Loaded bundle,
// synthesizing an entry `main` shim if none of the classes defines one, and
// rendering the reconstructed Sleela source into Loaded::emitted.
Loaded loadFiles(const std::vector<std::string>& paths);

// Render a reconstructed Program back to Sleela source text (used by --emit and
// stored in Loaded::emitted).
std::string emitSource(const Program& prog);

// A one-line human summary of an ingested class (used by --info and the shim).
std::string infoLine(const Meta& m);

} // namespace xclass
} // namespace sleela

#endif // SLEELA_XCLASS_LOADER_H
