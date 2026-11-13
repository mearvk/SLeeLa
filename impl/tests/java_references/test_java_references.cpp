// Build (from impl/):
//   with the Java handler:  g++ -std=c++17 -Ifrontend tests/java_references/test_java_references.cpp frontend/class_relations.cpp frontend/language_handler.cpp frontend/java/java_handler.cpp -o /tmp/jr_test
//   without any handler:    g++ -std=c++17 -DNO_HANDLER -Ifrontend tests/java_references/test_java_references.cpp frontend/class_relations.cpp frontend/language_handler.cpp -o /tmp/jr_none
#include "class_relations.h"
#ifndef NO_HANDLER
#include "java/java_handler.h"
#endif
#include <cassert>
#include <iostream>
using namespace sleela;
int main(){
  ClassDecl lib; lib.name="Throwable"; lib.fromLibrary=true;
  ClassDecl loc; loc.name="Object"; loc.fromLibrary=false;
  classrel::ClassIndex m{{"Throwable",&lib},{"Object",&loc}};
  assert(classrel::resolveBaseName(m,"Throwable")=="Throwable");
  assert(classrel::resolveBaseName(m,"")=="");
  assert(classrel::resolveBaseName(m,"java.lang.Missing")=="java.lang.Missing");
#ifndef NO_HANDLER
  assert(classrel::resolveBaseName(m,"java.lang.Throwable")=="Throwable");
  assert(classrel::resolveBaseName(m,"java.lang.Object")=="java.lang.Object"); // local cannot impersonate
  assert(javahandler::simpleName("a.b.C")=="C" && javahandler::simpleName("C")=="C");
  assert(lang::isDocumentMarker("spark") && lang::stringMethodLibrary()=="JavaStringMethods");
  std::cout<<"java_references (java handler registered): ok\n";
#else
  // With no handler the compiler core has no Java knowledge at all.
  assert(classrel::resolveBaseName(m,"java.lang.Throwable")=="java.lang.Throwable");
  assert(!lang::isDocumentMarker("spark") && lang::stringMethodLibrary().empty() && lang::faultFunction().empty());
  assert(lang::eraseSignatureType("java.util.List<E>",{})=="java.util.List<E>");
  std::cout<<"java_references (no handler): ok\n";
#endif
}
