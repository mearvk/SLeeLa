#include "../impl/frontend/java_exceptions.h"
#include <cassert>
#include <iostream>
using namespace sleela;
int main(){
 JavaExceptionModel m;
 m.addType({"java.lang.Throwable","",false});
 m.addType({"java.lang.Exception","java.lang.Throwable",false});
 m.addType({"java.lang.RuntimeException","java.lang.Exception",true});
 m.addType({"java.io.IOException","java.lang.Exception",false});
 m.addType({"java.io.FileNotFoundException","java.io.IOException",false});
 auto a=m.check({"java.io.IOException"},{},{"java.io.IOException"});
 assert(a.diagnostics.empty());
 auto b=m.check({"java.io.IOException"},{},{});
 assert(!b.diagnostics.empty() && b.diagnostics[0].kind==JavaExceptionDiagnosticKind::UnhandledCheckedException);
 auto c=m.check({"java.io.FileNotFoundException"},{"java.io.IOException"},{});
 assert(c.diagnostics.empty());
 auto d=m.check({"java.lang.RuntimeException"},{},{});
 assert(d.diagnostics.empty());
 auto e=m.check({"java.io.IOException"},{"java.lang.Exception"},{});
 assert(e.diagnostics.empty());
 auto f=m.check({"java.io.IOException"},{"java.lang.Exception","java.io.IOException"},{});
 assert(f.diagnostics.size()==1 && f.diagnostics[0].kind==JavaExceptionDiagnosticKind::RedundantCatch);
 assert(m.overrideThrowsCompatible({"java.io.FileNotFoundException"},{"java.io.IOException"}));
 assert(!m.overrideThrowsCompatible({"java.io.IOException"},{"java.io.FileNotFoundException"}));
 std::cout<<"java checked-exception tests passed\n";
}
