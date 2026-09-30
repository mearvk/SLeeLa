// ===========================================================================
// java_flow.cpp -- Java source-level definite-assignment/reachability model.
// ===========================================================================
#include "java_flow.h"
#include <algorithm>

namespace sleela {

namespace {
struct Context {
    std::set<std::string> assigned;
    std::set<std::string> unassigned;
    bool reachable = true;
    std::vector<JavaFlowDiagnostic> diagnostics;
    std::vector<JavaFlowFacts> breakFacts;
    std::vector<JavaFlowFacts> continueFacts;
};

static void diagnostic(Context& c, JavaFlowDiagnosticKind k,
                       const std::string& v, const std::string& m) {
    c.diagnostics.push_back({k, v, m, true});
}

static std::set<std::string> intersectSets(const std::vector<std::set<std::string>>& xs) {
    if (xs.empty()) return {};
    std::set<std::string> out = xs.front();
    for (std::size_t i = 1; i < xs.size(); ++i) {
        for (auto it = out.begin(); it != out.end();) {
            if (xs[i].find(*it) == xs[i].end()) it = out.erase(it);
            else ++it;
        }
    }
    return out;
}

static void readExpr(Context& c, const JavaFlowExpr& e);
static void writeExpr(Context& c, const JavaFlowExpr& e);

static void readExpr(Context& c, const JavaFlowExpr& e) {
    if (!c.reachable) return;
    if (e.kind == JavaFlowExprKind::Variable) {
        if (c.assigned.find(e.name) == c.assigned.end())
            diagnostic(c, JavaFlowDiagnosticKind::UseBeforeAssignment, e.name,
                       "variable is not definitely assigned before this read");
        return;
    }
    if (e.kind == JavaFlowExprKind::Assignment) {
        if (!e.children.empty()) {
            if (e.children.front().kind == JavaFlowExprKind::Variable)
                writeExpr(c, e.children.front());
            else
                readExpr(c, e.children.front());
        }
        if (e.children.size() > 1) readExpr(c, e.children[1]);
        return;
    }
    if (e.kind == JavaFlowExprKind::CompoundAssignment ||
        (e.kind == JavaFlowExprKind::Unary && (e.op == "++" || e.op == "--"))) {
        if (!e.children.empty()) readExpr(c, e.children.front());
        if (e.kind == JavaFlowExprKind::CompoundAssignment && e.children.size() > 1)
            readExpr(c, e.children[1]);
        return;
    }
    if (e.kind == JavaFlowExprKind::Conditional && e.children.size() >= 3) {
        readExpr(c, e.children[0]);
        Context left = c, right = c;
        readExpr(left, e.children[1]);
        readExpr(right, e.children[2]);
        c.assigned = intersectSets({left.assigned, right.assigned});
        c.unassigned = intersectSets({left.unassigned, right.unassigned});
        c.diagnostics.insert(c.diagnostics.end(), left.diagnostics.begin(), left.diagnostics.end());
        c.diagnostics.insert(c.diagnostics.end(), right.diagnostics.begin(), right.diagnostics.end());
        return;
    }
    if (e.kind == JavaFlowExprKind::Binary && (e.op == "&&" || e.op == "||") &&
        e.children.size() >= 2) {
        readExpr(c, e.children[0]);
        Context rhs = c;
        readExpr(rhs, e.children[1]);
        c.assigned = intersectSets({c.assigned, rhs.assigned});
        c.unassigned = intersectSets({c.unassigned, rhs.unassigned});
        c.diagnostics.insert(c.diagnostics.end(), rhs.diagnostics.begin(), rhs.diagnostics.end());
        return;
    }
    for (const auto& child : e.children) readExpr(c, child);
}

static void writeExpr(Context& c, const JavaFlowExpr& e) {
    if (e.kind != JavaFlowExprKind::Variable) {
        readExpr(c, e);
        return;
    }
    if (c.unassigned.find(e.name) == c.unassigned.end() &&
        c.assigned.find(e.name) != c.assigned.end()) {
        diagnostic(c, JavaFlowDiagnosticKind::FinalReassignment, e.name,
                   "blank-final/local-final variable is not definitely unassigned before assignment");
    }
    c.assigned.insert(e.name);
    c.unassigned.erase(e.name);
}

static void analyzeStmt(Context& c, const JavaFlowStmt& s);

static void analyzeList(Context& c, const std::vector<JavaFlowStmt>& ss) {
    for (const auto& s : ss) {
        if (!c.reachable) {
            diagnostic(c, JavaFlowDiagnosticKind::UnreachableStatement, "", "statement is unreachable");
            return;
        }
        analyzeStmt(c, s);
    }
}

static void analyzeStmt(Context& c, const JavaFlowStmt& s) {
    if (!c.reachable) {
        diagnostic(c, JavaFlowDiagnosticKind::UnreachableStatement, "", "statement is unreachable");
        return;
    }
    switch (s.kind) {
    case JavaFlowStmtKind::Empty:
        return;
    case JavaFlowStmtKind::Block:
        analyzeList(c, s.children); return;
    case JavaFlowStmtKind::LocalDeclaration:
        if (s.hasInitializer) {
            readExpr(c, s.expression);
            c.assigned.insert(s.variable);
            c.unassigned.erase(s.variable);
        } else {
            c.unassigned.insert(s.variable);
            c.assigned.erase(s.variable);
        }
        return;
    case JavaFlowStmtKind::Expression:
        readExpr(c, s.expression); return;
    case JavaFlowStmtKind::Assignment:
        // The right-hand side is evaluated before the assignment takes effect.
        // This matters for cases such as: x = x + 1.
        if (s.expression.kind != JavaFlowExprKind::Literal) readExpr(c, s.expression);
        if (s.finalVariable && c.assigned.find(s.variable) != c.assigned.end()) {
            diagnostic(c, JavaFlowDiagnosticKind::FinalReassignment, s.variable,
                       "final variable is assigned more than once");
        }
        c.assigned.insert(s.variable);
        c.unassigned.erase(s.variable);
        return;
    case JavaFlowStmtKind::If: {
        readExpr(c, s.condition);
        Context thenC = c, elseC = c;
        analyzeList(thenC, s.children);
        analyzeList(elseC, s.elseChildren);
        if (s.conditionConstant) {
            c = s.conditionValue ? thenC : elseC;
            return;
        }
        c.assigned = intersectSets({thenC.assigned, elseC.assigned});
        c.unassigned = intersectSets({thenC.unassigned, elseC.unassigned});
        c.reachable = thenC.reachable || elseC.reachable;
        c.diagnostics.insert(c.diagnostics.end(), thenC.diagnostics.begin(), thenC.diagnostics.end());
        c.diagnostics.insert(c.diagnostics.end(), elseC.diagnostics.begin(), elseC.diagnostics.end());
        return;
    }
    case JavaFlowStmtKind::While: {
        readExpr(c, s.condition);
        Context body = c;
        analyzeList(body, s.children);
        if (s.conditionConstant && s.conditionValue) {
            c.reachable = false;
            c.assigned = c.assigned;
        }
        c.diagnostics.insert(c.diagnostics.end(), body.diagnostics.begin(), body.diagnostics.end());
        return;
    }
    case JavaFlowStmtKind::Do: {
        Context body = c;
        analyzeList(body, s.children);
        readExpr(body, s.condition);
        c = body;
        return;
    }
    case JavaFlowStmtKind::For: {
        if (!s.children.empty()) analyzeStmt(c, s.children.front());
        if (s.condition.kind != JavaFlowExprKind::Literal) readExpr(c, s.condition);
        Context body = c;
        if (s.children.size() > 1) analyzeList(body, {s.children.begin()+1, s.children.end()});
        return;
    }
    case JavaFlowStmtKind::Switch: {
        readExpr(c, s.expression);
        std::vector<std::set<std::string>> exits;
        for (const auto& branch : s.switchCases) {
            Context branchC = c;
            analyzeList(branchC, branch);
            if (branchC.reachable) exits.push_back(branchC.assigned);
            c.diagnostics.insert(c.diagnostics.end(), branchC.diagnostics.begin(), branchC.diagnostics.end());
        }
        if (!exits.empty()) c.assigned = intersectSets(exits);
        return;
    }
    case JavaFlowStmtKind::Break:
        c.breakFacts.push_back({c.assigned, c.unassigned, true});
        c.reachable = false; return;
    case JavaFlowStmtKind::Continue:
        c.continueFacts.push_back({c.assigned, c.unassigned, true});
        c.reachable = false; return;
    case JavaFlowStmtKind::Return:
    case JavaFlowStmtKind::Throw:
        if (s.expression.kind != JavaFlowExprKind::Literal) readExpr(c, s.expression);
        c.reachable = false; return;
    case JavaFlowStmtKind::Yield:
        readExpr(c, s.expression); c.reachable = false; return;
    case JavaFlowStmtKind::Assert:
        readExpr(c, s.condition); return;
    case JavaFlowStmtKind::Synchronized:
        readExpr(c, s.condition); analyzeList(c, s.children); return;
    case JavaFlowStmtKind::Try: {
        Context tryC = c;
        analyzeList(tryC, s.children);
        std::vector<std::set<std::string>> exits;
        if (tryC.reachable) exits.push_back(tryC.assigned);
        for (const auto& block : s.catchBlocks) {
            Context catchC = c;
            analyzeList(catchC, block);
            if (catchC.reachable) exits.push_back(catchC.assigned);
            c.diagnostics.insert(c.diagnostics.end(), catchC.diagnostics.begin(), catchC.diagnostics.end());
        }
        if (!s.finallyBlock.empty()) {
            Context fin = c;
            if (!exits.empty()) fin.assigned = intersectSets(exits);
            analyzeList(fin, s.finallyBlock);
            c = fin;
        } else if (!exits.empty()) {
            c.assigned = intersectSets(exits);
        }
        c.diagnostics.insert(c.diagnostics.end(), tryC.diagnostics.begin(), tryC.diagnostics.end());
        return;
    }
    case JavaFlowStmtKind::Labeled:
        analyzeList(c, s.children); return;
    }
}
} // namespace

JavaFlowResult analyzeJavaFlow(const std::vector<JavaFlowStmt>& statements,
                               const std::set<std::string>& initiallyAssigned,
                               const std::set<std::string>& initiallyUnassigned) {
    Context c;
    c.assigned = initiallyAssigned;
    c.unassigned = initiallyUnassigned;
    analyzeList(c, statements);
    JavaFlowResult r;
    r.after.definitelyAssigned = c.assigned;
    r.after.definitelyUnassigned = c.unassigned;
    r.after.reachable = c.reachable;
    r.diagnostics = std::move(c.diagnostics);
    return r;
}

const char* javaFlowDiagnosticName(JavaFlowDiagnosticKind k) {
    switch (k) {
    case JavaFlowDiagnosticKind::UseBeforeAssignment: return "use-before-assignment";
    case JavaFlowDiagnosticKind::FinalReassignment: return "final-reassignment";
    case JavaFlowDiagnosticKind::UnreachableStatement: return "unreachable-statement";
    case JavaFlowDiagnosticKind::InvalidBreak: return "invalid-break";
    case JavaFlowDiagnosticKind::InvalidContinue: return "invalid-continue";
    }
    return "unknown";
}

} // namespace sleela
