// ===========================================================================
// java_flow.cpp -- Java source-level definite-assignment/reachability model.
// ===========================================================================
// Implements the 0.3.21 directional-flow foundation:
// true/false expression states, abrupt exits, structural break/continue
// targets, and conservative normal-path joins.
#include "java_flow.h"
#include <algorithm>
#include <utility>

namespace sleela {
namespace {

struct FlowState {
    std::set<std::string> assigned;
    std::set<std::string> unassigned;
    bool reachable = true;
    std::vector<JavaFlowDiagnostic> diagnostics;
};

enum class ExitKind { Break, Continue };

struct AbruptExit {
    ExitKind kind;
    std::string label;
    FlowState state;
};

struct Target {
    std::string label;
    bool acceptsBreak = true;
    bool acceptsContinue = false;
    std::size_t depth = 0;
};

struct Context {
    std::vector<Target> targets;
    std::vector<AbruptExit> exits;
};

struct BoolFlow {
    FlowState whenTrue;
    FlowState whenFalse;
};

static std::set<std::string> meet(const std::vector<std::set<std::string>>& xs) {
    if (xs.empty()) return {};
    std::set<std::string> out = xs.front();
    for (std::size_t i=1; i<xs.size(); ++i) {
        for (auto it=out.begin(); it!=out.end();) {
            if (xs[i].find(*it)==xs[i].end()) it=out.erase(it);
            else ++it;
        }
    }
    return out;
}

static FlowState joinNormal(const std::vector<FlowState>& states) {
    FlowState out;
    std::vector<std::set<std::string>> a,u;
    for (const auto& s:states) if (s.reachable) {
        a.push_back(s.assigned); u.push_back(s.unassigned);
    }
    if (a.empty()) { out.reachable=false; return out; }
    out.assigned=meet(a); out.unassigned=meet(u); out.reachable=true;
    for (const auto& s:states)
        out.diagnostics.insert(out.diagnostics.end(),s.diagnostics.begin(),s.diagnostics.end());
    return out;
}

static void addDiag(FlowState& s, JavaFlowDiagnosticKind k,
                    const std::string& v, const std::string& m) {
    s.diagnostics.push_back({k,v,m,true});
}

static FlowState clone(const FlowState& s) { return s; }

static void readExpr(FlowState& s, const JavaFlowExpr& e);
static BoolFlow boolExpr(FlowState s, const JavaFlowExpr& e);
static void writeVar(FlowState& s, const std::string& name, bool isFinal=false) {
    if (!s.reachable) return;
    if (isFinal && s.unassigned.find(name)==s.unassigned.end()) {
        addDiag(s,JavaFlowDiagnosticKind::FinalReassignment,name,
                "final variable is not definitely unassigned before assignment");
        return;
    }
    s.assigned.insert(name);
    s.unassigned.erase(name);
}

static void readExpr(FlowState& s, const JavaFlowExpr& e) {
    if (!s.reachable) return;
    if (e.kind==JavaFlowExprKind::Variable) {
        if (s.assigned.find(e.name)==s.assigned.end())
            addDiag(s,JavaFlowDiagnosticKind::UseBeforeAssignment,e.name,
                    "variable is not definitely assigned before this read");
        return;
    }
    if (e.kind==JavaFlowExprKind::Assignment) {
        if (e.children.size()>1) readExpr(s,e.children[1]);
        if (!e.children.empty() && e.children[0].kind==JavaFlowExprKind::Variable)
            writeVar(s,e.children[0].name);
        else if (!e.children.empty()) readExpr(s,e.children[0]);
        return;
    }
    if (e.kind==JavaFlowExprKind::CompoundAssignment) {
        if (!e.children.empty()) readExpr(s,e.children[0]);
        if (e.children.size()>1) readExpr(s,e.children[1]);
        if (!e.children.empty() && e.children[0].kind==JavaFlowExprKind::Variable)
            writeVar(s,e.children[0].name);
        return;
    }
    if (e.kind==JavaFlowExprKind::Unary && (e.op=="++" || e.op=="--")) {
        if (!e.children.empty()) readExpr(s,e.children[0]);
        if (!e.children.empty() && e.children[0].kind==JavaFlowExprKind::Variable)
            writeVar(s,e.children[0].name);
        return;
    }
    if (e.kind==JavaFlowExprKind::Conditional && e.children.size()>=3) {
        auto b=boolExpr(s,e.children[0]);
        readExpr(b.whenTrue,e.children[1]);
        readExpr(b.whenFalse,e.children[2]);
        auto j=joinNormal({b.whenTrue,b.whenFalse});
        s=j;
        return;
    }
    if (e.kind==JavaFlowExprKind::Binary && (e.op=="&&" || e.op=="||") &&
        e.children.size()>=2) {
        auto b=boolExpr(s,e);
        s=joinNormal({b.whenTrue,b.whenFalse});
        return;
    }
    if (e.kind==JavaFlowExprKind::Unary && e.op=="!" && !e.children.empty()) {
        auto b=boolExpr(s,e.children[0]);
        s=joinNormal({b.whenTrue,b.whenFalse});
        return;
    }
    for (const auto& c:e.children) readExpr(s,c);
}

static BoolFlow boolExpr(FlowState s, const JavaFlowExpr& e) {
    BoolFlow out{clone(s),clone(s)};
    if (!s.reachable) { out.whenTrue.reachable=false; out.whenFalse.reachable=false; return out; }

    if (e.kind==JavaFlowExprKind::Literal && e.op=="true") {
        out.whenFalse.reachable=false; return out;
    }
    if (e.kind==JavaFlowExprKind::Literal && e.op=="false") {
        out.whenTrue.reachable=false; return out;
    }
    if (e.kind==JavaFlowExprKind::Unary && e.op=="!" && !e.children.empty()) {
        auto b=boolExpr(s,e.children[0]);
        std::swap(b.whenTrue,b.whenFalse); return b;
    }
    if (e.kind==JavaFlowExprKind::Binary && e.children.size()>=2 &&
        e.op=="&&") {
        auto l=boolExpr(s,e.children[0]);
        auto r=boolExpr(l.whenTrue,e.children[1]);
        out.whenTrue=r.whenTrue;
        out.whenFalse=joinNormal({l.whenFalse,r.whenFalse});
        return out;
    }
    if (e.kind==JavaFlowExprKind::Binary && e.children.size()>=2 &&
        e.op=="||") {
        auto l=boolExpr(s,e.children[0]);
        auto r=boolExpr(l.whenFalse,e.children[1]);
        out.whenFalse=r.whenFalse;
        out.whenTrue=joinNormal({l.whenTrue,r.whenTrue});
        return out;
    }
    if (e.kind==JavaFlowExprKind::Conditional && e.children.size()>=3) {
        auto c=boolExpr(s,e.children[0]);
        auto t=boolExpr(c.whenTrue,e.children[1]);
        auto f=boolExpr(c.whenFalse,e.children[2]);
        out.whenTrue=joinNormal({t.whenTrue,f.whenTrue});
        out.whenFalse=joinNormal({t.whenFalse,f.whenFalse});
        return out;
    }
    readExpr(out.whenTrue,e);
    out.whenFalse=out.whenTrue;
    return out;
}

static void analyzeList(Context& ctx, FlowState& s, const std::vector<JavaFlowStmt>& ss);

static void analyzeStmt(Context& ctx, FlowState& s, const JavaFlowStmt& st) {
    if (!s.reachable) {
        addDiag(s,JavaFlowDiagnosticKind::UnreachableStatement,"","statement is unreachable");
        return;
    }
    switch(st.kind) {
    case JavaFlowStmtKind::Empty: return;
    case JavaFlowStmtKind::Block:
        analyzeList(ctx,s,st.children); return;
    case JavaFlowStmtKind::LocalDeclaration:
        if (st.hasInitializer) { readExpr(s,st.expression); writeVar(s,st.variable); }
        else { s.unassigned.insert(st.variable); s.assigned.erase(st.variable); }
        return;
    case JavaFlowStmtKind::Expression: readExpr(s,st.expression); return;
    case JavaFlowStmtKind::Assignment:
        if (st.expression.kind!=JavaFlowExprKind::Literal) readExpr(s,st.expression);
        writeVar(s,st.variable,st.finalVariable); return;
    case JavaFlowStmtKind::If: {
        auto b=boolExpr(s,st.condition);
        analyzeList(ctx,b.whenTrue,st.children);
        analyzeList(ctx,b.whenFalse,st.elseChildren);
        if (st.conditionConstant) {
            s=st.conditionValue?b.whenTrue:b.whenFalse; return;
        }
        s=joinNormal({b.whenTrue,b.whenFalse}); return;
    }
    case JavaFlowStmtKind::While: {
        auto entry=s;
        auto cond=boolExpr(entry,st.condition);
        Target t{st.label,true,true,ctx.targets.size()};
        ctx.targets.push_back(t);
        FlowState body=cond.whenTrue;
        analyzeList(ctx,body,st.children);
        ctx.targets.pop_back();
        std::vector<FlowState> exits;
        exits.push_back(cond.whenFalse);
        for (const auto& e:ctx.exits) if (e.kind==ExitKind::Break &&
            (e.label.empty() || e.label==st.label)) exits.push_back(e.state);
        // The loop is not guaranteed to execute, so only condition-false and
        // matching break paths contribute to normal completion.
        s=joinNormal(exits);
        return;
    }
    case JavaFlowStmtKind::Do: {
        Target t{st.label,true,true,ctx.targets.size()};
        ctx.targets.push_back(t);
        FlowState body=s;
        analyzeList(ctx,body,st.children);
        ctx.targets.pop_back();
        auto cond=boolExpr(body,st.condition);
        std::vector<FlowState> exits{cond.whenFalse};
        for (const auto& e:ctx.exits) if (e.kind==ExitKind::Break &&
            (e.label.empty() || e.label==st.label)) exits.push_back(e.state);
        s=joinNormal(exits); return;
    }
    case JavaFlowStmtKind::For: {
        // The existing fixture encoding uses children[0] as initialization
        // and the remaining children as the body. Condition is evaluated
        // before each iteration; update is represented by expression metadata
        // when supplied by the frontend.
        if (!st.children.empty()) analyzeStmt(ctx,s,st.children.front());
        auto cond=boolExpr(s,st.condition);
        Target t{st.label,true,true,ctx.targets.size()};
        ctx.targets.push_back(t);
        FlowState body=cond.whenTrue;
        if (st.children.size()>1)
            for (std::size_t i=1;i<st.children.size();++i) analyzeStmt(ctx,body,st.children[i]);
        ctx.targets.pop_back();
        std::vector<FlowState> exits{cond.whenFalse};
        for (const auto& e:ctx.exits) if (e.kind==ExitKind::Break &&
            (e.label.empty() || e.label==st.label)) exits.push_back(e.state);
        s=joinNormal(exits); return;
    }
    case JavaFlowStmtKind::Switch: {
        readExpr(s,st.expression);
        std::vector<FlowState> exits;
        for (const auto& branch:st.switchCases) {
            FlowState b=s;
            analyzeList(ctx,b,branch);
            if (b.reachable) exits.push_back(b);
        }
        exits.push_back(s); // no matching case completes normally
        s=joinNormal(exits); return;
    }
    case JavaFlowStmtKind::Break: {
        bool ok=false;
        for (auto it=ctx.targets.rbegin();it!=ctx.targets.rend();++it)
            if (st.label.empty() || it->label==st.label) { ok=it->acceptsBreak; break; }
        if (!ok) addDiag(s,JavaFlowDiagnosticKind::InvalidBreak,st.label,"break target is not valid");
        else ctx.exits.push_back({ExitKind::Break,st.label,s});
        s.reachable=false; return;
    }
    case JavaFlowStmtKind::Continue: {
        bool ok=false;
        for (auto it=ctx.targets.rbegin();it!=ctx.targets.rend();++it)
            if (st.label.empty() || it->label==st.label) { ok=it->acceptsContinue; break; }
        if (!ok) addDiag(s,JavaFlowDiagnosticKind::InvalidContinue,st.label,"continue target is not a loop");
        else ctx.exits.push_back({ExitKind::Continue,st.label,s});
        s.reachable=false; return;
    }
    case JavaFlowStmtKind::Return:
    case JavaFlowStmtKind::Throw:
        if (st.expression.kind!=JavaFlowExprKind::Literal) readExpr(s,st.expression);
        s.reachable=false; return;
    case JavaFlowStmtKind::Yield:
        readExpr(s,st.expression); s.reachable=false; return;
    case JavaFlowStmtKind::Assert: readExpr(s,st.condition); return;
    case JavaFlowStmtKind::Synchronized: readExpr(s,st.condition); analyzeList(ctx,s,st.children); return;
    case JavaFlowStmtKind::Try: {
        std::vector<FlowState> exits;
        FlowState tr=s; analyzeList(ctx,tr,st.children);
        if (tr.reachable) exits.push_back(tr);
        for (const auto& cb:st.catchBlocks) {
            FlowState cc=s; analyzeList(ctx,cc,cb);
            if (cc.reachable) exits.push_back(cc);
        }
        s=exits.empty()?FlowState{}:joinNormal(exits);
        if (!st.finallyBlock.empty()) analyzeList(ctx,s,st.finallyBlock);
        return;
    }
    case JavaFlowStmtKind::Labeled: {
        Target t{st.label,true,false,ctx.targets.size()};
        ctx.targets.push_back(t);
        analyzeList(ctx,s,st.children);
        ctx.targets.pop_back();
        return;
    }
    }
}

static void analyzeList(Context& ctx, FlowState& s, const std::vector<JavaFlowStmt>& ss) {
    for (const auto& st:ss) {
        if (!s.reachable) break;
        analyzeStmt(ctx,s,st);
    }
}

} // namespace

JavaFlowResult analyzeJavaFlow(const std::vector<JavaFlowStmt>& statements,
                               const std::set<std::string>& initiallyAssigned,
                               const std::set<std::string>& initiallyUnassigned) {
    Context ctx;
    FlowState s;
    s.assigned=initiallyAssigned;
    s.unassigned=initiallyUnassigned;
    analyzeList(ctx,s,statements);
    return {JavaFlowFacts{s.assigned,s.unassigned,s.reachable},std::move(s.diagnostics)};
}

const char* javaFlowDiagnosticName(JavaFlowDiagnosticKind k) {
    switch(k) {
    case JavaFlowDiagnosticKind::UseBeforeAssignment: return "use-before-assignment";
    case JavaFlowDiagnosticKind::FinalReassignment: return "final-reassignment";
    case JavaFlowDiagnosticKind::UnreachableStatement: return "unreachable-statement";
    case JavaFlowDiagnosticKind::InvalidBreak: return "invalid-break";
    case JavaFlowDiagnosticKind::InvalidContinue: return "invalid-continue";
    }
    return "unknown";
}

} // namespace sleela
