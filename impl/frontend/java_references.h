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
#include <string>
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
}
}
#endif
