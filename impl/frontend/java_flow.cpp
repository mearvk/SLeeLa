#include "java_flow.h"
#include <algorithm>
#include <utility>

namespace sleela {
namespace {

struct FlowState {
    std::set<std::string> assigned;
    std::set<std::string> unassigned;
    bool reachable=true;
    std::vector<JavaFlowDiagnostic> diagnostics;
};
enum class ExitKind { Break, Continue };
struct AbruptExit { ExitKind kind; std::string label; FlowState state; };
struct Target { std::string label; bool acceptsBreak; bool acceptsContinue; };
struct Context { std::vector<Target> targets; std::vector<AbruptExit> exits; };

struct BoolFlow { FlowState whenTrue; FlowState whenFalse; };

static std::set<std::string> meet(const std::vector<std::set<std::string>>& xs) {
    if(xs.empty()) return {};
    std::set<std::string> out=xs.front();
    for(std::size_t i=1;i<xs.size();++i)
        for(auto it=out.begin();it!=out.end();)
            if(xs[i].find(*it)==xs[i].end()) it=out.erase(it); else ++it;
    return out;
}
static FlowState joinNormal(const std::vector<FlowState>& states) {
    FlowState out; std::vector<std::set<std::string>> a,u;
    for(const auto& s:states) if(s.reachable){a.push_back(s.assigned);u.push_back(s.unassigned);}
    if(a.empty()){out.reachable=false;return out;}
    out.assigned=meet(a); out.unassigned=meet(u);
    for(const auto& s:states) out.diagnostics.insert(out.diagnostics.end(),s.diagnostics.begin(),s.diagnostics.end());
    return out;
}
static void diag(FlowState& s,JavaFlowDiagnosticKind k,const std::string& v,const std::string& m){
    s.diagnostics.push_back({k,v,m,true});
}
static void mergeDiagnostics(FlowState& dst,const FlowState& src){
    dst.diagnostics.insert(dst.diagnostics.end(),src.diagnostics.begin(),src.diagnostics.end());
}
static void consumeExits(Context& c,std::size_t start,const std::string& label,ExitKind kind,
                         std::vector<FlowState>& matched){
    std::vector<AbruptExit> keep;
    for(std::size_t i=0;i<c.exits.size();++i){
        const auto& e=c.exits[i];
        if(i>=start && e.kind==kind && (e.label.empty() || e.label==label)) matched.push_back(e.state);
        else keep.push_back(e);
    }
    c.exits.swap(keep);
}
static void readExpr(FlowState&,const JavaFlowExpr&);
static BoolFlow boolExpr(FlowState,const JavaFlowExpr&);
static void writeVar(FlowState& s,const std::string& n,bool isFinal=false){
    if(!s.reachable)return;
    if(isFinal && s.unassigned.find(n)==s.unassigned.end()){
        diag(s,JavaFlowDiagnosticKind::FinalReassignment,n,"final variable is not definitely unassigned before assignment");
        return;
    }
    s.assigned.insert(n); s.unassigned.erase(n);
}
static void readExpr(FlowState& s,const JavaFlowExpr& e){
    if(!s.reachable)return;
    if(e.kind==JavaFlowExprKind::Variable){
        if(s.assigned.find(e.name)==s.assigned.end())
            diag(s,JavaFlowDiagnosticKind::UseBeforeAssignment,e.name,"variable is not definitely assigned before this read");
        return;
    }
    if(e.kind==JavaFlowExprKind::Assignment){
        if(e.children.size()>1)readExpr(s,e.children[1]);
        if(!e.children.empty()&&e.children[0].kind==JavaFlowExprKind::Variable)writeVar(s,e.children[0].name);
        else if(!e.children.empty())readExpr(s,e.children[0]);
        return;
    }
    if(e.kind==JavaFlowExprKind::CompoundAssignment){
        if(!e.children.empty())readExpr(s,e.children[0]);
        if(e.children.size()>1)readExpr(s,e.children[1]);
        if(!e.children.empty()&&e.children[0].kind==JavaFlowExprKind::Variable)writeVar(s,e.children[0].name);
        return;
    }
    if(e.kind==JavaFlowExprKind::Unary&&(e.op=="++"||e.op=="--")){
        if(!e.children.empty())readExpr(s,e.children[0]);
        if(!e.children.empty()&&e.children[0].kind==JavaFlowExprKind::Variable)writeVar(s,e.children[0].name);
        return;
    }
    if(e.kind==JavaFlowExprKind::Conditional&&e.children.size()>=3){
        auto b=boolExpr(s,e.children[0]); readExpr(b.whenTrue,e.children[1]); readExpr(b.whenFalse,e.children[2]);
        s=joinNormal({b.whenTrue,b.whenFalse}); return;
    }
    if(e.kind==JavaFlowExprKind::Binary&&(e.op=="&&"||e.op=="||")&&e.children.size()>=2){
        auto b=boolExpr(s,e); s=joinNormal({b.whenTrue,b.whenFalse}); return;
    }
    if(e.kind==JavaFlowExprKind::Unary&&e.op=="!"&&!e.children.empty()){
        auto b=boolExpr(s,e); s=joinNormal({b.whenTrue,b.whenFalse}); return;
    }
    for(const auto& child:e.children)readExpr(s,child);
}
static BoolFlow boolExpr(FlowState s,const JavaFlowExpr& e){
    BoolFlow out{s,s};
    if(!s.reachable){out.whenTrue.reachable=false;out.whenFalse.reachable=false;return out;}
    if(e.kind==JavaFlowExprKind::Literal&&e.op=="true"){out.whenFalse.reachable=false;return out;}
    if(e.kind==JavaFlowExprKind::Literal&&e.op=="false"){out.whenTrue.reachable=false;return out;}
    if(e.kind==JavaFlowExprKind::Unary&&e.op=="!"&&!e.children.empty()){
        auto b=boolExpr(s,e.children[0]); std::swap(b.whenTrue,b.whenFalse); return b;
    }
    if(e.kind==JavaFlowExprKind::Binary&&e.children.size()>=2&&e.op=="&&"){
        auto l=boolExpr(s,e.children[0]); auto r=boolExpr(l.whenTrue,e.children[1]);
        out.whenTrue=r.whenTrue; out.whenFalse=joinNormal({l.whenFalse,r.whenFalse}); return out;
    }
    if(e.kind==JavaFlowExprKind::Binary&&e.children.size()>=2&&e.op=="||"){
        auto l=boolExpr(s,e.children[0]); auto r=boolExpr(l.whenFalse,e.children[1]);
        out.whenFalse=r.whenFalse; out.whenTrue=joinNormal({l.whenTrue,r.whenTrue}); return out;
    }
    if(e.kind==JavaFlowExprKind::Conditional&&e.children.size()>=3){
        auto c=boolExpr(s,e.children[0]); auto t=boolExpr(c.whenTrue,e.children[1]); auto f=boolExpr(c.whenFalse,e.children[2]);
        out.whenTrue=joinNormal({t.whenTrue,f.whenTrue}); out.whenFalse=joinNormal({t.whenFalse,f.whenFalse}); return out;
    }
    readExpr(out.whenTrue,e); out.whenFalse=out.whenTrue; return out;
}
static void analyzeList(Context&,FlowState&,const std::vector<JavaFlowStmt>&);
static void analyzeStmt(Context& c,FlowState& s,const JavaFlowStmt& st){
    if(!s.reachable){diag(s,JavaFlowDiagnosticKind::UnreachableStatement,"","statement is unreachable");return;}
    switch(st.kind){
    case JavaFlowStmtKind::Empty:return;
    case JavaFlowStmtKind::Block:analyzeList(c,s,st.children);return;
    case JavaFlowStmtKind::LocalDeclaration:
        if(st.hasInitializer){readExpr(s,st.expression);writeVar(s,st.variable,st.finalVariable);}
        else{s.unassigned.insert(st.variable);s.assigned.erase(st.variable);} return;
    case JavaFlowStmtKind::Expression:readExpr(s,st.expression);return;
    case JavaFlowStmtKind::Assignment:
        if(st.expression.kind!=JavaFlowExprKind::Literal)readExpr(s,st.expression);
        writeVar(s,st.variable,st.finalVariable);return;
    case JavaFlowStmtKind::If:{
        auto b=boolExpr(s,st.condition); analyzeList(c,b.whenTrue,st.children); analyzeList(c,b.whenFalse,st.elseChildren);
        if(st.conditionConstant){s=st.conditionValue?b.whenTrue:b.whenFalse;return;}
        s=joinNormal({b.whenTrue,b.whenFalse});return;
    }
    case JavaFlowStmtKind::While:{
        const std::size_t start=c.exits.size(); auto cond=boolExpr(s,st.condition);
        c.targets.push_back({st.label,true,true}); FlowState body=cond.whenTrue; analyzeList(c,body,st.children); c.targets.pop_back();
        std::vector<FlowState> exits{cond.whenFalse}; consumeExits(c,start,st.label,ExitKind::Break,exits);
        std::vector<FlowState> continues; consumeExits(c,start,st.label,ExitKind::Continue,continues);
        s=joinNormal(exits); mergeDiagnostics(s,cond); mergeDiagnostics(s,body); return;
    }
    case JavaFlowStmtKind::Do:{
        const std::size_t start=c.exits.size(); c.targets.push_back({st.label,true,true});
        FlowState body=s; analyzeList(c,body,st.children); c.targets.pop_back();
        auto cond=boolExpr(body,st.condition); std::vector<FlowState> exits{cond.whenFalse}; consumeExits(c,start,st.label,ExitKind::Break,exits);
        std::vector<FlowState> continues; consumeExits(c,start,st.label,ExitKind::Continue,continues);
        s=joinNormal(exits); mergeDiagnostics(s,body); mergeDiagnostics(s,cond); return;
    }
    case JavaFlowStmtKind::For:{
        const std::size_t start=c.exits.size();

        // JLS 16.2.12: initialization completes before the first condition;
        // each normal body/continue path reaches the update part; the
        // condition-false path and matching breaks are the normal exits.
        FlowState init=s;
        for(const auto& statement:st.forInitialization) analyzeStmt(c,init,statement);

        const bool hasCondition =
            st.condition.kind!=JavaFlowExprKind::Literal || !st.condition.op.empty();
        auto cond=hasCondition ? boolExpr(init,st.condition)
                               : BoolFlow{init,init};

        c.targets.push_back({st.label,true,true});
        FlowState body=cond.whenTrue;
        analyzeList(c,body,st.children);

        std::vector<FlowState> continuePaths;
        consumeExits(c,start,st.label,ExitKind::Continue,continuePaths);
        for(const auto& path:continuePaths){
            FlowState updated=path;
            for(const auto& expression:st.forUpdate) readExpr(updated,expression);
            if(updated.reachable) {
                auto next=boolExpr(updated,st.condition);
                (void)next;
            }
        }

        // A normal body completion also reaches the update part.
        if(body.reachable){
            FlowState updated=body;
            for(const auto& expression:st.forUpdate) readExpr(updated,expression);
            // The update establishes a possible subsequent iteration; it is
            // not itself a normal exit from the for statement.
            if(updated.reachable) {
                auto next=boolExpr(updated,st.condition);
                (void)next;
            }
        }
        c.targets.pop_back();

        std::vector<FlowState> exits;
        if(hasCondition) exits.push_back(cond.whenFalse);
        consumeExits(c,start,st.label,ExitKind::Break,exits);
        // With no condition, only a matching break can complete the loop.
        s=joinNormal(exits);
        mergeDiagnostics(s,init); mergeDiagnostics(s,cond); mergeDiagnostics(s,body);
        return;
    }
    case JavaFlowStmtKind::Switch:{
        readExpr(s,st.expression); std::vector<FlowState> exits;
        for(const auto& branch:st.switchCases){FlowState b=s;analyzeList(c,b,branch);if(b.reachable)exits.push_back(b);mergeDiagnostics(s,b);}
        exits.push_back(s); s=joinNormal(exits); return;
    }
    case JavaFlowStmtKind::Break:{
        bool ok=false; for(auto it=c.targets.rbegin();it!=c.targets.rend();++it)if(st.label.empty()||it->label==st.label){ok=it->acceptsBreak;break;}
        if(!ok)diag(s,JavaFlowDiagnosticKind::InvalidBreak,st.label,"break target is not valid");
        else c.exits.push_back({ExitKind::Break,st.label,s}); s.reachable=false; return;
    }
    case JavaFlowStmtKind::Continue:{
        bool ok=false; for(auto it=c.targets.rbegin();it!=c.targets.rend();++it)if(st.label.empty()||it->label==st.label){ok=it->acceptsContinue;break;}
        if(!ok)diag(s,JavaFlowDiagnosticKind::InvalidContinue,st.label,"continue target is not a loop");
        else c.exits.push_back({ExitKind::Continue,st.label,s}); s.reachable=false; return;
    }
    case JavaFlowStmtKind::Return:
    case JavaFlowStmtKind::Throw:if(st.expression.kind!=JavaFlowExprKind::Literal)readExpr(s,st.expression);s.reachable=false;return;
    case JavaFlowStmtKind::Yield:readExpr(s,st.expression);s.reachable=false;return;
    case JavaFlowStmtKind::Assert:readExpr(s,st.condition);return;
    case JavaFlowStmtKind::Synchronized:readExpr(s,st.condition);analyzeList(c,s,st.children);return;
    case JavaFlowStmtKind::Try:{
        FlowState tr=s;analyzeList(c,tr,st.children);std::vector<FlowState> exits;if(tr.reachable)exits.push_back(tr);
        for(const auto& cb:st.catchBlocks){FlowState cc=s;analyzeList(c,cc,cb);if(cc.reachable)exits.push_back(cc);mergeDiagnostics(s,cc);}
        s=exits.empty()?FlowState{}:joinNormal(exits);mergeDiagnostics(s,tr);
        if(!st.finallyBlock.empty())analyzeList(c,s,st.finallyBlock);return;
    }
    case JavaFlowStmtKind::Labeled:{
        const std::size_t start=c.exits.size();c.targets.push_back({st.label,true,false});FlowState body=s;analyzeList(c,body,st.children);c.targets.pop_back();
        std::vector<FlowState> exits;if(body.reachable)exits.push_back(body);consumeExits(c,start,st.label,ExitKind::Break,exits);
        s=joinNormal(exits);mergeDiagnostics(s,body);return;
    }
    }
}
static void analyzeList(Context& c,FlowState& s,const std::vector<JavaFlowStmt>& ss){
    for(const auto& st:ss){if(!s.reachable)break;analyzeStmt(c,s,st);}
}
} // namespace

JavaFlowResult analyzeJavaFlow(const std::vector<JavaFlowStmt>& statements,
 const std::set<std::string>& initiallyAssigned,const std::set<std::string>& initiallyUnassigned){
    Context c;FlowState s;s.assigned=initiallyAssigned;s.unassigned=initiallyUnassigned;analyzeList(c,s,statements);
    return {JavaFlowFacts{s.assigned,s.unassigned,s.reachable},std::move(s.diagnostics)};
}
const char* javaFlowDiagnosticName(JavaFlowDiagnosticKind k){
    switch(k){case JavaFlowDiagnosticKind::UseBeforeAssignment:return "use-before-assignment";case JavaFlowDiagnosticKind::FinalReassignment:return "final-reassignment";case JavaFlowDiagnosticKind::UnreachableStatement:return "unreachable-statement";case JavaFlowDiagnosticKind::InvalidBreak:return "invalid-break";case JavaFlowDiagnosticKind::InvalidContinue:return "invalid-continue";}
    return "unknown";
}
} // namespace sleela
