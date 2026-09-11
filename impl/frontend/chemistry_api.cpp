#include "chemistry_api.h"
#include <memory>
#include <set>
#include <stdexcept>
#include <string>

namespace sleela {
namespace chemistry {
namespace {
ExprP V(const std::string& n) { return std::make_unique<VarExpr>(n); }
ExprP D(double n) { return std::make_unique<DoubleLit>(n); }
ExprP B(const char* op, ExprP a, ExprP b) { return std::make_unique<Binary>(op, std::move(a), std::move(b)); }
ExprP C(const std::string& name) { return std::make_unique<Call>(name); }
ExprP C1(const std::string& name, ExprP a) { auto c=C(name); c->args.push_back(std::move(a)); return c; }
ExprP C2(const std::string& name, ExprP a, ExprP b) { auto c=C(name); c->args.push_back(std::move(a)); c->args.push_back(std::move(b)); return c; }
StmtP Ret(ExprP e) { auto s=std::make_unique<ReturnStmt>(); s->value=std::move(e); return s; }
Method M1(const char* r,const char* n,const char* t,const char* p) { Method m; m.retType=r; m.name=n; m.params.push_back({t,p}); m.body=std::make_unique<Block>(); return m; }
Method M2(const char* r,const char* n,const char* t1,const char* p1,const char* t2,const char* p2) { auto m=M1(r,n,t1,p1); m.params.push_back({t2,p2}); return m; }
Method M6(const char* r,const char* n) { Method m; m.retType=r; m.name=n; m.body=std::make_unique<Block>(); m.params={{"double","observation"},{"double","bond"},{"double","valence"},{"double","similarity"},{"double","signal"},{"double","symmetry"}}; return m; }

ExprP evidenceSum() {
    return B("+",
        B("+",B("+",B("*",D(.24),C1("__native_math_clamp",V("observation"))),B("*",D(.20),C1("__native_math_clamp",V("bond")))),B("*",D(.22),C1("__native_math_clamp",V("valence")))),
        B("+",B("*",D(.13),C1("__native_math_clamp",V("similarity"))),B("+",B("*",D(.12),C1("__native_math_clamp",V("signal"))),B("*",D(.09),C1("__native_math_clamp",V("symmetry"))))));
}

void addChemistry(Program& p) {
    ClassDecl c; c.name="__NativeChemistry";

    auto m=M2("double","__native_chemistry_ratio","double","numerator","double","denominator");
    auto z=std::make_unique<IfStmt>(); z->cond=B("==",V("denominator"),D(0)); z->thenS=Ret(D(0)); z->elseS=Ret(B("/",V("numerator"),V("denominator"))); m.body->stmts.push_back(std::move(z)); c.methods.push_back(std::move(m));

    m=M2("double","__native_chemistry_similarity","double","a","double","b");
    m.body->stmts.push_back(Ret(B("-",D(1),C2("__native_chemistry_ratio",C1("__native_math_abs",B("-",V("a"),V("b"))),C2("__native_math_max",C2("__native_math_max",C1("__native_math_abs",V("a")),C1("__native_math_abs",V("b"))),D(1))))));
    c.methods.push_back(std::move(m));

    m=M2("double","__native_chemistry_stochastic","double","seed","double","weight");
    m.body->stmts.push_back(Ret(B("+",D(0.98),B("*",B("*",D(0.04),C2("__native_math_abs",C2("__native_math_fmod",B("*",V("seed"),D(0.6180339887498949)),D(1)))),C1("__native_math_clamp",V("weight"))))));
    c.methods.push_back(std::move(m));

    m=M6("double","__native_chemistry_inference_level");
    m.body->stmts.push_back(Ret(C1("__native_math_clamp",B("*",D(24),evidenceSum()),D(0))));
    c.methods.push_back(std::move(m));

    m=M6("double","__native_chemistry_confidence");
    m.body->stmts.push_back(Ret(C1("__native_math_clamp",evidenceSum(),D(0))));
    c.methods.push_back(std::move(m));

    m=M6("double","__native_chemistry_uncertainty");
    m.body->stmts.push_back(Ret(B("-",D(1),C1("__native_math_clamp",evidenceSum(),D(0)))));
    c.methods.push_back(std::move(m));

    p.classes.push_back(std::move(c));
}

ExprP lowerExpr(ExprP e) {
    if (!e) return e;
    if (auto* call=dynamic_cast<Call*>(e.get())) {
        for (auto& a:call->args) a=lowerExpr(std::move(a));
        const std::string prefix="chemistry.";
        if (call->callee.rfind(prefix,0)==0) {
            const std::string name=call->callee.substr(prefix.size());
            static const std::set<std::string> allowed={"ratio","similarity","stochastic","inference_level","confidence","uncertainty"};
            if (!allowed.count(name)) throw std::runtime_error("Semantic error: unsupported executable chemistry call '"+call->callee+"'");
            call->callee="__native_chemistry_"+name;
        }
        return e;
    }
    if (auto* b=dynamic_cast<Binary*>(e.get())) { b->lhs=lowerExpr(std::move(b->lhs)); b->rhs=lowerExpr(std::move(b->rhs)); }
    else if (auto* u=dynamic_cast<Unary*>(e.get())) u->operand=lowerExpr(std::move(u->operand));
    return e;
}

void lowerStmt(StmtP& s) {
    if (!s) return;
    if (auto* v=dynamic_cast<VarDecl*>(s.get())) v->init=lowerExpr(std::move(v->init));
    else if (auto* a=dynamic_cast<Assign*>(s.get())) a->value=lowerExpr(std::move(a->value));
    else if (auto* e=dynamic_cast<ExprStmt*>(s.get())) e->expr=lowerExpr(std::move(e->expr));
    else if (auto* p=dynamic_cast<PrintStmt*>(s.get())) p->expr=lowerExpr(std::move(p->expr));
    else if (auto* r=dynamic_cast<ReturnStmt*>(s.get())) r->value=lowerExpr(std::move(r->value));
    else if (auto* b=dynamic_cast<Block*>(s.get())) for (auto& x:b->stmts) lowerStmt(x);
    else if (auto* i=dynamic_cast<IfStmt*>(s.get())) { i->cond=lowerExpr(std::move(i->cond)); lowerStmt(i->thenS); lowerStmt(i->elseS); }
    else if (auto* w=dynamic_cast<WhileStmt*>(s.get())) { w->cond=lowerExpr(std::move(w->cond)); lowerStmt(w->body); }
    else if (auto* f=dynamic_cast<ForStmt*>(s.get())) { lowerStmt(f->init); f->cond=lowerExpr(std::move(f->cond)); lowerStmt(f->update); lowerStmt(f->body); }
}

} // namespace

void lowerProgram(Program& program) {
    bool imported=false;
    bool math=false;
    for (const auto& module:program.imports) {
        if (module=="chemistry") imported=true;
        if (module=="math") math=true;
    }
    if (!imported) return;
    if (!math) throw std::runtime_error("Semantic error: chemistry requires 'import math;' for its numerical inference kernels");
    bool helper=false;
    for (const auto& c:program.classes) if (c.name=="__NativeChemistry") helper=true;
    if (!helper) addChemistry(program);
    for (auto& c:program.classes) {
        if (c.name=="__NativeChemistry") continue;
        for (auto& m:c.methods) for (auto& s:m.body->stmts) lowerStmt(s);
        for (auto& f:c.fields) f.init=lowerExpr(std::move(f.init));
    }
}

} // namespace chemistry
} // namespace sleela
