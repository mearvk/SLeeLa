#include "../impl/frontend/java_flow.h"
#include <cassert>
#include <iostream>
using namespace sleela;

static JavaFlowExpr var(const char* n) { JavaFlowExpr e; e.kind=JavaFlowExprKind::Variable; e.name=n; return e; }
static JavaFlowExpr lit() { return {}; }
static JavaFlowStmt decl(const char* n, bool init=false) { JavaFlowStmt s; s.kind=JavaFlowStmtKind::LocalDeclaration; s.variable=n; s.hasInitializer=init; if(init)s.expression=lit(); return s; }
static JavaFlowStmt assign(const char* n) { JavaFlowStmt s; s.kind=JavaFlowStmtKind::Assignment; s.variable=n; s.expression=lit(); return s; }
static JavaFlowStmt read(const char* n) { JavaFlowStmt s; s.kind=JavaFlowStmtKind::Expression; s.expression=var(n); return s; }

static bool has(const JavaFlowResult& r, JavaFlowDiagnosticKind k) {
    for (const auto& d : r.diagnostics) if (d.kind==k) return true;
    return false;
}

int main() {
    auto r1=analyzeJavaFlow({decl("x"), read("x")});
    assert(has(r1,JavaFlowDiagnosticKind::UseBeforeAssignment));

    auto r2=analyzeJavaFlow({decl("x"), assign("x"), read("x")});
    assert(!has(r2,JavaFlowDiagnosticKind::UseBeforeAssignment));
    assert(r2.after.definitelyAssigned.count("x")==1);

    JavaFlowStmt branch; branch.kind=JavaFlowStmtKind::If;
    branch.condition=lit();
    branch.children={assign("x")};
    branch.elseChildren={assign("x")};
    auto r3=analyzeJavaFlow({decl("x"),branch,read("x")});
    assert(!has(r3,JavaFlowDiagnosticKind::UseBeforeAssignment));

    JavaFlowStmt partial; partial.kind=JavaFlowStmtKind::If;
    partial.condition=lit();
    partial.children={assign("x")};
    auto r4=analyzeJavaFlow({decl("x"),partial,read("x")});
    assert(has(r4,JavaFlowDiagnosticKind::UseBeforeAssignment));

    JavaFlowStmt ret; ret.kind=JavaFlowStmtKind::Return; ret.expression=lit();
    auto r5=analyzeJavaFlow({ret,read("x")});
    assert(has(r5,JavaFlowDiagnosticKind::UnreachableStatement));

    JavaFlowStmt loop; loop.kind=JavaFlowStmtKind::While; loop.condition=lit(); loop.conditionConstant=true; loop.conditionValue=true;
    loop.children={assign("x")};
    auto r6=analyzeJavaFlow({decl("x"),loop});
    assert(!r6.after.reachable);

    JavaFlowStmt fin; fin.kind=JavaFlowStmtKind::Try;
    fin.children={assign("x")};
    fin.finallyBlock={read("x")};
    auto r7=analyzeJavaFlow({decl("x"),fin});
    assert(!has(r7,JavaFlowDiagnosticKind::UseBeforeAssignment));

    JavaFlowStmt finBad; finBad.kind=JavaFlowStmtKind::Try;
    finBad.children={};
    finBad.finallyBlock={read("x")};
    auto r8=analyzeJavaFlow({decl("x"),finBad});
    assert(has(r8,JavaFlowDiagnosticKind::UseBeforeAssignment));

    JavaFlowExpr andExpr; andExpr.kind=JavaFlowExprKind::Binary; andExpr.op="&&"; andExpr.children={var("flag"),var("x")};
    JavaFlowStmt andRead; andRead.kind=JavaFlowStmtKind::Expression; andRead.expression=andExpr;
    auto r9=analyzeJavaFlow({decl("x"),andRead},{"flag"});
    assert(has(r9,JavaFlowDiagnosticKind::UseBeforeAssignment));

    JavaFlowStmt badBreak; badBreak.kind=JavaFlowStmtKind::Break; badBreak.label="missing";
    auto r10=analyzeJavaFlow({badBreak});
    assert(has(r10,JavaFlowDiagnosticKind::InvalidBreak));

    JavaFlowStmt loop2; loop2.kind=JavaFlowStmtKind::While; loop2.condition=lit();
    JavaFlowStmt goodBreak; goodBreak.kind=JavaFlowStmtKind::Break;
    loop2.children={assign("x"),goodBreak};
    auto r11=analyzeJavaFlow({decl("x"),loop2});
    assert(!has(r11,JavaFlowDiagnosticKind::InvalidBreak));

    std::cout<<"java definite-assignment/reachability tests passed\n";
    return 0;
}
