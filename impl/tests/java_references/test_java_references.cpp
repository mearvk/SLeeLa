// Build (from impl/): g++ -std=c++17 -Ifrontend tests/java_references/test_java_references.cpp frontend/java_references.cpp -o /tmp/jr_test
#include "java_references.h"
#include <cassert>
#include <iostream>
using namespace sleela;
int main(){
  ClassDecl lib; lib.name="Throwable"; lib.fromLibrary=true;
  ClassDecl loc; loc.name="Object"; loc.fromLibrary=false;
  javaref::ClassIndex m{{"Throwable",&lib},{"Object",&loc}};
  assert(javaref::resolveBaseName(m,"Throwable")=="Throwable");
  assert(javaref::resolveBaseName(m,"java.lang.Throwable")=="Throwable");
  assert(javaref::resolveBaseName(m,"java.lang.Object")=="java.lang.Object"); // local cannot impersonate
  assert(javaref::resolveBaseName(m,"java.lang.Missing")=="java.lang.Missing");
  assert(javaref::resolveBaseName(m,"")=="");
  assert(javaref::simpleName("a.b.C")=="C" && javaref::simpleName("C")=="C");
  std::cout<<"java_references: ok\n";
}
