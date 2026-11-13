// ===========================================================================
// class_relations.h -- language-neutral class relations (extends / implements
// chains, subtype test, inheritance depth, Sleela.home lineage).
// ===========================================================================
// Every stage that follows an `extends` edge (semantic analysis, instance-field
// layout, runtime parent registration, method aliasing, Sleela.home lineage,
// Any dispatch) goes through this module so they all agree. A declared name is
// resolved by exact match first; anything else is offered to the registered
// language handlers (language_handler.h), so this module holds no knowledge of
// any foreign naming convention. Sleela.home lineage is a SLeeLa feature
// (SLEELA.syntax section 42), independent of any handler.
// ===========================================================================
#ifndef SLEELA_CLASS_RELATIONS_H
#define SLEELA_CLASS_RELATIONS_H
#include "ast.h"
#include "language_handler.h"
#include <string>
#include <vector>
namespace sleela {
namespace classrel {
using ClassIndex = ClassIndexMap;
// Resolve a declared base/interface/catch type name (see lang::resolveReference).
std::string resolveBaseName(const ClassIndex& byName,const std::string& declared);
// Resolved superclass of a class ("" when it has none).
std::string superclassOf(const ClassIndex& byName,const ClassDecl& cls);
// "List<E>" -> "List" (type arguments dropped).
std::string stripTypeArguments(const std::string& t);
// True when `from` is `to` or reaches it through extends or implemented interfaces. Cycle-safe.
bool isSubtypeOf(const ClassIndex& classes,const std::string& from,const std::string& to);
// Number of loaded superclasses above `n` (0 for a root). Cycle-safe.
int inheritanceDepth(const ClassIndex& classes,const std::string& n);
// Most derived first; ties keep input order.
std::vector<std::string> mostDerivedFirst(const ClassIndex& classes,const std::vector<std::string>& names);
// ---- Sleela.home lineage (SLEELA.syntax section 42) ----
std::string lineageParentOf(const ClassIndex& classes,const std::string& n);
std::string lineageRootOf(const ClassIndex& classes,std::string n);
std::string lineageValue(const ClassIndex& classes,const std::string& currentClass,const LineageExpr& x);
}
}
#endif
