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
#include <set>
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
    bool boxed = false; // wrapper reference type; emitted as its primitive representation
    int wildcard = 0; // 0 exact, 1 ?, 2 ? extends, 3 ? super
    std::vector<Ty> args; // invariant generic arguments; emitted with erasure
    std::shared_ptr<Ty> bound; // wildcard bound, when present
    bool operator==(const Ty& o) const {
        if (n != o.n || dims != o.dims || boxed != o.boxed || wildcard != o.wildcard || args != o.args) return false;
        if (bool(bound) != bool(o.bound)) return false;
        return !bound || *bound == *o.bound;
    }
    bool known() const { return n != "?" && n.rfind("$T$", 0) != 0 && n.rfind("$C$", 0) != 0; }
};

struct MethodSig {
    std::string name;
    Ty ret;
    std::vector<Ty> params;
    std::vector<std::string> typeParams; // method-level type variables retained for invocation inference
    bool hasTypeParamBounds = false; // true when one or more type parameters declare bounds
    std::map<std::string, std::vector<std::string>> typeParamBounds; // every declared upper bound, including intersections
    bool isStatic = false;
    bool varargs = false;   // the last parameter is T... (its type is stored as T[])
    bool isAbstract = false; // declared without a body (an interface method, or abstract)
    std::string owner;       // the class that declares it
    std::string visibility = "package";
    // The return type as declared (before invocation/class substitution); set on the
    // per-call-site signatures pickMethod returns. Its erasure is what the emitted
    // SLeeLa method returns, so a type-variable declaration needs a cast at the call.
    std::shared_ptr<Ty> declaredRet;
    std::vector<Ty> declaredParams;   // the parameter types as declared (empty when not recorded)
};
struct FieldInfo { Ty ty; bool isStatic = false; std::string visibility = "package"; };
struct ClassInfo {
    std::string sourceName;           // source unit that contributed this declaration
    std::string packageName;          // declared Java package (empty for the unnamed package)
    std::string typeVisibility = "package";
    bool ambiguous = false;           // duplicate simple name across project units: fail closed
    bool seeded = false;              // a JDK functional interface seeded for lambda targets, not a project type
    std::map<std::string, FieldInfo> fields;
    std::vector<MethodSig> methods;
    std::vector<std::string> typeParams; // declared class type variables, in source order
    std::map<std::string, std::vector<std::string>> typeParamBounds; // supported declared class upper bounds
    std::string innerOf;               // for an inner (non-static) class: the hoisted name of its enclosing class
    std::string enclosingType;         // hoisted enclosing type for every member type, including static nested types
    bool isStaticNested = false;       // distinguishes static member types from non-static inner classes
    bool isInterface = false;          // an interface (its bodiless methods are the functional-interface candidates)
    bool isEnum = false;               // an enum type (constants are static fields of the class)
    std::string super;                 // simple name of the superclass, "" if none
    Ty superType;                       // parameterized superclass type, when declared
    std::vector<std::string> ifaces;   // simple names of implemented / extended interfaces
    std::vector<Ty> ifaceTypes;         // parameterized interface types, in declaration order
};

// Member signatures of every type seen so far. Collecting the whole JDK tree
// first lets calls and field reads across files be typed (and inherited members
// be found through the super chain).
struct SymbolTable {
    std::map<std::string, ClassInfo> classes;
    // nested type simple name -> hoisted top-level name (Outer__Inner); "\x01" marks a name used by several nested types
    std::map<std::string, std::string> nested;
    // Method names emitted under signature names (overloaded user methods). Computed
    // once, after every declaration of the project is collected, and shared by all
    // source units so a name is renamed the same way everywhere.
    std::set<std::string> overloadedNames;
    bool overloadsComputed = false;
};

// The SLeeLa side of non-project types: the /lib/java units the SLeeLa loader can resolve,
// and for the JDK tier (lib/java/jdk-28) the members each unit declares. A member in the
// unit's "java signatures" section is a stub that only returns a default value.
// `stub` is a body that only returns a default value. An abstract method of an interface unit
// is written in the same stub form, but calling it dispatches to the implementing class, so it
// is not a stub (`stub` stays false for it).
struct LibMember { std::string name; std::string ret; int arity = 0; bool isStatic = false; bool stub = false; bool ctor = false; };
// A field declared by a lib/java/jdk-28 unit. In the generated "java declarations" section a
// reflected constant is `static final` with its JDK value; a non-final static there is a
// placeholder (`= null`, or 0.0 for an infinity) whose value is not the JDK value.
// `ty` is the declared SLeeLa type, which is lossy (a Java char is stored as int, a float as double).
struct LibField { std::string name; std::string ty; bool isStatic = false; bool placeholder = false; };
struct LibUnit {
    bool parsed = false;          // the unit was found and read
    std::string super;            // SLeeLa superclass named in the class header, "" if none
    bool declaresCtor = false;
    std::vector<LibMember> members;
    std::vector<LibField> fields;
};
struct LibraryIndex {
    std::set<std::string> units;                  // file stems under lib/java (e.g. "Objects", "Map.Entry")
    std::map<std::string, std::string> jdkPath;   // stem -> the lib/java/jdk-28 file the loader selects
    mutable std::map<std::string, LibUnit> cache; // parsed on first use
    // nullptr when the stem has no lib/java/jdk-28 unit (its members are then not checked)
    const LibUnit* unit(const std::string& stem) const;
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
    // Java throws NegativeArraySizeException for a negative array size; the SLeeLa VM stops the program.
    // The converter wraps every non-constant size in JavaChecks.size(n). (An out-of-range INDEX needs no
    // wrapper: the VM throws it inside a try, SLEELA.syntax 37.5 and lib/java/support/SLJavaFaults.)
    // --unchecked-sizes omits the wrapper (NOT equivalent for a program that reaches a negative size).
    bool checkedArrays = true;
    // The SLeeLa units of the Java tier. When set, a type that is neither a project type of
    // this run nor a model type must be a lib/java unit, or the member fails closed
    // [library-type]; a call into a lib/java/jdk-28 unit must name a member that unit
    // declares with real logic, or it fails closed [library-member] / [library-stub].
    const LibraryIndex* library = nullptr;
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
