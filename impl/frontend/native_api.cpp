#include "native_api.h"
#include <stdexcept>
#include <set>
#include <map>
#include <cmath>

namespace sleela { namespace native {

bool isModuleAvailable(const std::string& module) {
    static const std::set<std::string> modules = {
        "math", "physics", "economics", "excel", "json", "crypto", "net"
    };
    return modules.count(module) != 0;
}

void validateImports(const Program& program) {
    std::set<std::string> seen;
    for (const auto& module : program.imports) {
        if (!isModuleAvailable(module))
            throw std::runtime_error("Semantic error: unknown native module '" + module + "'");
        if (!seen.insert(module).second)
            throw std::runtime_error("Semantic error: duplicate import '" + module + "'");
    }
}

namespace {

ExprP V(const std::string& n) { return std::make_unique<VarExpr>(n); }
ExprP I(long long n) { return std::make_unique<IntLit>(n); }
ExprP D(double n) { return std::make_unique<DoubleLit>(n); }
ExprP B(bool n) { return std::make_unique<BoolLit>(n); }
ExprP Bin(const char* op, ExprP a, ExprP b) {
    return std::make_unique<Binary>(op, std::move(a), std::move(b));
}
ExprP Call0(const std::string& n) { return std::make_unique<Call>(n); }
ExprP Call1(const std::string& n, ExprP a) {
    auto c = std::make_unique<Call>(n); c->args.push_back(std::move(a)); return c;
}
ExprP Call2(const std::string& n, ExprP a, ExprP b) {
    auto c = std::make_unique<Call>(n); c->args.push_back(std::move(a)); c->args.push_back(std::move(b)); return c;
}
ExprP Call3(const std::string& n, ExprP a, ExprP b, ExprP c0) {
    auto c = std::make_unique<Call>(n); c->args.push_back(std::move(a)); c->args.push_back(std::move(b)); c->args.push_back(std::move(c0)); return c;
}
ExprP Neg(ExprP a) { return std::make_unique<Unary>("-", std::move(a)); }
StmtP Ret(ExprP e) { auto s=std::make_unique<ReturnStmt>(); s->value=std::move(e); return s; }
StmtP Decl(const std::string& t,const std::string& n,ExprP e) { auto s=std::make_unique<VarDecl>(); s->type=t;s->name=n;s->init=std::move(e);return s; }
StmtP Set(const std::string& n,ExprP e) { auto s=std::make_unique<Assign>();s->name=n;s->value=std::move(e);return s; }
StmtP ExprS(ExprP e) { auto s=std::make_unique<ExprStmt>();s->expr=std::move(e);return s; }
StmtP While(ExprP cond,std::unique_ptr<Block> body) { auto s=std::make_unique<WhileStmt>();s->cond=std::move(cond);s->body=std::move(body);return s; }

Method Method0(const std::string& ret,const std::string& name) {
    Method m;m.retType=ret;m.name=name;m.body=std::make_unique<Block>();return m;
}
Method Method1(const std::string& ret,const std::string& name,const std::string& t,const std::string& p) {
    Method m=Method0(ret,name);m.params.push_back({t,p});return m;
}
Method Method2(const std::string& ret,const std::string& name,const std::string& t1,const std::string& p1,const std::string& t2,const std::string& p2) {
    Method m=Method1(ret,name,t1,p1);m.params.push_back({t2,p2});return m;
}
Method Method3(const std::string& ret,const std::string& name,const std::string& t1,const std::string& p1,const std::string& t2,const std::string& p2,const std::string& t3,const std::string& p3) {
    Method m=Method2(ret,name,t1,p1,t2,p2);m.params.push_back({t3,p3});return m;
}

void addMath(Program& p) {
    ClassDecl c;c.name="__NativeMath";
    auto m=Method0("double","__native_math_pi");m.body->stmts.push_back(Ret(D(3.141592653589793238462643383279502884)));c.methods.push_back(std::move(m));
    m=Method0("double","__native_math_e");m.body->stmts.push_back(Ret(D(2.7182818284590452353602874713526625)));c.methods.push_back(std::move(m));
    m=Method0("double","__native_math_tau");m.body->stmts.push_back(Ret(D(6.283185307179586476925286766559)));c.methods.push_back(std::move(m));

    m=Method1("double","__native_math_abs","double","x");
    {auto b=std::make_unique<Block>();b->stmts.push_back(Ret(Neg(V("x"))));auto i=std::make_unique<IfStmt>();i->cond=Bin("<",V("x"),D(0));i->thenS=std::move(b);i->elseS=Ret(V("x"));m.body->stmts.push_back(std::move(i));} c.methods.push_back(std::move(m));
    m=Method1("double","__native_math_sign","double","x");
    {auto i1=std::make_unique<IfStmt>();i1->cond=Bin(">",V("x"),D(0));i1->thenS=Ret(D(1));auto i2=std::make_unique<IfStmt>();i2->cond=Bin("<",V("x"),D(0));i2->thenS=Ret(D(-1));i2->elseS=Ret(D(0));i1->elseS=std::move(i2);m.body->stmts.push_back(std::move(i1));}c.methods.push_back(std::move(m));
    m=Method2("double","__native_math_min","double","a","double","b");{auto i=std::make_unique<IfStmt>();i->cond=Bin("<",V("a"),V("b"));i->thenS=Ret(V("a"));i->elseS=Ret(V("b"));m.body->stmts.push_back(std::move(i));}c.methods.push_back(std::move(m));
    m=Method2("double","__native_math_max","double","a","double","b");{auto i=std::make_unique<IfStmt>();i->cond=Bin(">",V("a"),V("b"));i->thenS=Ret(V("a"));i->elseS=Ret(V("b"));m.body->stmts.push_back(std::move(i));}c.methods.push_back(std::move(m));
    m=Method3("double","__native_math_clamp","double","x","double","lo","double","hi");m.body->stmts.push_back(Ret(Call2("__native_math_min",Call1("__native_math_max",V("x"),V("lo")),V("hi"))));c.methods.push_back(std::move(m));

    m=Method1("double","__native_math_sqrt","double","x");
    m.body->stmts.push_back(Decl("double","g",Bin("+",Bin("/",V("x"),D(2)),D(1))));
    m.body->stmts.push_back(Decl("int","i",I(0)));
    {auto b=std::make_unique<Block>();b->stmts.push_back(Set("g",Bin("/",Bin("+",V("g"),Bin("/",V("x"),V("g"))),D(2))));b->stmts.push_back(Set("i",Bin("+",V("i"),I(1))));m.body->stmts.push_back(While(Bin("<",V("i"),I(12)),std::move(b)));}
    m.body->stmts.push_back(Ret(V("g")));c.methods.push_back(std::move(m));

    m=Method1("double","__native_math_exp","double","x");
    m.body->stmts.push_back(Decl("double","term",D(1)));m.body->stmts.push_back(Decl("double","sum",D(1)));m.body->stmts.push_back(Decl("int","i",I(1)));
    {auto b=std::make_unique<Block>();b->stmts.push_back(Set("term",Bin("/",Bin("*",V("term"),V("x")),V("i"))));b->stmts.push_back(Set("sum",Bin("+",V("sum"),V("term"))));b->stmts.push_back(Set("i",Bin("+",V("i"),I(1))));m.body->stmts.push_back(While(Bin("<",V("i"),I(30)),std::move(b)));}m.body->stmts.push_back(Ret(V("sum")));c.methods.push_back(std::move(m));

    m=Method1("double","__native_math_log","double","x");
    m.body->stmts.push_back(Decl("double","z",Bin("/",Bin("-",V("x"),D(1)),Bin("+",V("x"),D(1)))));m.body->stmts.push_back(Decl("double","z2",Bin("*",V("z"),V("z"))));m.body->stmts.push_back(Decl("double","term",V("z")));m.body->stmts.push_back(Decl("double","sum",V("z")));m.body->stmts.push_back(Decl("int","i",I(1)));
    {auto b=std::make_unique<Block>();b->stmts.push_back(Set("term",Bin("*",V("term"),V("z2"))));b->stmts.push_back(Set("sum",Bin("+",V("sum"),Bin("/",V("term"),Bin("+",Bin("*",I(2),V("i")),I(1))))));b->stmts.push_back(Set("i",Bin("+",V("i"),I(1))));m.body->stmts.push_back(While(Bin("<",V("i"),I(24)),std::move(b)));}m.body->stmts.push_back(Ret(Bin("*",D(2),V("sum"))));c.methods.push_back(std::move(m));

    m=Method2("double","__native_math_pow","double","x","double","y");m.body->stmts.push_back(Ret(Call1("__native_math_exp",Bin("*",V("y"),Call1("__native_math_log",V("x"))))));c.methods.push_back(std::move(m));

    m=Method1("double","__native_math_sin","double","x");m.body->stmts.push_back(Decl("double","term",V("x")));m.body->stmts.push_back(Decl("double","sum",V("x")));m.body->stmts.push_back(Decl("double","xx",Bin("*",V("x"),V("x"))));m.body->stmts.push_back(Decl("int","i",I(1)));
    {auto b=std::make_unique<Block>();b->stmts.push_back(Set("term",Bin("/",Bin("*",V("term"),Bin("-",D(0),V("xx"))),Bin("*",Bin("+",Bin("*",I(2),V("i")),I(1)),Bin("+",Bin("*",I(2),V("i")),I(2))))));b->stmts.push_back(Set("sum",Bin("+",V("sum"),V("term"))));b->stmts.push_back(Set("i",Bin("+",V("i"),I(1))));m.body->stmts.push_back(While(Bin("<",V("i"),I(10)),std::move(b)));}m.body->stmts.push_back(Ret(V("sum")));c.methods.push_back(std::move(m));
    m=Method1("double","__native_math_cos","double","x");m.body->stmts.push_back(Ret(Call1("__native_math_sin",Bin("+",V("x"),D(1.57079632679489661923)))));c.methods.push_back(std::move(m));
    m=Method1("double","__native_math_tan","double","x");m.body->stmts.push_back(Ret(Bin("/",Call1("__native_math_sin",V("x")),Call1("__native_math_cos",V("x")))));c.methods.push_back(std::move(m));
    m=Method2("double","__native_math_hypot","double","a","double","b");m.body->stmts.push_back(Ret(Call1("__native_math_sqrt",Bin("+",Bin("*",V("a"),V("a")),Bin("*",V("b"),V("b"))))));c.methods.push_back(std::move(m));
    m=Method2("double","__native_math_fmod","double","a","double","b");m.body->stmts.push_back(Ret(Bin("-",V("a"),Bin("*",Bin("/",V("a"),V("b")),V("b")))));c.methods.push_back(std::move(m));
    p.classes.push_back(std::move(c));
}

void addPhysics(Program& p) {
    ClassDecl c;c.name="__NativePhysics";
    auto m=Method0("double","__native_physics_C");m.body->stmts.push_back(Ret(D(299792458.0)));c.methods.push_back(std::move(m));
    m=Method0("double","__native_physics_H");m.body->stmts.push_back(Ret(D(6.62607015e-34)));c.methods.push_back(std::move(m));
    m=Method0("double","__native_physics_HBAR");m.body->stmts.push_back(Ret(D(1.054571817e-34)));c.methods.push_back(std::move(m));
    m=Method0("double","__native_physics_E_CHARGE");m.body->stmts.push_back(Ret(D(1.602176634e-19)));c.methods.push_back(std::move(m));
    m=Method0("double","__native_physics_K_B");m.body->stmts.push_back(Ret(D(1.380649e-23)));c.methods.push_back(std::move(m));
    m=Method0("double","__native_physics_G");m.body->stmts.push_back(Ret(D(6.67430e-11)));c.methods.push_back(std::move(m));
    m=Method0("double","__native_physics_G0");m.body->stmts.push_back(Ret(D(9.80665)));c.methods.push_back(std::move(m));
    m=Method0("double","__native_physics_N_A");m.body->stmts.push_back(Ret(D(6.02214076e23)));c.methods.push_back(std::move(m));
    m=Method0("double","__native_physics_R");m.body->stmts.push_back(Ret(D(8.31446261815324)));c.methods.push_back(std::move(m));
    m=Method2("double","__native_physics_velocity","double","dx","double","dt");m.body->stmts.push_back(Ret(Bin("/",V("dx"),V("dt"))));c.methods.push_back(std::move(m));
    m=Method2("double","__native_physics_acceleration","double","dv","double","dt");m.body->stmts.push_back(Ret(Bin("/",V("dv"),V("dt"))));c.methods.push_back(std::move(m));
    m=Method4_PLACEHOLDER;
}

}

void lowerProgram(Program& program) {
    // implemented below
}

}} // namespace sleela::native
