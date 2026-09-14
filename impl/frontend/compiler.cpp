// ===========================================================================
// compiler.cpp  --  AST -> Sleela Core bytecode.
// ===========================================================================
#include "compiler.h"

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace sleela {

namespace {

struct MethodCtx {
    std::map<std::string, int> locals;
    int nextSlot = 0;
    int slotOf(const std::string& name) const { auto it=locals.find(name); return it==locals.end()?-1:it->second; }
    int declare(const std::string& name) { auto it=locals.find(name); if(it!=locals.end()) return it->second; int s=nextSlot++; locals[name]=s; return s; }
};

class Compiler {
public:
    Compiler(const Program& prog, SLVM* vm, const catalog::Catalog* cat, const SyntaxVersion& syntax)
        : prog_(prog), vm_(vm), cat_(cat), syntax_(syntax) {}

    int run() {
        for(const auto& cls:prog_.classes) for(const auto& f:cls.fields){
            if(fieldGlobal_.count(f.name)) throw std::runtime_error("Semantic error: duplicate field '"+f.name+"'");
            fieldGlobal_[f.name]=slvm_declare_global(vm_,f.name.c_str()); fields_.push_back(&f);
        }
        for(const auto& cls:prog_.classes) for(const auto& m:cls.methods){
            MethodInfo mi; mi.method=&m; mi.nlocals=countLocals(m); funcIndex_[m.name]=(int)methods_.size(); methods_.push_back(mi);
        }
        if(funcIndex_.find("main")==funcIndex_.end()) throw std::runtime_error("Semantic error: no 'main' method found");
        for(auto& mi:methods_) emitMethod(mi);
        int entry=funcIndex_["main"]; slvm_set_entry(vm_,entry); return entry;
    }
private:
    struct MethodInfo { const Method* method; int nlocals; };
    const Program& prog_; SLVM* vm_; const catalog::Catalog* cat_; SyntaxVersion syntax_;
    std::map<std::string,int> funcIndex_; std::vector<MethodInfo> methods_;
    std::map<std::string,int> fieldGlobal_; std::vector<const Field*> fields_; MethodCtx* ctx_=nullptr;
    int fieldSlot(const std::string& name) const { auto it=fieldGlobal_.find(name); return it==fieldGlobal_.end()?-1:it->second; }
    int countLocals(const Method& m){int n=(int)m.params.size();countInBlock(*m.body,n);return n;}
    void countInBlock(const Block& b,int& n){for(const auto& s:b.stmts)countInStmt(s.get(),n);}
    void countInStmt(const Stmt* s,int& n){
        if(dynamic_cast<const VarDecl*>(s)) n++;
        else if(auto b=dynamic_cast<const Block*>(s)) countInBlock(*b,n);
        else if(auto i=dynamic_cast<const IfStmt*>(s)){countInStmt(i->thenS.get(),n);if(i->elseS)countInStmt(i->elseS.get(),n);}
        else if(auto w=dynamic_cast<const WhileStmt*>(s)) countInStmt(w->body.get(),n);
        else if(auto f=dynamic_cast<const ForStmt*>(s)){if(f->init)countInStmt(f->init.get(),n);countInStmt(f->body.get(),n);}
    }
    int here(){return slvm_here(vm_);} int emit(SLOp op,int a=0){return slvm_emit(vm_,op,a);} void patch(int at,int target){slvm_patch(vm_,at,target);}
    void emitMethod(MethodInfo& mi){
        const Method& m=*mi.method; MethodCtx ctx; for(const auto& p:m.params)ctx.declare(p.name);
        slvm_begin_func(vm_,m.name.c_str(),(int)m.params.size(),mi.nlocals); ctx_=&ctx;
        if(m.name=="main") for(const Field* f:fields_){if(f->init)emitExpr(f->init.get());else emit(OP_CONST,addNullConst());emit(OP_STOREG,fieldGlobal_[f->name]);}
        emitBlock(*m.body);ctx_=nullptr;emit(OP_CONST,addNullConst());emit(OP_RET);slvm_end_func(vm_);
    }
    int addNullConst(){if(nullConst_<0){SLExchangeArg a{};a.value=slval_null();slcore_exchange(vm_,SLX_ADD_CONST,&a);nullConst_=a.out;}return nullConst_;}
    int nullConst_=-1;
    void emitStmt(const Stmt* s){
        if(auto b=dynamic_cast<const Block*>(s)){emitBlock(*b);return;} if(auto d=dynamic_cast<const VarDecl*>(s)){emitVarDecl(*d);return;}
        if(auto a=dynamic_cast<const Assign*>(s)){emitAssign(*a);return;} if(auto e=dynamic_cast<const ExprStmt*>(s)){emitExpr(e->expr.get());emit(OP_POP);return;}
        if(auto p=dynamic_cast<const PrintStmt*>(s)){emitExpr(p->expr.get());emit(OP_PRINT);return;} if(auto r=dynamic_cast<const ReturnStmt*>(s)){emitReturn(*r);return;}
        if(auto i=dynamic_cast<const IfStmt*>(s)){emitIf(*i);return;} if(auto w=dynamic_cast<const WhileStmt*>(s)){emitWhile(*w);return;} if(auto f=dynamic_cast<const ForStmt*>(s)){emitFor(*f);return;}
        throw std::runtime_error("Semantic error: unknown statement kind");
    }
    void emitBlock(const Block& b){for(const auto& s:b.stmts)emitStmt(s.get());}
    void emitVarDecl(const VarDecl& d){int slot=ctx_->declare(d.name);if(d.init)emitExpr(d.init.get());else emit(OP_CONST,addNullConst());emit(OP_STOREL,slot);}
    void emitAssign(const Assign& a){int slot=ctx_->slotOf(a.name);if(slot>=0){emitExpr(a.value.get());emit(OP_STOREL,slot);return;}int g=fieldSlot(a.name);if(g>=0){emitExpr(a.value.get());emit(OP_STOREG,g);return;}throw std::runtime_error("Semantic error: assignment to undeclared variable '"+a.name+"'");}
    void emitReturn(const ReturnStmt& r){if(r.value)emitExpr(r.value.get());else emit(OP_CONST,addNullConst());emit(OP_RET);}
    void emitIf(const IfStmt& s){emitExpr(s.cond.get());int jf=emit(OP_JMPF,0);emitStmt(s.thenS.get());if(s.elseS){int jend=emit(OP_JMP,0);patch(jf,here());emitStmt(s.elseS.get());patch(jend,here());}else patch(jf,here());}
    void emitWhile(const WhileStmt& s){int top=here();emitExpr(s.cond.get());int jf=emit(OP_JMPF,0);emitStmt(s.body.get());emit(OP_JMP,top);patch(jf,here());}
    void emitFor(const ForStmt& s){if(s.init)emitStmt(s.init.get());int top=here();int jf=-1;if(s.cond){emitExpr(s.cond.get());jf=emit(OP_JMPF,0);}emitStmt(s.body.get());if(s.update)emitStmt(s.update.get());emit(OP_JMP,top);if(jf>=0)patch(jf,here());}
    void emitExpr(const Expr* e){
        if(auto x=dynamic_cast<const IntLit*>(e)){emit(OP_CONST,slvm_add_const_int(vm_,x->value));return;} if(auto x=dynamic_cast<const DoubleLit*>(e)){emit(OP_CONST,slvm_add_const_double(vm_,x->value));return;}
        if(auto x=dynamic_cast<const BoolLit*>(e)){emit(OP_CONST,slvm_add_const_bool(vm_,x->value?1:0));return;} if(auto x=dynamic_cast<const StrLit*>(e)){emit(OP_CONST,slvm_add_const_str(vm_,x->value.c_str()));return;}
        if(dynamic_cast<const NullLit*>(e)){emit(OP_CONST,addNullConst());return;} if(auto x=dynamic_cast<const VarExpr*>(e)){emitVar(*x);return;} if(auto x=dynamic_cast<const Unary*>(e)){emitUnary(*x);return;}
        if(auto x=dynamic_cast<const Binary*>(e)){emitBinary(*x);return;} if(auto x=dynamic_cast<const Call*>(e)){emitCall(*x);return;} throw std::runtime_error("Semantic error: unknown expression kind");
    }
    void emitVar(const VarExpr& v){int slot=ctx_->slotOf(v.name);if(slot>=0){emit(OP_LOADL,slot);return;}int g=fieldSlot(v.name);if(g>=0){emit(OP_LOADG,g);return;}throw std::runtime_error("Semantic error: use of undeclared variable '"+v.name+"'");}
    void emitUnary(const Unary& u){emitExpr(u.operand.get());if(u.op=="-")emit(OP_NEG);else if(u.op=="!")emit(OP_NOT);else throw std::runtime_error("Semantic error: unknown unary operator '"+u.op+"'");}
    void emitBinary(const Binary& b){emitExpr(b.lhs.get());emitExpr(b.rhs.get());const std::string&o=b.op;if(o=="+")emit(OP_ADD);else if(o=="-")emit(OP_SUB);else if(o=="*")emit(OP_MUL);else if(o=="/")emit(OP_DIV);else if(o=="%")emit(OP_MOD);else if(o=="==")emit(OP_EQ);else if(o=="!=")emit(OP_NE);else if(o=="<")emit(OP_LT);else if(o=="<=")emit(OP_LE);else if(o==">")emit(OP_GT);else if(o==">=")emit(OP_GE);else if(o=="&&")emit(OP_AND);else if(o=="||")emit(OP_OR);else throw std::runtime_error("Semantic error: unknown binary operator '"+o+"'");}

    bool tryEmitBuiltin(const Call& c){
        const std::string& n=c.callee;
        auto litInt=[&](const Expr*e,const char*what)->int{auto il=dynamic_cast<const IntLit*>(e);if(!il)throw std::runtime_error("Semantic error: "+std::string(what)+" must be an integer literal");return(int)il->value;};
        if(n=="spawn"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: spawn(method) takes exactly one argument");auto var=dynamic_cast<const VarExpr*>(c.args[0].get());if(!var)throw std::runtime_error("Semantic error: spawn(method) argument must be a method name");auto it=funcIndex_.find(var->name);if(it==funcIndex_.end())throw std::runtime_error("Semantic error: spawn of unknown method '"+var->name+"'");if(!methods_[it->second].method->params.empty())throw std::runtime_error("Semantic error: spawn target '"+var->name+"' must take no parameters");emit(OP_SPAWN,it->second);return true;}
        if(n=="join"){if(!c.args.empty())throw std::runtime_error("Semantic error: join() takes no arguments");emit(OP_JOINALL);emit(OP_CONST,addNullConst());return true;}
        if(n=="lock"||n=="unlock"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: "+n+"(id) takes exactly one argument");int id=litInt(c.args[0].get(),(n+" id").c_str());emit(n=="lock"?OP_LOCK:OP_UNLOCK,id);emit(OP_CONST,addNullConst());return true;}
        if(n=="send"){if(c.args.size()!=2)throw std::runtime_error("Semantic error: send(slot, value) takes two arguments");int slot=litInt(c.args[0].get(),"send slot");emitExpr(c.args[1].get());emit(OP_SEND,slot);emit(OP_CONST,addNullConst());return true;}
        if(n=="recv"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: recv(slot) takes exactly one argument");emit(OP_RECV,litInt(c.args[0].get(),"recv slot"));return true;}

        if(n=="listen"||n=="accept"||n=="connect"||n=="sockread"||n=="sockwrite"||n=="sockclose"){
            if(syntax_<SyntaxVersion{1,1})throw std::runtime_error("Semantic error: network built-ins require #sleela 1.1");
        }
        if(n=="listen"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: listen(port) takes exactly one argument");emitExpr(c.args[0].get());emit(OP_LISTEN);return true;}
        if(n=="accept"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: accept(socket) takes exactly one argument");emitExpr(c.args[0].get());emit(OP_ACCEPT);return true;}
        if(n=="connect"){if(c.args.size()!=2)throw std::runtime_error("Semantic error: connect(host, port) takes exactly two arguments");emitExpr(c.args[0].get());emitExpr(c.args[1].get());emit(OP_CONNECT);return true;}
        if(n=="sockread"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: sockread(socket) takes exactly one argument");emitExpr(c.args[0].get());emit(OP_SOCKREAD);return true;}
        if(n=="sockwrite"){if(c.args.size()!=2)throw std::runtime_error("Semantic error: sockwrite(socket, data) takes exactly two arguments");emitExpr(c.args[0].get());emitExpr(c.args[1].get());emit(OP_SOCKWRITE);return true;}
        if(n=="sockclose"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: sockclose(socket) takes exactly one argument");emitExpr(c.args[0].get());emit(OP_SOCKCLOSE);return true;}

        // Linux file/I/O built-ins. Version 1.1 adds a POSIX-oriented
        // descriptor layer; handles are VM-local integers, never raw FDs.
        if(n=="pipe"||n=="pipePeer"||n=="fifoCreate"||n=="npfsCreate"||n=="openFile"||n=="read"||n=="write"||n=="close"||n=="unlinkFile"){
            if(syntax_<SyntaxVersion{1,1})throw std::runtime_error("Semantic error: file I/O built-ins require #sleela 1.1");
        }
        if(n=="pipe"){
            if(!c.args.empty())throw std::runtime_error("Semantic error: pipe() takes no arguments");
            emit(OP_PIPE);return true;
        }
        if(n=="pipePeer"){
            if(c.args.size()!=1)throw std::runtime_error("Semantic error: pipePeer(handle) takes one argument");
            emitExpr(c.args[0].get());emit(OP_PIPEPEER);return true;
        }
        if(n=="fifoCreate"||n=="npfsCreate"){
            if(c.args.size()!=2)throw std::runtime_error("Semantic error: "+n+"(path, mode) takes two arguments");
            emitExpr(c.args[0].get());emitExpr(c.args[1].get());emit(OP_FIFO_MK);return true;
        }
        if(n=="openFile"){
            if(c.args.size()!=2)throw std::runtime_error("Semantic error: openFile(path, mode) takes two arguments");
            emitExpr(c.args[0].get());emitExpr(c.args[1].get());emit(OP_FILEOPEN);return true;
        }
        if(n=="read"){
            if(c.args.size()!=1)throw std::runtime_error("Semantic error: read(handle) takes one argument");
            emitExpr(c.args[0].get());emit(OP_FILEREAD);return true;
        }
        if(n=="write"){
            if(c.args.size()!=2)throw std::runtime_error("Semantic error: write(handle, data) takes two arguments");
            emitExpr(c.args[0].get());emitExpr(c.args[1].get());emit(OP_FILEWRITE);return true;
        }
        if(n=="close"){
            if(c.args.size()!=1)throw std::runtime_error("Semantic error: close(handle) takes one argument");
            emitExpr(c.args[0].get());emit(OP_FILECLOSE);return true;
        }
        if(n=="unlinkFile"){
            if(c.args.size()!=1)throw std::runtime_error("Semantic error: unlinkFile(path) takes one argument");
            emitExpr(c.args[0].get());emit(OP_FILEUNLINK);return true;
        }

        if(n=="timeUtcMillis"){if(!c.args.empty())throw std::runtime_error("Semantic error: timeUtcMillis() takes no arguments");emit(OP_TIME_UTC_MS);return true;}
        if(n=="timeMonotonicNanos"){if(!c.args.empty())throw std::runtime_error("Semantic error: timeMonotonicNanos() takes no arguments");emit(OP_TIME_MONO_NS);return true;}
        if(n=="timePrecisionMillis"){if(!c.args.empty())throw std::runtime_error("Semantic error: timePrecisionMillis() takes no arguments");emit(OP_TIME_PRECISION_MS);return true;}
        if(n=="timeLocation"){if(!c.args.empty())throw std::runtime_error("Semantic error: timeLocation() takes no arguments");emit(OP_TIME_LOCATION);return true;}
        if(n=="timeHttpDate"){if(!c.args.empty())throw std::runtime_error("Semantic error: timeHttpDate() takes no arguments");emit(OP_TIME_HTTP_DATE);return true;}
        if(n=="timeJson"){if(!c.args.empty())throw std::runtime_error("Semantic error: timeJson() takes no arguments");emit(OP_TIME_JSON);return true;}
        if(n=="timeNtp"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: timeNtp(host) takes one argument");emitExpr(c.args[0].get());emit(OP_TIME_NTP);return true;}
        if(n=="timeSetLocation"){if(c.args.size()!=2)throw std::runtime_error("Semantic error: timeSetLocation(country, timezone) takes two arguments");emitExpr(c.args[0].get());emitExpr(c.args[1].get());emit(OP_TIME_SET_LOCATION);return true;}

        auto litStr=[&](const Expr*e,const char*what)->std::string{auto sl=dynamic_cast<const StrLit*>(e);if(!sl)throw std::runtime_error("Semantic error: "+std::string(what)+" must be a string literal (an object name)");return sl->value;};
        auto emitStr=[&](const std::string&s){emit(OP_CONST,slvm_add_const_str(vm_,s.c_str()));};
        auto emitBool=[&](bool b){emit(OP_CONST,slvm_add_const_bool(vm_,b?1:0));};
        if(n=="conduct"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: conduct(name) takes one argument");std::string name=litStr(c.args[0].get(),"conduct name");emitBool(cat_&&cat_->find(name)!=nullptr);return true;}
        if(n=="role"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: role(name) takes one argument");std::string name=litStr(c.args[0].get(),"role name");const auto*o=cat_?cat_->find(name):nullptr;emitStr(o?o->role:"");return true;}
        if(n=="insight"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: insight(name) takes one argument");std::string name=litStr(c.args[0].get(),"insight name");const auto*o=cat_?cat_->find(name):nullptr;emitStr(o?o->note:"");return true;}
        if(n=="congruent"){if(c.args.size()!=2)throw std::runtime_error("Semantic error: congruent(a, b) takes two arguments");std::string a=litStr(c.args[0].get(),"congruent a"),b=litStr(c.args[1].get(),"congruent b");emitBool(cat_&&cat_->congruent(a,b));return true;}
        if(n=="route"){if(c.args.size()!=2)throw std::runtime_error("Semantic error: route(a, b) takes two arguments");std::string a=litStr(c.args[0].get(),"route a"),b=litStr(c.args[1].get(),"route b");emitStr(cat_&&cat_->congruent(a,b)?a+" -> "+b:"");return true;}
        if(n=="sysdepth"){if(!c.args.empty())throw std::runtime_error("Semantic error: sysdepth() takes no arguments");emit(OP_CONST,slvm_add_const_int(vm_,cat_?cat_->depth:0));return true;}
        if(n=="degreemax"){if(!c.args.empty())throw std::runtime_error("Semantic error: degreemax() takes no arguments");emit(OP_CONST,slvm_add_const_int(vm_,cat_?cat_->complexityDegreeMax:0));return true;}
        return false;
    }
    void emitCall(const Call& c){
        if(tryEmitBuiltin(c))return; auto it=funcIndex_.find(c.callee); if(it==funcIndex_.end())throw std::runtime_error("Semantic error: call to unknown method '"+c.callee+"'");
        const Method* target=methods_[it->second].method; if((int)c.args.size()!=(int)target->params.size())throw std::runtime_error("Semantic error: method '"+c.callee+"' expects "+std::to_string(target->params.size())+" argument(s), got "+std::to_string(c.args.size()));
        for(const auto&a:c.args)emitExpr(a.get());emit(OP_CALL,it->second);
    }
};

} // anonymous namespace

int compile(const Program& prog, SLVM* vm, const catalog::Catalog* cat, const SyntaxVersion& syntax){Compiler c(prog,vm,cat,syntax);return c.run();}

} // namespace sleela
