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
    struct AbruptFact { JavaFlowFacts facts; std::string label; };
    std::vector<AbruptFact> breakFacts;
    std::vector<AbruptFact> continueFacts;
    std::vector<std::string> breakTargets;
    std::vector<std::string> continueTargets;
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

static JavaFlowFacts factsOf(const Context& c) {
    return {c.assigned, c.unassigned, c.reachable};
}

static void restoreFacts(Context& c, const JavaFlowFacts& f) {
    c.assigned=f.definitelyAssigned; c.unassigned=f.definitelyUnassigned; c.reachable=f.reachable;
}

static bool isBooleanConstant(const JavaFlowExpr& e, bool& value) {
    if (e.kind != JavaFlowExprKind::Literal) return false;
    if (e.op == "true") { value=true; return true; }
    if (e.op == "false") { value=false; return true; }
    return false;
}

static void analyzeBoolean(Context& c, const JavaFlowExpr& e,
                           JavaFlowFacts& whenTrue, JavaFlowFacts& whenFalse) {
    Context base=c;
    bool value=false;
    if (isBooleanConstant(e,value)) {
        whenTrue=factsOf(base); whenFalse=factsOf(base);
        if (value) whenFalse.reachable=false; else whenTrue.reachable=false;
        return;
    }
    if (e.kind==JavaFlowExprKind::Unary && e.op=="!" && !e.children.empty()) {
        JavaFlowFacts t,f; analyzeBoolean(c,e.children[0],t,f);
        whenTrue=f; whenFalse=t; return;
    }
    if (e.kind==JavaFlowExprKind::Binary && e.children.size()>=2 && e.op=="&&") {
        JavaFlowFacts lt,lf; analyzeBoolean(c,e.children[0],lt,lf);
        Context rhs=c; restoreFacts(rhs,lt);
        JavaFlowFacts rt,rf; analyzeBoolean(rhs,e.children[1],rt,rf);
        whenTrue=rt;
        whenFalse={intersectSets({lf.definitelyAssigned,rf.definitelyAssigned}),
                   intersectSets({lf.definitelyUnassigned,rf.definitelyUnassigned}),
                   lf.reachable||rf.reachable};
        return;
    }
    if (e.kind==JavaFlowExprKind::Binary && e.children.size()>=2 && e.op=="||") {
        JavaFlowFacts lt,lf; analyzeBoolean(c,e.children[0],lt,lf);
        Context rhs=c; restoreFacts(rhs,lf);
        JavaFlowFacts rt,rf; analyzeBoolean(rhs,e.children[1],rt,rf);
        whenFalse=rf;
        whenTrue={intersectSets({lt.definitelyAssigned,rt.definitelyAssigned}),
                  intersectSets({lt.definitelyUnassigned,rt.definitelyUnassigned}),
                  lt.reachable||rt.reachable};
        return;
    }
    if (e.kind==JavaFlowExprKind::Conditional && e.children.size()>=3) {
        JavaFlowFacts ct,cf; analyzeBoolean(c,e.children[0],ct,cf);
        Context a=c,b=c; restoreFacts(a,ct); restoreFacts(b,cf);
        JavaFlowFacts at,af,bt,bf;
        analyzeBoolean(a,e.children[1],at,af); analyzeBoolean(b,e.children[2],bt,bf);
        whenTrue={intersectSets({at.definitelyAssigned,bt.definitelyAssigned}),
                  intersectSets({at.definitelyUnassigned,bt.definitelyUnassigned}),
                  at.reachable||bt.reachable};
        whenFalse={intersectSets({af.definitelyAssigned,bf.definitelyAssigned}),
                   intersectSets({af.definitelyUnassigned,bf.definitelyUnassigned}),
                   af.reachable||bf.reachable};
        return;
    }
    Context evaluated=c; readExpr(evaluated,e);
    whenTrue=factsOf(evaluated); whenFalse=factsOf(evaluated);
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
    if ((e.kind == JavaFlowExprKind::Conditional && e.children.size() >= 3) ||
        (e.kind == JavaFlowExprKind::Binary && (e.op == "&&" || e.op == "||") && e.children.size() >= 2) ||
        (e.kind == JavaFlowExprKind::Unary && e.op == "!" && !e.children.empty())) {
        JavaFlowFacts t,f; analyzeBoolean(c,e,t,f);
        c.assigned=intersectSets({t.definitelyAssigned,f.definitelyAssigned});
        c.unassigned=intersectSets({t.definitelyUnassigned,f.definitelyUnassigned});
        c.reachable=t.reachable||f.reachable;
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
        if (s.finalVariable && c.unassigned.find(s.variable) == c.unassigned.end() &&
            c.assigned.find(s.variable) != c.assigned.end()) {
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
        JavaFlowFacts ct,cf; analyzeBoolean(c,s.condition,ct,cf);
        Context body=c; restoreFacts(body,ct);
        body.breakTargets.push_back(s.label); body.continueTargets.push_back(s.label);
        analyzeList(body,s.children);
        std::vector<std::set<std::string>> exits;
        if (cf.reachable) exits.push_back(cf.definitelyAssigned);
        for (const auto& b: body.breakFacts)
            if (b.label.empty() || b.label==s.label) exits.push_back(b.facts.definitelyAssigned);
        if (!exits.empty()) {
            c.assigned=intersectSets(exits); c.unassigned=cf.definitelyUnassigned; c.reachable=true;
        } else c.reachable=false;
        c.diagnostics.insert(c.diagnostics.end(),body.diagnostics.begin(),body.diagnostics.end());
        return;
    }
    case JavaFlowStmtKind::Do: {
        Context body=c; body.breakTargets.push_back(s.label); body.continueTargets.push_back(s.label);
        analyzeList(body,s.children);
        JavaFlowFacts ct,cf; analyzeBoolean(body,s.condition,ct,cf);
        std::vector<std::set<std::string>> exits;
        if (cf.reachable) exits.push_back(cf.definitelyAssigned);
        for (const auto& b: body.breakFacts)
            if (b.label.empty() || b.label==s.label) exits.push_back(b.facts.definitelyAssigned);
        if (!exits.empty()) { c.assigned=intersectSets(exits); c.unassigned=body.unassigned; c.reachable=true; }
        else c.reachable=false;
        c.diagnostics.insert(c.diagnostics.end(),body.diagnostics.begin(),body.diagnostics.end());
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
    case JavaFlowStmtKind::Break: {
        bool valid=s.label.empty() || std::find(c.breakTargets.begin(),c.breakTargets.end(),s.label)!=c.breakTargets.end();
        if (!valid) diagnostic(c,JavaFlowDiagnosticKind::InvalidBreak,s.label,"break target label is not in scope");
        c.breakFacts.push_back({factsOf(c),s.label}); c.reachable=false; return;
    }
    case JavaFlowStmtKind::Continue: {
        bool valid=s.label.empty() || std::find(c.continueTargets.begin(),c.continueTargets.end(),s.label)!=c.continueTargets.end();
        if (!valid) diagnostic(c,JavaFlowDiagnosticKind::InvalidContinue,s.label,"continue target label is not a loop in scope");
        c.continueFacts.push_back({factsOf(c),s.label}); c.reachable=false; return;
    }
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
    case JavaFlowStmtKind::Labeled: {
        if (s.children.empty()) return;
        Context labeled=c;
        const auto kind=s.children.front().kind;
        if (kind==JavaFlowStmtKind::While || kind==JavaFlowStmtKind::Do || kind==JavaFlowStmtKind::For) {
            auto target=s.children.front(); target.label=s.label; analyzeStmt(labeled,target);
        } else {
            labeled.breakTargets.push_back(s.label); analyzeList(labeled,s.children);
        }
        c=labeled; return;
    }
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
