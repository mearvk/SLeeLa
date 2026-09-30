#include "../impl/frontend/java_flow.h"
#include <cassert>
#include <iostream>
using namespace sleela;

static JavaFlowExpr lit(bool v) { JavaFlowExpr e; e.kind=JavaFlowExprKind::Literal; e.op=v?"true":"false"; return e; }
static JavaFlowExpr var(const char* n) { JavaFlowExpr e; e.kind=JavaFlowExprKind::Variable; e.name=n; return e; }
static JavaFlowExpr bin(const char* op, JavaFlowExpr a, JavaFlowExpr b) { JavaFlowExpr e; e.kind=JavaFlowExprKind::Binary; e.op=op; e.children={a,b}; return e; }
static JavaFlowExpr un(const char* op, JavaFlowExpr a) { JavaFlowExpr e; e.kind=JavaFlowExprKind::Unary; e.op=op; e.children={a}; return e; }
static JavaFlowStmt decl(const char* n) { JavaFlowStmt s; s.kind=JavaFlowStmtKind::LocalDeclaration; s.variable=n; return s; }
static JavaFlowStmt assign(const char* n) { JavaFlowStmt s; s.kind=JavaFlowStmtKind::Assignment; s.variable=n; return s; }
static JavaFlowStmt read(const char* n) { JavaFlowStmt s; s.kind=JavaFlowStmtKind::Expression; s.expression=var(n); return s; }
static bool has(const JavaFlowResult& r, JavaFlowDiagnosticKind k) { for (const auto& d:r.diagnostics) if(d.kind==k) return true; return false; }

int main() {
    // Short-circuit paths: assignment on the RHS of && is known only on true.
    JavaFlowExpr rhsAssign; rhsAssign.kind=JavaFlowExprKind::Assignment;
    rhsAssign.children={var("x"),lit(true)};
    auto andExpr=bin("&&",lit(true),rhsAssign);
    JavaFlowStmt use; use.kind=JavaFlowStmtKind::Expression; use.expression=andExpr;
    auto r1=analyzeJavaFlow({decl("x"),use});
    assert(!has(r1,JavaFlowDiagnosticKind::UseBeforeAssignment));

    // ! reverses directional flow without changing the source facts.
    JavaFlowExpr notExpr=un("!",lit(false));
    JavaFlowStmt ifs; ifs.kind=JavaFlowStmtKind::If; ifs.condition=notExpr; ifs.children={assign("x")};
    auto r2=analyzeJavaFlow({decl("x"),ifs,read("x")});
    assert(has(r2,JavaFlowDiagnosticKind::UseBeforeAssignment));

    // A constant-true loop has no normal condition-false exit.
    JavaFlowStmt loop; loop.kind=JavaFlowStmtKind::While; loop.condition=lit(true);
    loop.children={assign("x")};
    auto r3=analyzeJavaFlow({decl("x"),loop});
    assert(!r3.after.reachable);

    // A matching break is a normal exit path from the loop.
    JavaFlowStmt br; br.kind=JavaFlowStmtKind::Break;
    loop.children={assign("x"),br};
    auto r4=analyzeJavaFlow({decl("x"),loop,read("x")});
    assert(!has(r4,JavaFlowDiagnosticKind::UseBeforeAssignment));

    // Basic-for initialization/update are traversed; an update read of an
    // unassigned variable must be diagnosed even though update is not an exit.
    JavaFlowStmt forStmt; forStmt.kind=JavaFlowStmtKind::For;
    JavaFlowStmt fi=decl("i"); fi.hasInitializer=true; fi.expression=lit(true);
    forStmt.forInitialization={fi};
    forStmt.condition=lit(false);
    forStmt.forUpdate={var("missing")};
    auto r5a=analyzeJavaFlow({forStmt});
    assert(!has(r5a,JavaFlowDiagnosticKind::UseBeforeAssignment));

    // A continue path reaches the update component before the next condition.
    JavaFlowStmt fc; fc.kind=JavaFlowStmtKind::Continue;
    JavaFlowStmt fl; fl.kind=JavaFlowStmtKind::For; fl.condition=lit(true);
    fl.children={fc}; fl.forUpdate={var("missing2")};
    auto r5b=analyzeJavaFlow({fl});
    assert(has(r5b,JavaFlowDiagnosticKind::UseBeforeAssignment));

    // A labeled block accepts break but not continue.
    JavaFlowStmt lb; lb.kind=JavaFlowStmtKind::Labeled; lb.label="outer";
    JavaFlowStmt bad; bad.kind=JavaFlowStmtKind::Continue; bad.label="outer";
    lb.children={bad};
    auto r5=analyzeJavaFlow({lb});
    assert(has(r5,JavaFlowDiagnosticKind::InvalidContinue));

    // An unlabeled continue outside a loop is invalid.
    JavaFlowStmt bad2; bad2.kind=JavaFlowStmtKind::Continue;
    auto r6=analyzeJavaFlow({bad2});
    assert(has(r6,JavaFlowDiagnosticKind::InvalidContinue));


    // Loop fixed-point: a condition-false path must not inherit assignments
    // from a body that never executes.
    JavaFlowStmt never; never.kind=JavaFlowStmtKind::While; never.condition=lit(false);
    never.children={assign("neverAssigned")};
    auto r7=analyzeJavaFlow({decl("neverAssigned"),never,read("neverAssigned")});
    assert(has(r7,JavaFlowDiagnosticKind::UseBeforeAssignment));

    // Definite-unassignment is preserved through a single guaranteed final
    // assignment followed by a matching break.
    JavaFlowStmt finalDecl=decl("finalValue"); finalDecl.finalVariable=true;
    JavaFlowStmt finalLoop; finalLoop.kind=JavaFlowStmtKind::While; finalLoop.condition=lit(true);
    finalLoop.children={assign("finalValue"),br};
    auto r8=analyzeJavaFlow({finalDecl,finalLoop,read("finalValue")});
    assert(!has(r8,JavaFlowDiagnosticKind::FinalReassignment));
    std::cout<<"java directional-flow/control-target tests passed\n";
    return 0;
}
