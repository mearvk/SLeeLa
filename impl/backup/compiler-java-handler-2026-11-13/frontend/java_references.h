// ===========================================================================
// java_references.h -- the single home for Java name references in the
// SLeeLa frontend (extends / superclass resolution).
// ===========================================================================
// A /lib/java unit may name its base either by simple name (extends Throwable)
// or by its qualified Java name (extends java.lang.Throwable). Every stage that
// follows an `extends` edge (semantic analysis, instance-field layout, runtime
// parent registration, method aliasing, Sleela.home lineage) MUST resolve the
// declared name through this module so they all agree. Source semantics only:
// no JVM is involved. Backup of the pre-change files:
//   impl/backup/java-references-2026-11-12/
#ifndef SLEELA_JAVA_REFERENCES_H
#define SLEELA_JAVA_REFERENCES_H
#include "ast.h"
#include <map>
#include <set>
#include <string>
#include <vector>
namespace sleela {
namespace javaref {
using ClassIndex = std::map<std::string,const ClassDecl*>;

// Last segment of a qualified Java name ("java.lang.Throwable" -> "Throwable").
// A name with no '.' is returned unchanged.
std::string simpleName(const std::string& qualified);

// True when the name contains a package qualifier.
bool isQualified(const std::string& name);

// Resolve a declared superclass against the loaded classes. An exact match wins.
// A qualified name falls back to its simple name only when that class came from
// /lib (fromLibrary), so a local class named Object cannot impersonate
// java.lang.Object. Unresolvable names are returned unchanged.
std::string resolveBaseName(const ClassIndex& byName,const std::string& declared);

// Resolved superclass of a class ("" when it has none).
std::string superclassOf(const ClassIndex& byName,const ClassDecl& cls);

// ---- Java type relations (SLEELA.syntax section 43.6) --------------------
// "java.util.List<E>" -> "java.util.List" (type arguments dropped).
std::string stripTypeArguments(const std::string& t);
// True when class `from` is `to` or reaches it through its extends chain or
// any implemented interface (transitively). Names resolve as resolveBaseName.
// Cycle-safe. Unknown names are only equal to themselves.
bool isSubtypeOf(const ClassIndex& classes,const std::string& from,const std::string& to);
// Number of loaded superclasses above `n` (0 for a root). Cycle-safe.
int inheritanceDepth(const ClassIndex& classes,const std::string& n);
// Order class names so every class comes before each of its supertypes in
// the list (most derived first; SLEELA.syntax 43.7 dynamic dispatch on Any).
// A class that is a subtype of more listed names sorts earlier; ties keep the
// input order.
std::vector<std::string> mostDerivedFirst(const ClassIndex& classes,const std::vector<std::string>& names);

// ---- spark Preonem (SLEELA.syntax section 43) ----------------------------
// True when the document carries the `spark` marker, which indicates Java style
// declarative: SLeeLa constructs written in Java style, supported at the interpreter/VM level (this module, the
// compiler, the VM), never through a SLeeLa housing class.
bool sparkDocument(const Program& p);

// ---- Sleela.home lineage (SLEELA.syntax section 42) ----------------------
// Resolved at compile time against the loaded classes; every extends edge goes
// through resolveBaseName above.
// Resolved parent of n that is itself a loaded class ("" when none).
std::string lineageParentOf(const ClassIndex& classes,const std::string& n);
// Root of n's lineage (n itself when it has no loaded parent; cycle-safe).
std::string lineageRootOf(const ClassIndex& classes,std::string n);
// Constant string value of a Sleela.home expression inside currentClass.
// Sleela.System names the VM and is the value when no class applies.
std::string lineageValue(const ClassIndex& classes,const std::string& currentClass,const LineageExpr& x);
}
}
#endif
