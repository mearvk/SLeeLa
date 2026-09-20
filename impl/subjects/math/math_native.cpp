// math_native.cpp -- Native Math subject library.
//
// Synthesizes the __NativeMath class: constants (pi, e, tau) and executable
// numeric functions (abs, sign, min, max, clamp, sqrt, exp, log, pow, sin,
// cos, tan, hypot, fmod) built from ordinary Sleela Core methods. Math is the
// foundational numeric domain; physics and economics depend on it.
#include "native_add.h"
#include "native_builders.h"

namespace sleela { namespace native {
using namespace sleela::native::builders;

void addMath(Program&p){ClassDecl c;c.name="__NativeMath";auto m=M0("double","__native_math_pi");m.body->stmts.push_back(Ret(D(3.14159265358979323846)));c.methods.push_back(std::move(m));m=M0("double","__native_math_e");m.body->stmts.push_back(Ret(D(2.71828182845904523536)));c.methods.push_back(std::move(m));m=M0("double","__native_math_tau");m.body->stmts.push_back(Ret(D(6.28318530717958647692)));c.methods.push_back(std::move(m));m=M1("double","__native_math_abs","double","x");{auto i=std::make_unique<IfStmt>();i->cond=Bn("<",V("x"),D(0));i->thenS=Ret(Neg(V("x")));i->elseS=Ret(V("x"));m.body->stmts.push_back(std::move(i));}c.methods.push_back(std::move(m));m=M1("double","__native_math_sign","double","x");{auto i=std::make_unique<IfStmt>();i->cond=Bn(">",V("x"),D(0));i->thenS=Ret(D(1));auto j=std::make_unique<IfStmt>();j->cond=Bn("<",V("x"),D(0));j->thenS=Ret(D(-1));j->elseS=Ret(D(0));i->elseS=std::move(j);m.body->stmts.push_back(std::move(i));}c.methods.push_back(std::move(m));m=M2("double","__native_math_min","double","a","double","b");{auto i=std::make_unique<IfStmt>();i->cond=Bn("<",V("a"),V("b"));i->thenS=Ret(V("a"));i->elseS=Ret(V("b"));m.body->stmts.push_back(std::move(i));}c.methods.push_back(std::move(m));m=M2("double","__native_math_max","double","a","double","b");{auto i=std::make_unique<IfStmt>();i->cond=Bn(">",V("a"),V("b"));i->thenS=Ret(V("a"));i->elseS=Ret(V("b"));m.body->stmts.push_back(std::move(i));}c.methods.push_back(std::move(m));m=M3("double","__native_math_clamp","double","x","double","lo","double","hi");m.body->stmts.push_back(Ret(C2("__native_math_min",C2("__native_math_max",V("x"),V("lo")),V("hi"))));c.methods.push_back(std::move(m));m=M1("double","__native_math_sqrt","double","x");
{
  // Range-reduced Newton sqrt: robust for any magnitude. Reduce x into
  // [0.25,4) by factoring out powers of 4 (each divide-by-4 halves the root),
  // run Newton there, then scale the root back. Non-positive x returns 0.
  {auto guard=std::make_unique<IfStmt>();guard->cond=Bn("<=",V("x"),D(0));guard->thenS=Ret(D(0));m.body->stmts.push_back(std::move(guard));}
  m.body->stmts.push_back(Decl("double","s",D(1)));
  {auto b=std::make_unique<Block>();b->stmts.push_back(Set("x",Bn("/",V("x"),D(4))));b->stmts.push_back(Set("s",Bn("*",V("s"),D(2))));m.body->stmts.push_back(Loop(Bn(">=",V("x"),D(4)),std::move(b)));}
  {auto b=std::make_unique<Block>();b->stmts.push_back(Set("x",Bn("*",V("x"),D(4))));b->stmts.push_back(Set("s",Bn("/",V("s"),D(2))));m.body->stmts.push_back(Loop(Bn("<",V("x"),D(0.25)),std::move(b)));}
  m.body->stmts.push_back(Decl("double","g",Bn("+",V("x"),D(0.5))));
  m.body->stmts.push_back(Decl("int","i",I(0)));
  {auto b=std::make_unique<Block>();b->stmts.push_back(Set("g",Bn("/",Bn("+",V("g"),Bn("/",V("x"),V("g"))),D(2))));b->stmts.push_back(Set("i",Bn("+",V("i"),I(1))));m.body->stmts.push_back(Loop(Bn("<",V("i"),I(20)),std::move(b)));}
  m.body->stmts.push_back(Ret(Bn("*",V("g"),V("s"))));
}
c.methods.push_back(std::move(m));m=M1("double","__native_math_exp","double","x");
{
  // exp via the Maclaurin series sum(x^i/i!). That series only converges well
  // for small |x|, so RANGE-REDUCE first: pull integer factors of e out of x
  // (x -> x-1 and multiply the result by e for each) until x lands in [-1, 1],
  // where 30 terms are accurate. Without this, exp(-78) (from pow(10,-34))
  // returned garbage. `scale` accumulates the e^k factor.
  m.body->stmts.push_back(Decl("double","scale",D(1)));
  {auto b=std::make_unique<Block>();b->stmts.push_back(Set("x",Bn("-",V("x"),D(1))));b->stmts.push_back(Set("scale",Bn("*",V("scale"),D(2.71828182845904523536))));m.body->stmts.push_back(Loop(Bn(">",V("x"),D(1)),std::move(b)));}
  {auto b=std::make_unique<Block>();b->stmts.push_back(Set("x",Bn("+",V("x"),D(1))));b->stmts.push_back(Set("scale",Bn("/",V("scale"),D(2.71828182845904523536))));m.body->stmts.push_back(Loop(Bn("<",V("x"),D(-1)),std::move(b)));}
  m.body->stmts.push_back(Decl("double","term",D(1)));m.body->stmts.push_back(Decl("double","sum",D(1)));m.body->stmts.push_back(Decl("int","i",I(1)));
  {auto b=std::make_unique<Block>();b->stmts.push_back(Set("term",Bn("/",Bn("*",V("term"),V("x")),V("i"))));b->stmts.push_back(Set("sum",Bn("+",V("sum"),V("term"))));b->stmts.push_back(Set("i",Bn("+",V("i"),I(1))));m.body->stmts.push_back(Loop(Bn("<",V("i"),I(30)),std::move(b)));}
  m.body->stmts.push_back(Ret(Bn("*",V("sum"),V("scale"))));
}
c.methods.push_back(std::move(m));m=M1("double","__native_math_log","double","x");
{
  // Natural log via the atanh series 2*sum(z^(2i+1)/(2i+1)), z=(x-1)/(x+1).
  // That series only converges quickly for x near 1, so first RANGE-REDUCE:
  // pull factors of e out of x (each one adds/subtracts 1 from the result)
  // until x lands in a band around 1 where the series is accurate. Without
  // this reduction log(1000) came out ~5.05 instead of 6.9078.
  {auto guard=std::make_unique<IfStmt>();guard->cond=Bn("<=",V("x"),D(0));guard->thenS=Ret(D(0));m.body->stmts.push_back(std::move(guard));}
  m.body->stmts.push_back(Decl("double","k",D(0)));
  {auto b=std::make_unique<Block>();b->stmts.push_back(Set("x",Bn("/",V("x"),D(2.71828182845904523536))));b->stmts.push_back(Set("k",Bn("+",V("k"),D(1))));m.body->stmts.push_back(Loop(Bn(">",V("x"),D(1.6487212707001282)),std::move(b)));}
  {auto b=std::make_unique<Block>();b->stmts.push_back(Set("x",Bn("*",V("x"),D(2.71828182845904523536))));b->stmts.push_back(Set("k",Bn("-",V("k"),D(1))));m.body->stmts.push_back(Loop(Bn("<",V("x"),D(0.6065306597126334)),std::move(b)));}
  m.body->stmts.push_back(Decl("double","z",Bn("/",Bn("-",V("x"),D(1)),Bn("+",V("x"),D(1)))));m.body->stmts.push_back(Decl("double","z2",Bn("*",V("z"),V("z"))));m.body->stmts.push_back(Decl("double","term",V("z")));m.body->stmts.push_back(Decl("double","sum",V("z")));m.body->stmts.push_back(Decl("int","i",I(1)));
  {auto b=std::make_unique<Block>();b->stmts.push_back(Set("term",Bn("*",V("term"),V("z2"))));b->stmts.push_back(Set("sum",Bn("+",V("sum"),Bn("/",V("term"),Bn("+",Bn("*",I(2),V("i")),I(1))))));b->stmts.push_back(Set("i",Bn("+",V("i"),I(1))));m.body->stmts.push_back(Loop(Bn("<",V("i"),I(24)),std::move(b)));}
  m.body->stmts.push_back(Ret(Bn("+",V("k"),Bn("*",D(2),V("sum")))));
}
c.methods.push_back(std::move(m));
// log10(x) = ln(x) / ln(10). ln(10) = 2.302585092994046.
m=M1("double","__native_math_log10","double","x");m.body->stmts.push_back(Ret(Bn("/",C1("__native_math_log",V("x")),D(2.302585092994046))));c.methods.push_back(std::move(m));
// trunc(x): drop the fractional part toward zero, using floating modulo.
m=M1("double","__native_math_trunc","double","x");m.body->stmts.push_back(Ret(Bn("-",V("x"),Bn("%",V("x"),D(1)))));c.methods.push_back(std::move(m));
// floor(x): largest integer <= x. trunc rounds toward zero, so a negative x
// with a fractional part needs one subtracted.
m=M1("double","__native_math_floor","double","x");m.body->stmts.push_back(Decl("double","t",Bn("-",V("x"),Bn("%",V("x"),D(1)))));{auto i=std::make_unique<IfStmt>();i->cond=Bn("<",V("x"),V("t"));i->thenS=Ret(Bn("-",V("t"),D(1)));i->elseS=Ret(V("t"));m.body->stmts.push_back(std::move(i));}c.methods.push_back(std::move(m));
// ceil(x): smallest integer >= x.
m=M1("double","__native_math_ceil","double","x");m.body->stmts.push_back(Decl("double","t",Bn("-",V("x"),Bn("%",V("x"),D(1)))));{auto i=std::make_unique<IfStmt>();i->cond=Bn(">",V("x"),V("t"));i->thenS=Ret(Bn("+",V("t"),D(1)));i->elseS=Ret(V("t"));m.body->stmts.push_back(std::move(i));}c.methods.push_back(std::move(m));m=M2("double","__native_math_pow","double","x","double","y");m.body->stmts.push_back(Ret(C1("__native_math_exp",Bn("*",V("y"),C1("__native_math_log",V("x"))))));c.methods.push_back(std::move(m));m=M1("double","__native_math_sin","double","x");
{
  // Range-reduce x into [-pi, pi] before the Maclaurin series. Without this the
  // fixed-term series diverges badly for |x| beyond ~1; e.g. sin(pi/2) must be
  // 1.0. Reduction uses floating modulo by tau, then folds into [-pi, pi].
  m.body->stmts.push_back(Set("x",Bn("%",V("x"),D(6.28318530717958647692))));
  {auto hi=std::make_unique<IfStmt>();hi->cond=Bn(">",V("x"),D(3.14159265358979323846));hi->thenS=Set("x",Bn("-",V("x"),D(6.28318530717958647692)));m.body->stmts.push_back(std::move(hi));}
  {auto lo=std::make_unique<IfStmt>();lo->cond=Bn("<",V("x"),D(-3.14159265358979323846));lo->thenS=Set("x",Bn("+",V("x"),D(6.28318530717958647692)));m.body->stmts.push_back(std::move(lo));}
  m.body->stmts.push_back(Decl("double","term",V("x")));m.body->stmts.push_back(Decl("double","sum",V("x")));m.body->stmts.push_back(Decl("double","xx",Bn("*",V("x"),V("x"))));m.body->stmts.push_back(Decl("int","i",I(1)));
  // term_i = x^(2i+1)/(2i+1)!; step from term_{i-1} multiplies by
  // -x^2 / ((2i)(2i+1)). (The previous denominator (2i+1)(2i+2) was off by one
  // and made sin overshoot, e.g. sin(pi/2) -> 1.27324 instead of 1.)
  {auto b=std::make_unique<Block>();b->stmts.push_back(Set("term",Bn("/",Bn("*",V("term"),Neg(V("xx"))),Bn("*",Bn("*",I(2),V("i")),Bn("+",Bn("*",I(2),V("i")),I(1))))));b->stmts.push_back(Set("sum",Bn("+",V("sum"),V("term"))));b->stmts.push_back(Set("i",Bn("+",V("i"),I(1))));m.body->stmts.push_back(Loop(Bn("<",V("i"),I(14)),std::move(b)));}
  m.body->stmts.push_back(Ret(V("sum")));
}
c.methods.push_back(std::move(m));m=M1("double","__native_math_cos","double","x");m.body->stmts.push_back(Ret(C1("__native_math_sin",Bn("+",V("x"),D(1.5707963267948966)))));c.methods.push_back(std::move(m));m=M1("double","__native_math_tan","double","x");m.body->stmts.push_back(Ret(Bn("/",C1("__native_math_sin",V("x")),C1("__native_math_cos",V("x")))));c.methods.push_back(std::move(m));m=M2("double","__native_math_hypot","double","a","double","b");m.body->stmts.push_back(Ret(C1("__native_math_sqrt",Bn("+",Bn("*",V("a"),V("a")),Bn("*",V("b"),V("b"))))));c.methods.push_back(std::move(m));m=M2("double","__native_math_fmod","double","a","double","b");m.body->stmts.push_back(Ret(Bn("%",V("a"),V("b"))));c.methods.push_back(std::move(m));p.classes.push_back(std::move(c));}

}} // namespace sleela::native
