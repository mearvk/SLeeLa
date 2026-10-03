// native_api.cpp -- Unified native-module dispatcher for math/physics/economics.
//
// This is the shared entry point that ties the executable quantitative
// Subject Libraries together. It validates imports, enforces the math dependency for
// physics and economics, invokes the per-subject class builders
// (addMath/addPhysics/addEconomics, each defined in its own subject file under
// subjects/{math,physics,economics}/), and lowers qualified module calls
// (e.g. `math.sqrt`) into the synthesized native methods.
//
// Chemistry and Financial are independent compiled libraries with their own
// frontend lowering (subjects/chemistry, subjects/finance); they are not part
// of this native dispatcher.
#include "native_api.h"
#include "native_add.h"
#include <stdexcept>
#include <set>
#include <string>

namespace sleela { namespace native {

bool isModuleAvailable(const std::string& module) {
    static const std::set<std::string> modules = {"math","physics","economics","inference","astrophysics","sociology","excel","json","crypto","net"};
    return modules.count(module) != 0;
}

void validateImports(const Program& program) {
    std::set<std::string> seen;
    for (const auto& module : program.imports) {
        if (!isModuleAvailable(module)) throw std::runtime_error("Semantic error: unknown native module '" + module + "'");
        if (!seen.insert(module).second) throw std::runtime_error("Semantic error: duplicate import '" + module + "'");
    }
}

namespace {
bool imported(const Program&p,const std::string&m){for(const auto&s:p.imports)if(s==m)return true;return false;}
bool hasClass(const Program&p,const std::string&name){for(const auto&c:p.classes)if(c.name==name)return true;return false;}
std::string helper(const std::string&s){return "__native_"+s;}
void lowerExpr(ExprP&e,const Program&p){if(!e)return;if(auto c=dynamic_cast<Call*>(e.get())){for(auto&a:c->args)lowerExpr(a,p);auto pos=c->callee.find('.');if(pos!=std::string::npos){std::string mod=c->callee.substr(0,pos),fn=c->callee.substr(pos+1);if(isModuleAvailable(mod)){if(!imported(p,mod))throw std::runtime_error("Semantic error: native module '"+mod+"' is not imported");c->callee=helper(mod+"_"+fn);}}return;}if(auto u=dynamic_cast<Unary*>(e.get())){lowerExpr(u->operand,p);return;}if(auto b=dynamic_cast<Binary*>(e.get())){lowerExpr(b->lhs,p);lowerExpr(b->rhs,p);return;}if(auto ma=dynamic_cast<MemberAccess*>(e.get())){lowerExpr(ma->base,p);return;}if(auto mc=dynamic_cast<MethodCall*>(e.get())){lowerExpr(mc->receiver,p);for(auto&a:mc->args)lowerExpr(a,p);return;}if(auto n=dynamic_cast<NewExpr*>(e.get())){for(auto&a:n->args)lowerExpr(a,p);return;}if(auto ae=dynamic_cast<AssignmentExpr*>(e.get())){lowerExpr(ae->target,p);lowerExpr(ae->value,p);return;}if(auto ce=dynamic_cast<ConditionalExpr*>(e.get())){lowerExpr(ce->cond,p);lowerExpr(ce->thenE,p);lowerExpr(ce->elseE,p);return;}if(auto io=dynamic_cast<InstanceOfExpr*>(e.get())){lowerExpr(io->value,p);return;}if(auto ca=dynamic_cast<CastExpr*>(e.get())){lowerExpr(ca->operand,p);return;}if(auto aa=dynamic_cast<ArrayAccess*>(e.get())){lowerExpr(aa->base,p);lowerExpr(aa->index,p);return;}if(auto mr=dynamic_cast<MethodReferenceExpr*>(e.get())){lowerExpr(mr->base,p);return;}}
void lowerStmt(StmtP&s,const Program&p){if(auto v=dynamic_cast<VarDecl*>(s.get())){lowerExpr(v->init,p);return;}if(auto a=dynamic_cast<Assign*>(s.get())){lowerExpr(a->value,p);return;}if(auto fa=dynamic_cast<FieldAssign*>(s.get())){lowerExpr(fa->base,p);lowerExpr(fa->value,p);return;}if(auto r=dynamic_cast<ReturnStmt*>(s.get())){lowerExpr(r->value,p);return;}if(auto e=dynamic_cast<ExprStmt*>(s.get())){lowerExpr(e->expr,p);return;}if(auto q=dynamic_cast<PrintStmt*>(s.get())){lowerExpr(q->expr,p);return;}if(auto b=dynamic_cast<Block*>(s.get())){for(auto&x:b->stmts)lowerStmt(x,p);return;}if(auto i=dynamic_cast<IfStmt*>(s.get())){lowerExpr(i->cond,p);lowerStmt(i->thenS,p);if(i->elseS)lowerStmt(i->elseS,p);return;}if(auto w=dynamic_cast<WhileStmt*>(s.get())){lowerExpr(w->cond,p);lowerStmt(w->body,p);return;}if(auto f=dynamic_cast<ForStmt*>(s.get())){if(f->init)lowerStmt(f->init,p);if(f->cond)lowerExpr(f->cond,p);if(f->update)lowerStmt(f->update,p);lowerStmt(f->body,p);return;}if(auto d=dynamic_cast<DoStmt*>(s.get())){lowerStmt(d->body,p);lowerExpr(d->cond,p);return;}if(auto t=dynamic_cast<ThrowStmt*>(s.get())){lowerExpr(t->value,p);return;}if(auto as=dynamic_cast<AssertStmt*>(s.get())){lowerExpr(as->cond,p);if(as->message)lowerExpr(as->message,p);return;}if(auto y=dynamic_cast<YieldStmt*>(s.get())){lowerExpr(y->value,p);return;}if(auto sy=dynamic_cast<SynchronizedStmt*>(s.get())){lowerExpr(sy->monitor,p);for(auto&x:sy->body->stmts)lowerStmt(x,p);return;}if(auto tr=dynamic_cast<TryStmt*>(s.get())){for(auto&x:tr->body->stmts)lowerStmt(x,p);for(auto&c:tr->catches)for(auto&x:c.body->stmts)lowerStmt(x,p);if(tr->finallyBlock)for(auto&x:tr->finallyBlock->stmts)lowerStmt(x,p);return;}if(auto sw=dynamic_cast<SwitchStmt*>(s.get())){lowerExpr(sw->selector,p);for(auto&c:sw->cases){for(auto&l:c.labels)lowerExpr(l,p);for(auto&x:c.statements)lowerStmt(x,p);}return;}}
}

void lowerProgram(Program&program){validateImports(program);bool m=imported(program,"math"),ph=imported(program,"physics"),ec=imported(program,"economics"),inf=imported(program,"inference"),ap=imported(program,"astrophysics"),soc=imported(program,"sociology");if(ph&&!m)throw std::runtime_error("Semantic error: physics requires import math");if(ec&&!m)throw std::runtime_error("Semantic error: economics requires import math");if(inf&&!m)throw std::runtime_error("Semantic error: inference requires import math");if(ap&&!m)throw std::runtime_error("Semantic error: astrophysics requires import math");if(soc&&!m)throw std::runtime_error("Semantic error: sociology requires import math");if(m&&!hasClass(program,"__NativeMath"))addMath(program);if(ph&&!hasClass(program,"__NativePhysics"))addPhysics(program);if(ec&&!hasClass(program,"__NativeEconomics"))addEconomics(program);if(inf&&!hasClass(program,"__NativeInference"))addInference(program);if(ap&&!hasClass(program,"__NativeAstrophysics"))addAstrophysics(program);if(soc&&!hasClass(program,"__NativeSociology"))addSociology(program);std::size_t original=program.classes.size();for(std::size_t ci=0;ci<original;ci++)for(auto&me:program.classes[ci].methods)for(auto&s:me.body->stmts)lowerStmt(s,program);}

}} // namespace sleela::native
