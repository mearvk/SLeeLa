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
  // Java tier selection (moved from the driver): lang::filterLibraryCandidates.
  {
    const std::string L1="java/jdk-28/java.base/java/util/List.sleela", L2="java/jdk-28/java.desktop/java/awt/List.sleela";
    const std::string V1="java/jdk-28/java.base/java/util/Vector.sleela", V2="java/jdk-28/jdk.incubator.vector/jdk/incubator/vector/Vector.sleela";
    const std::string real="java/util/List.sleela", doc="java/early_access/jdk28/docs/api/java/util/List.sleela";
    auto one=[&](std::vector<std::string> in,std::vector<std::string> roots){ return lang::filterLibraryCandidates("X",in,roots); };
    assert((one({L1,L2},{"java/jdk-28"})==std::vector<std::string>{L1}));                    // java.base beats java.desktop
    assert((one({L1,L2},{"java/jdk-28","java/jdk-28/java.desktop"})==std::vector<std::string>{L2})); // longest import root wins
    assert((one({V2,V1},{"java/jdk-28"})==std::vector<std::string>{V1}));                    // java.* beats jdk.*
    assert((one({real,doc,L1},{})==std::vector<std::string>{real}));                          // real source beats reference files
    assert((one({doc,L1},{})==std::vector<std::string>{L1}));                                 // jdk-28 beats docs/api
    assert((one({real,L1},{"java/jdk-28"})==std::vector<std::string>{L1}));                   // import java.jdk-28 selects the tier
    assert((one({doc},{})==std::vector<std::string>{doc}));                                   // docs/api only as a last resort
  }
  std::cout<<"java_references (java handler registered): ok\n";
#else
  // With no handler the compiler core has no Java knowledge at all.
  assert(classrel::resolveBaseName(m,"java.lang.Throwable")=="java.lang.Throwable");
  assert(!lang::isDocumentMarker("spark") && lang::stringMethodLibrary().empty() && lang::faultFunction().empty());
  assert(lang::eraseSignatureType("java.util.List<E>",{})=="java.util.List<E>");
  assert((lang::filterLibraryCandidates("X",{"a","b"},{})==std::vector<std::string>{"a","b"}));
  { int rc=7; assert(!lang::runHost({"x.java"},rc) && rc==7); }
  std::cout<<"java_references (no handler): ok\n";
#endif
}
