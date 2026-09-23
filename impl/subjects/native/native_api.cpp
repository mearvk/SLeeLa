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
std::string helper(const std::string&s){return "__native_"+s;}
void lowerExpr(ExprP&e,const Program&p){if(!e)return;if(auto c=dynamic_cast<Call*>(e.get())){for(auto&a:c->args)lowerExpr(a,p);auto pos=c->callee.find('.');if(pos!=std::string::npos){std::string mod=c->callee.substr(0,pos),fn=c->callee.substr(pos+1);if(isModuleAvailable(mod)){if(!imported(p,mod))throw std::runtime_error("Semantic error: native module '"+mod+"' is not imported");c->callee=helper(mod+"_"+fn);}}return;}if(auto u=dynamic_cast<Unary*>(e.get())){lowerExpr(u->operand,p);return;}if(auto b=dynamic_cast<Binary*>(e.get())){lowerExpr(b->lhs,p);lowerExpr(b->rhs,p);return;}if(auto ma=dynamic_cast<MemberAccess*>(e.get())){lowerExpr(ma->base,p);return;}}
void lowerStmt(StmtP&s,const Program&p){if(auto v=dynamic_cast<VarDecl*>(s.get())){lowerExpr(v->init,p);return;}if(auto a=dynamic_cast<Assign*>(s.get())){lowerExpr(a->value,p);return;}if(auto fa=dynamic_cast<FieldAssign*>(s.get())){lowerExpr(fa->base,p);lowerExpr(fa->value,p);return;}if(auto r=dynamic_cast<ReturnStmt*>(s.get())){lowerExpr(r->value,p);return;}if(auto e=dynamic_cast<ExprStmt*>(s.get())){lowerExpr(e->expr,p);return;}if(auto q=dynamic_cast<PrintStmt*>(s.get())){lowerExpr(q->expr,p);return;}if(auto b=dynamic_cast<Block*>(s.get())){for(auto&x:b->stmts)lowerStmt(x,p);return;}if(auto i=dynamic_cast<IfStmt*>(s.get())){lowerExpr(i->cond,p);lowerStmt(i->thenS,p);if(i->elseS)lowerStmt(i->elseS,p);return;}if(auto w=dynamic_cast<WhileStmt*>(s.get())){lowerExpr(w->cond,p);lowerStmt(w->body,p);return;}if(auto f=dynamic_cast<ForStmt*>(s.get())){if(f->init)lowerStmt(f->init,p);if(f->cond)lowerExpr(f->cond,p);if(f->update)lowerStmt(f->update,p);lowerStmt(f->body,p);}}
}

void lowerProgram(Program&program){validateImports(program);bool m=imported(program,"math"),ph=imported(program,"physics"),ec=imported(program,"economics"),inf=imported(program,"inference"),ap=imported(program,"astrophysics"),soc=imported(program,"sociology");if(ph&&!m)throw std::runtime_error("Semantic error: physics requires import math");if(ec&&!m)throw std::runtime_error("Semantic error: economics requires import math");if(inf&&!m)throw std::runtime_error("Semantic error: inference requires import math");if(ap&&!m)throw std::runtime_error("Semantic error: astrophysics requires import math");if(soc&&!m)throw std::runtime_error("Semantic error: sociology requires import math");if(m)addMath(program);if(ph)addPhysics(program);if(ec)addEconomics(program);if(inf)addInference(program);if(ap)addAstrophysics(program);if(soc)addSociology(program);std::size_t original=program.classes.size();for(std::size_t ci=0;ci<original;ci++)for(auto&me:program.classes[ci].methods)for(auto&s:me.body->stmts)lowerStmt(s,program);}

}} // namespace sleela::native
