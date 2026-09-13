#ifndef SLEELA_NATIVE_BUILDERS_H
#define SLEELA_NATIVE_BUILDERS_H
// -----------------------------------------------------------------------------
// Shared AST builder helpers for the native math/physics/economics lowering.
//
// The math, physics, and economics subject libraries all synthesize ordinary
// Sleela Core methods out of the same small set of AST-builder primitives
// (expressions, statements, and method shells). Those primitives live here so
// each subject can be compiled from its own translation unit
// (subjects/math, subjects/physics, subjects/economics) while sharing one
// definition of the builders. They are inline and header-local; there is no
// separate object file for the builders.
// -----------------------------------------------------------------------------
#include "native_api.h"
#include <memory>
#include <string>

namespace sleela { namespace native { namespace builders {

inline ExprP V(const std::string&n){return std::make_unique<VarExpr>(n);}
inline ExprP I(long long n){return std::make_unique<IntLit>(n);}
inline ExprP D(double n){return std::make_unique<DoubleLit>(n);}
inline ExprP Bn(const char*o,ExprP a,ExprP b){return std::make_unique<Binary>(o,std::move(a),std::move(b));}
inline std::unique_ptr<Call> C0(const std::string&n){return std::make_unique<Call>(n);}
inline ExprP C1(const std::string&n,ExprP a){auto c=C0(n);c->args.push_back(std::move(a));return c;}
inline ExprP C2(const std::string&n,ExprP a,ExprP b){auto c=C0(n);c->args.push_back(std::move(a));c->args.push_back(std::move(b));return c;}
inline ExprP Neg(ExprP a){return std::make_unique<Unary>("-",std::move(a));}
inline StmtP Ret(ExprP e){auto s=std::make_unique<ReturnStmt>();s->value=std::move(e);return s;}
inline StmtP Decl(const char*t,const char*n,ExprP e){auto s=std::make_unique<VarDecl>();s->type=t;s->name=n;s->init=std::move(e);return s;}
inline StmtP Set(const char*n,ExprP e){auto s=std::make_unique<Assign>();s->name=n;s->value=std::move(e);return s;}
inline StmtP Loop(ExprP c,std::unique_ptr<Block>b){auto s=std::make_unique<WhileStmt>();s->cond=std::move(c);s->body=std::move(b);return s;}
inline Method M0(const char*r,const char*n){Method m;m.retType=r;m.name=n;m.body=std::make_unique<Block>();return m;}
inline Method M1(const char*r,const char*n,const char*t,const char*p){Method m=M0(r,n);m.params.push_back({t,p});return m;}
inline Method M2(const char*r,const char*n,const char*t1,const char*p1,const char*t2,const char*p2){Method m=M1(r,n,t1,p1);m.params.push_back({t2,p2});return m;}
inline Method M3(const char*r,const char*n,const char*t1,const char*p1,const char*t2,const char*p2,const char*t3,const char*p3){Method m=M2(r,n,t1,p1,t2,p2);m.params.push_back({t3,p3});return m;}
inline Method M4(const char*r,const char*n,const char*t1,const char*p1,const char*t2,const char*p2,const char*t3,const char*p3,const char*t4,const char*p4){Method m=M3(r,n,t1,p1,t2,p2,t3,p3);m.params.push_back({t4,p4});return m;}

}}} // namespace sleela::native::builders

#endif // SLEELA_NATIVE_BUILDERS_H
