// java_to_sleela.h -- Java source -> functionally equivalent SLeeLa source.
//
// The translator is a single-pass, typed, recursive-descent converter. It holds
// NO Java semantic rules of its own: operator widths, casts, promotions and
// library result types are read from the conversion subject model
// (impl/subjects/conversion/conversion.model) through conv_model.h.
//
// Contract (see .kiro/steering/java-to-sleela-conversion.md):
//   * functional equivalence is the goal, not textual similarity;
//   * anything the model does not cover FAILS CLOSED with a located diagnostic;
//     the translator never emits a near-miss and never silently drops behaviour.
#ifndef SLEELACTL_JAVA_TO_SLEELA_H
#define SLEELACTL_JAVA_TO_SLEELA_H

#include <map>
#include <memory>
#include <string>
#include <vector>

extern "C" {
#include "conv_model.h"
}

namespace javaconv {

// Static type as the converter sees it: a Java simple name plus array rank.
struct Ty {
    std::string n = "?";  // "?" unknown; "null"; "type:<name>" for a type used as a qualifier
    int dims = 0;
    bool operator==(const Ty& o) const { return n == o.n && dims == o.dims; }
    bool known() const { return n != "?"; }
};

struct MethodSig {
    std::string name;
    Ty ret;
    std::vector<Ty> params;
    bool isStatic = false;
    bool varargs = false;   // the last parameter is T... (its type is stored as T[])
    bool isAbstract = false; // declared without a body (an interface method, or abstract)
};
struct FieldInfo { Ty ty; bool isStatic = false; };
struct ClassInfo {
    std::map<std::string, FieldInfo> fields;
    std::vector<MethodSig> methods;
    std::string innerOf;               // for an inner (non-static) class: the hoisted name of its enclosing class
    bool isInterface = false;          // an interface (its bodiless methods are the functional-interface candidates)
    bool isEnum = false;               // an enum type (constants are static fields of the class)
    std::string super;                 // simple name of the superclass, "" if none
    std::vector<std::string> ifaces;   // simple names of implemented / extended interfaces
};

// Member signatures of every type seen so far. Collecting the whole JDK tree
// first lets calls and field reads across files be typed (and inherited members
// be found through the super chain).
struct SymbolTable {
    std::map<std::string, ClassInfo> classes;
    // nested type simple name -> hoisted top-level name (Outer__Inner); "\x01" marks a name used by several nested types
    std::map<std::string, std::string> nested;
};

struct Diagnostic {
    int line = 0, col = 0;
    std::string construct;   // model "unsupported" key, or "syntax"/"model"
    std::string message;
};

struct OutFile {
    std::string typeName;    // top-level Java type
    std::string text;        // complete SLeeLa document
};

struct Options {
    std::string sourceName;  // path shown in the generated header
    std::string jdkVersion;  // presumed JDK version, e.g. "28"
    std::string author = "unknown";
    // true: all top-level types of the compilation unit go into ONE document named
    // bundleName (what a runnable program needs: a user program does not auto-link
    // sibling files). false: one document per type (library mode, as under lib/java).
    bool bundle = false;
    std::string bundleName;
    // The converted program runs on one thread: Java synchronized and volatile are then plain
    // (they only order accesses between threads). Off by default: without it they are refused.
    bool singleThread = false;
};

struct Result {
    std::vector<OutFile> files;
    std::vector<Diagnostic> diagnostics;   // empty => every member converted
    bool ok() const { return diagnostics.empty(); }
};

// Convert one Java compilation unit. When any member fails, `files` is empty
// (fail closed: a partly converted type is never emitted).
Result convert(const std::string& javaSource, const cm_model& model, const Options& opt,
               SymbolTable* shared = nullptr);

// Pass 1 only: record the signatures of every type in the unit into `table`.
void collect(const std::string& javaSource, const cm_model& model, const Options& opt, SymbolTable& table);

}  // namespace javaconv

#endif
