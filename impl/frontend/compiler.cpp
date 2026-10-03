// ===========================================================================
// compiler.cpp  --  AST -> Sleela Core bytecode.
// ===========================================================================
#include "compiler.h"
#include "semantic.h"
#include "java_compat.h"

#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <set>
#include "library_index.h"

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

    // Protected members are deliberately stronger than ordinary C++ access:
    // Sleela protected source is admitted only as static protected and is bound
    // to the VM's managed-handle memory model. Degree 2 means the declaration
    // crosses two bounded relations: class member -> managed VM storage handle.
    // It is NOT raw pointer arithmetic or a promise to dereference an address.
    static constexpr int kProtectedSystemDegree = 2;
    // `next` is System Degree 1; `next.next` is the bounded Degree 2 idiom.
    // These are symbolic VM-safe relations, never pointer arithmetic.
    static constexpr int kNextSystemDegree = 1;
    // A static extension of a Degree-2 proposal gains a further two degrees
    // of viewpoint: Degree 2 + static extension(2) = Viewpoint Degree 4.
    static constexpr int kStaticExtensionViewpointDegree = 4;
    int run() {
        validateJavaCompatibilityMetadata(prog_);
        library::Index libraryIndex = library::Index::discover();
        library::validateImports(prog_.imports, libraryIndex);
        // Protected source is admitted only when both language invariants hold:
        // (1) the member is static, and (2) the runtime reports the managed VM
        // memory model. The native core never exposes a raw pointer to Sleela
        // source; struct values are bounded VM handles.
        bool hasProtected = false;
        for (const auto& cls : prog_.classes) {
            for (const auto& f : cls.fields) hasProtected = hasProtected || f.isProtected;
            for (const auto& m : cls.methods) hasProtected = hasProtected || m.isProtected;
        }
        if (hasProtected && !slvm_memory_safe_mode(vm_))
            throw std::runtime_error("Semantic error: protected source requires the SLeeLa managed memory safety scheme");
        for (const auto& cls : prog_.classes) {
            for (const auto& f : cls.fields) {
                if (f.isProtected && !f.isStatic)
                    throw std::runtime_error("Semantic error: protected field '" + f.name + "' must also be static (degree 2 safety rule)");
            }
            for (const auto& m : cls.methods) {
                if (m.isProtected && !m.isStatic)
                    throw std::runtime_error("Semantic error: protected method '" + m.name + "' must also be static (degree 2 safety rule)");
            }
        }
        (void)kProtectedSystemDegree;
        (void)kNextSystemDegree;
        (void)kStaticExtensionViewpointDegree;
        // Struct declarations: register each layout with the VM and record a
        // compiler-side layout (type index + ordered field names -> offsets).
        for(const auto& st:prog_.structs){
            if(st.fields.size()>64) throw std::runtime_error("Semantic error: struct '"+st.name+"' exceeds 64 fields");
            if(structLayout_.count(st.name)) throw std::runtime_error("Semantic error: duplicate struct '"+st.name+"'");
            if(syntax_<SyntaxVersion{1,2}) throw std::runtime_error("Semantic error: struct declarations require #sleela 1.2");
            StructLayout layout; layout.name=st.name;
            std::vector<const char*> fieldNames;
            for(size_t i=0;i<st.fields.size();++i){
                const auto& f=st.fields[i];
                if(layout.fieldOffset.count(f.name)) throw std::runtime_error("Semantic error: duplicate field '"+f.name+"' in struct '"+st.name+"'");
                layout.fieldOffset[f.name]=(int)i; layout.fieldType.push_back(f.type); fieldNames.push_back(f.name.c_str());
            }
            layout.typeIndex=slvm_declare_struct(vm_,st.name.c_str(),fieldNames.empty()?nullptr:fieldNames.data(),(int)fieldNames.size());
            if(layout.typeIndex<0) throw std::runtime_error("Semantic error: could not register struct '"+st.name+"' (too many struct types?)");
            structLayout_[st.name]=layout;
        }
        for(const auto& cls:prog_.classes) for(const auto& f:cls.fields){
            if(fieldGlobal_.count(f.name)) throw std::runtime_error("Semantic error: duplicate field '"+f.name+"'");
            fieldGlobal_[f.name]=slvm_declare_global(vm_,f.name.c_str()); fieldProtected_[f.name]=f.isProtected; fieldOwner_[f.name]=cls.name; fields_.push_back(&f);
            if(structLayout_.count(f.type)) varType_[f.name]=f.type; // struct-typed global
        }
        for(const auto& cls:prog_.classes) for(const auto& m:cls.methods){
            MethodInfo mi; mi.method=&m; mi.nlocals=countLocals(m); if (m.isProtected) protectedMethods_.insert(m.name); methodOwner_[m.name]=cls.name; funcIndex_[m.name]=(int)methods_.size(); methods_.push_back(mi);
        }
        if(funcIndex_.find("main")==funcIndex_.end()) throw std::runtime_error("Semantic error: no 'main' method found");
        for(auto& mi:methods_) emitMethod(mi);
        int entry=funcIndex_["main"]; slvm_set_entry(vm_,entry); return entry;
    }
private:
    struct MethodInfo { const Method* method; int nlocals; };
    // A struct's compile-time layout: its VM type index, and each field's
    // position (offset) and declared type, in declaration order.
    struct StructLayout { std::string name; int typeIndex=-1; std::map<std::string,int> fieldOffset; std::vector<std::string> fieldType; };
    const Program& prog_; SLVM* vm_; const catalog::Catalog* cat_; SyntaxVersion syntax_;
    std::map<std::string,int> funcIndex_; std::map<std::string,std::string> methodOwner_; std::set<std::string> protectedMethods_; std::vector<MethodInfo> methods_;
    std::map<std::string,int> fieldGlobal_; std::map<std::string,bool> fieldProtected_; std::map<std::string,std::string> fieldOwner_; std::vector<const Field*> fields_; MethodCtx* ctx_=nullptr;
    std::string currentClass_;
    std::map<std::string,StructLayout> structLayout_;
    // Track the declared type of each in-scope local/global so member access
    // can resolve `x.field` to the right struct layout. Names not present are
    // untyped (dynamic); member access then defers field resolution to runtime.
    std::map<std::string,std::string> varType_;
    int fieldSlot(const std::string& name) const { auto it=fieldGlobal_.find(name); return it==fieldGlobal_.end()?-1:it->second; }
    int countLocals(const Method& m){int n=(int)m.params.size();countInBlock(*m.body,n);return n;}
    void countInBlock(const Block& b,int& n){for(const auto& s:b.stmts)countInStmt(s.get(),n);}
    void countInStmt(const Stmt* s,int& n){
        if(dynamic_cast<const VarDecl*>(s)) n++;
        else if(auto b=dynamic_cast<const Block*>(s)) countInBlock(*b,n);
        else if(auto i=dynamic_cast<const IfStmt*>(s)){countInStmt(i->thenS.get(),n);if(i->elseS)countInStmt(i->elseS.get(),n);}
        else if(auto w=dynamic_cast<const WhileStmt*>(s)) countInStmt(w->body.get(),n);
        else if(auto f=dynamic_cast<const ForStmt*>(s)){if(f->init)countInStmt(f->init.get(),n);countInStmt(f->body.get(),n);}
        else if(auto d=dynamic_cast<const DoStmt*>(s)) countInStmt(d->body.get(),n);
        else if(auto sy=dynamic_cast<const SynchronizedStmt*>(s)) countInBlock(*sy->body,n);
        else if(auto tr=dynamic_cast<const TryStmt*>(s)){countInBlock(*tr->body,n);for(const auto& c:tr->catches){n++;countInBlock(*c.body,n);}if(tr->finallyBlock)countInBlock(*tr->finallyBlock,n);}
        else if(auto sw=dynamic_cast<const SwitchStmt*>(s)){n++;/* selector temp */for(const auto& c:sw->cases)for(const auto& st:c.statements)countInStmt(st.get(),n);}
    }
    int here(){return slvm_here(vm_);} int emit(SLOp op,int a=0){return slvm_emit(vm_,op,a);} void patch(int at,int target){slvm_patch(vm_,at,target);}
    void emitMethod(MethodInfo& mi){
        const Method& m=*mi.method; MethodCtx ctx; currentClass_=methodOwner_[m.name]; for(const auto& p:m.params)ctx.declare(p.name);
        // Per-method type scope: seed parameter types, restore globals after.
        std::map<std::string,std::string> savedTypes=varType_;
        for(const auto& p:m.params) if(structLayout_.count(p.type)) varType_[p.name]=p.type;
        slvm_begin_func(vm_,m.name.c_str(),(int)m.params.size(),mi.nlocals); ctx_=&ctx;
        if(m.name=="main") for(const Field* f:fields_){if(f->init)emitExpr(f->init.get());else emit(OP_CONST,addNullConst());emit(OP_STOREG,fieldGlobal_[f->name]);}
        emitBlock(*m.body);ctx_=nullptr;emit(OP_CONST,addNullConst());emit(OP_RET);slvm_end_func(vm_);
        varType_=savedTypes;
    }
    int addNullConst(){if(nullConst_<0){SLExchangeArg a{};a.value=slval_null();slcore_exchange(vm_,SLX_ADD_CONST,&a);nullConst_=a.out;}return nullConst_;}
    int nullConst_=-1;
    void emitStmt(const Stmt* s){
        if(auto b=dynamic_cast<const Block*>(s)){emitBlock(*b);return;} if(auto d=dynamic_cast<const VarDecl*>(s)){emitVarDecl(*d);return;}
        if(auto a=dynamic_cast<const Assign*>(s)){emitAssign(*a);return;} if(auto e=dynamic_cast<const ExprStmt*>(s)){emitExpr(e->expr.get());emit(OP_POP);return;}
        if(auto p=dynamic_cast<const PrintStmt*>(s)){emitExpr(p->expr.get());emit(OP_PRINT);return;} if(auto r=dynamic_cast<const ReturnStmt*>(s)){emitReturn(*r);return;}
        if(auto i=dynamic_cast<const IfStmt*>(s)){emitIf(*i);return;} if(auto w=dynamic_cast<const WhileStmt*>(s)){emitWhile(*w);return;} if(auto f=dynamic_cast<const ForStmt*>(s)){emitFor(*f);return;}
        if(auto fa=dynamic_cast<const FieldAssign*>(s)){emitFieldAssign(*fa);return;}
        if(auto d=dynamic_cast<const DoStmt*>(s)){emitDo(*d);return;}
        if(auto sw=dynamic_cast<const SwitchStmt*>(s)){emitSwitch(*sw);return;}
        if(auto sy=dynamic_cast<const SynchronizedStmt*>(s)){emitSynchronized(*sy);return;}
        if(auto tr=dynamic_cast<const TryStmt*>(s)){emitTry(*tr);return;}
        if(auto t=dynamic_cast<const ThrowStmt*>(s)){emitExpr(t->value.get());emit(OP_POP);return;}
        if(auto a=dynamic_cast<const AssertStmt*>(s)){emitExpr(a->cond.get());emit(OP_POP);return;}
        if(auto y=dynamic_cast<const YieldStmt*>(s)){emitExpr(y->value.get());emit(OP_POP);return;}
        if(dynamic_cast<const BreakStmt*>(s)){emitBreak();return;}
        if(dynamic_cast<const ContinueStmt*>(s)){emitContinue();return;}
        throw std::runtime_error("Semantic error: unknown statement kind");
    }
    void emitBlock(const Block& b){for(const auto& s:b.stmts)emitStmt(s.get());}
    void emitVarDecl(const VarDecl& d){int slot=ctx_->declare(d.name);if(structLayout_.count(d.type))varType_[d.name]=d.type;if(d.init)emitExpr(d.init.get());else emit(OP_CONST,addNullConst());emit(OP_STOREL,slot);}
    void emitAssign(const Assign& a){int slot=ctx_->slotOf(a.name);if(slot>=0){emitExpr(a.value.get());emit(OP_STOREL,slot);return;}int g=fieldSlot(a.name);if(g>=0){if(fieldProtected_[a.name] && fieldOwner_[a.name]!=currentClass_) throw std::runtime_error("protected field access denied");emitExpr(a.value.get());emit(OP_STOREG,g);return;}throw std::runtime_error("Semantic error: assignment to undeclared variable '"+a.name+"'");}
    void emitReturn(const ReturnStmt& r){if(r.value)emitExpr(r.value.get());else emit(OP_CONST,addNullConst());emit(OP_RET);}
    void emitFieldAssign(const FieldAssign& fa){int off=-1;memberLayout(fa.base.get(),fa.field,off);emitExpr(fa.base.get());emitExpr(fa.value.get());emit(OP_SETFIELD,off);emit(OP_POP);}
    void emitIf(const IfStmt& s){emitExpr(s.cond.get());int jf=emit(OP_JMPF,0);emitStmt(s.thenS.get());if(s.elseS){int jend=emit(OP_JMP,0);patch(jf,here());emitStmt(s.elseS.get());patch(jend,here());}else patch(jf,here());}
    // Loop context for break/continue: each enclosing loop records the jump
    // sites that must be patched to the loop exit (break) and to the loop's
    // continue target (continue). We patch them when the loop finishes.
    struct LoopCtx{std::vector<int> breaks;std::vector<int> continues;};
    std::vector<LoopCtx> loops_;
    void emitBreak(){if(loops_.empty())throw std::runtime_error("Semantic error: 'break' used outside a loop or switch");loops_.back().breaks.push_back(emit(OP_JMP,0));}
    void emitContinue(){if(loops_.empty())throw std::runtime_error("Semantic error: 'continue' used outside a loop");loops_.back().continues.push_back(emit(OP_JMP,0));}
    void emitWhile(const WhileStmt& s){loops_.push_back({});int top=here();emitExpr(s.cond.get());int jf=emit(OP_JMPF,0);emitStmt(s.body.get());for(int c:loops_.back().continues)patch(c,top);emit(OP_JMP,top);patch(jf,here());for(int b:loops_.back().breaks)patch(b,here());loops_.pop_back();}
    void emitDo(const DoStmt& s){loops_.push_back({});int top=here();emitStmt(s.body.get());int contTarget=here();for(int c:loops_.back().continues)patch(c,contTarget);emitExpr(s.cond.get());int jf=emit(OP_JMPF,0);emit(OP_JMP,top);patch(jf,here());for(int b:loops_.back().breaks)patch(b,here());loops_.pop_back();}
    void emitFor(const ForStmt& s){loops_.push_back({});if(s.init)emitStmt(s.init.get());int top=here();int jf=-1;if(s.cond){emitExpr(s.cond.get());jf=emit(OP_JMPF,0);}emitStmt(s.body.get());int contTarget=here();for(int c:loops_.back().continues)patch(c,contTarget);if(s.update)emitStmt(s.update.get());emit(OP_JMP,top);if(jf>=0)patch(jf,here());for(int b:loops_.back().breaks)patch(b,here());loops_.pop_back();}
    void emitSynchronized(const SynchronizedStmt& s){emitExpr(s.monitor.get());emit(OP_POP);emitBlock(*s.body);}
    void emitTry(const TryStmt& s){emitBlock(*s.body);if(s.finallyBlock)emitBlock(*s.finallyBlock);}
    void emitSwitch(const SwitchStmt& s){
        // Lower a switch to a dispatch chain: evaluate the selector once into a
        // temp local, test each label with ==, and jump to the matching case
        // body. Fall-through follows source order; `break` jumps to the end.
        loops_.push_back({});
        int sel=ctx_->nextSlot++; emitExpr(s.selector.get()); emit(OP_STOREL,sel);
        // Dispatch section: for each label, (selector==label) ? enter body.
        // Record one enter-jump per label, tagged with its owning case index.
        std::vector<int> enterJump; std::vector<int> enterCase; int defaultIdx=-1;
        for(size_t i=0;i<s.cases.size();++i){
            const auto& c=s.cases[i];
            if(c.isDefault){defaultIdx=(int)i;continue;}
            for(const auto& lab:c.labels){
                emit(OP_LOADL,sel); emitExpr(lab.get()); emit(OP_EQ);
                int jf=emit(OP_JMPF,0);          // no match -> next label test
                enterJump.push_back(emit(OP_JMP,0)); enterCase.push_back((int)i);
                patch(jf,here());
            }
        }
        int toDefault=emit(OP_JMP,0);            // no label matched
        // Case bodies, in source order; record each body's start address.
        std::vector<int> bodyStart(s.cases.size(),-1);
        for(size_t i=0;i<s.cases.size();++i){bodyStart[i]=here();for(const auto& st:s.cases[i].statements)emitStmt(st.get());}
        int endPos=here();
        patch(toDefault, defaultIdx>=0?bodyStart[defaultIdx]:endPos);
        for(size_t j=0;j<enterJump.size();++j)patch(enterJump[j],bodyStart[enterCase[j]]);
        for(int b:loops_.back().breaks)patch(b,endPos);
        loops_.pop_back();
    }
    void emitExpr(const Expr* e){
        if(auto x=dynamic_cast<const IntLit*>(e)){emit(OP_CONST,slvm_add_const_int(vm_,x->value));return;} if(auto x=dynamic_cast<const DoubleLit*>(e)){emit(OP_CONST,slvm_add_const_double(vm_,x->value));return;}
        if(auto x=dynamic_cast<const BoolLit*>(e)){emit(OP_CONST,slvm_add_const_bool(vm_,x->value?1:0));return;} if(auto x=dynamic_cast<const StrLit*>(e)){emit(OP_CONST,slvm_add_const_str(vm_,x->value.c_str()));return;}
        if(dynamic_cast<const NullLit*>(e)){emit(OP_CONST,addNullConst());return;} if(auto x=dynamic_cast<const VarExpr*>(e)){emitVar(*x);return;} if(auto x=dynamic_cast<const Unary*>(e)){emitUnary(*x);return;}
        if(auto x=dynamic_cast<const Binary*>(e)){emitBinary(*x);return;} if(auto x=dynamic_cast<const Call*>(e)){emitCall(*x);return;}
        if(auto x=dynamic_cast<const NewExpr*>(e)){emitNew(*x);return;} if(auto x=dynamic_cast<const MemberAccess*>(e)){emitMember(*x);return;}
        if(auto x=dynamic_cast<const MethodCall*>(e)){emitMethodCall(*x);return;}
        if(auto x=dynamic_cast<const AssignmentExpr*>(e)){emitAssignmentExpr(*x);return;}
        if(auto x=dynamic_cast<const ConditionalExpr*>(e)){emitConditional(*x);return;}
        if(auto x=dynamic_cast<const CastExpr*>(e)){emitExpr(x->operand.get());return;}
        if(auto x=dynamic_cast<const InstanceOfExpr*>(e)){emitExpr(x->value.get());emit(OP_POP);emit(OP_CONST,slvm_add_const_bool(vm_,0));return;}
        if(dynamic_cast<const SuperExpr*>(e)||dynamic_cast<const ThisExpr*>(e)){emit(OP_CONST,addNullConst());return;}
        if(auto x=dynamic_cast<const ArrayAccess*>(e)){emitExpr(x->base.get());emitExpr(x->index.get());emit(OP_POP);return;}
        if(auto x=dynamic_cast<const MethodReferenceExpr*>(e)){emitExpr(x->base.get());return;}
        throw std::runtime_error("Semantic error: unknown expression kind");
    }
    // Lower a (possibly compound) assignment expression. The result value is
    // left on the stack. Compound ops expand to `target = target <op> value`.
    void emitAssignmentExpr(const AssignmentExpr& a){
        std::string bin = a.op.size()==2 ? std::string(1,a.op[0]) : std::string();
        if(auto v=dynamic_cast<const VarExpr*>(a.target.get())){
            if(!bin.empty()){emitVar(*v);emitExpr(a.value.get());emitBinOp(bin);} else emitExpr(a.value.get());
            int slot=ctx_->slotOf(v->name);
            if(slot>=0){emit(OP_STOREL,slot);emit(OP_LOADL,slot);return;}
            int g=fieldSlot(v->name);
            if(g>=0){if(fieldProtected_[v->name]&&fieldOwner_[v->name]!=currentClass_)throw std::runtime_error("protected field access denied");emit(OP_STOREG,g);emit(OP_LOADG,g);return;}
            throw std::runtime_error("Semantic error: assignment to undeclared variable '"+v->name+"'");
        }
        if(auto m=dynamic_cast<const MemberAccess*>(a.target.get())){
            int off=-1;memberLayout(m->base.get(),m->field,off);
            // SETFIELD expects [struct, newval] and leaves newval on the stack.
            emitExpr(m->base.get());                 // [struct]
            if(!bin.empty()){emit(OP_DUP);emit(OP_GETFIELD,off);emitExpr(a.value.get());emitBinOp(bin);} // [struct, newval]
            else emitExpr(a.value.get());            // [struct, newval]
            emit(OP_SETFIELD,off);                   // [newval]
            return;
        }
        throw std::runtime_error("Semantic error: assignment target must be a variable or a struct field");
    }
    void emitConditional(const ConditionalExpr& c){emitExpr(c.cond.get());int jf=emit(OP_JMPF,0);emitExpr(c.thenE.get());int jend=emit(OP_JMP,0);patch(jf,here());emitExpr(c.elseE.get());patch(jend,here());}
    void emitBinOp(const std::string& o){if(o=="+")emit(OP_ADD);else if(o=="-")emit(OP_SUB);else if(o=="*")emit(OP_MUL);else if(o=="/")emit(OP_DIV);else if(o=="%")emit(OP_MOD);else throw std::runtime_error("Semantic error: unsupported compound assignment operator '"+o+"='");}
    // Determine the struct type name an expression evaluates to, or "" if it is
    // not statically known to be a struct. Used to resolve field offsets.
    std::string exprStructType(const Expr* e){
        if(auto v=dynamic_cast<const VarExpr*>(e)){auto it=varType_.find(v->name);return it==varType_.end()?"":it->second;}
        if(auto m=dynamic_cast<const MemberAccess*>(e)){std::string bt=exprStructType(m->base.get());if(bt.empty())return "";auto lit=structLayout_.find(bt);if(lit==structLayout_.end())return "";int idx=-1;auto oit=lit->second.fieldOffset.find(m->field);if(oit!=lit->second.fieldOffset.end())idx=oit->second;if(idx<0)return "";return lit->second.fieldType[idx];}
        if(auto n=dynamic_cast<const NewExpr*>(e))return n->typeName;
        return "";
    }
    // Resolve (layout, offset) for base.field; throws a clear semantic error if
    // the base's struct type or the field cannot be determined at compile time.
    const StructLayout& memberLayout(const Expr* base,const std::string& field,int& offset){
        std::string tn=exprStructType(base);
        if(tn.empty()) throw std::runtime_error("Semantic error: cannot access field '"+field+"' -- the value is not a known struct type (declare the variable with its struct type)");
        auto lit=structLayout_.find(tn);
        if(lit==structLayout_.end()) throw std::runtime_error("Semantic error: '"+tn+"' is not a struct type");
        auto oit=lit->second.fieldOffset.find(field);
        if(oit==lit->second.fieldOffset.end()) throw std::runtime_error("Semantic error: struct '"+tn+"' has no field '"+field+"'");
        offset=oit->second; return lit->second;
    }
    void emitNew(const NewExpr& n){auto it=structLayout_.find(n.typeName);if(it==structLayout_.end())throw std::runtime_error("Semantic error: 'new' of unknown struct '"+n.typeName+"'");emit(OP_NEWSTRUCT,it->second.typeIndex);}
    void emitMember(const MemberAccess& m){
        // Back-propagate the terminal degree requirement to the origin.
        // next.next is the Degree-2 proposal. When that proposal is itself
        // static, its viewpoint extends by another two degrees to Degree 4.
        // The four-step form is therefore explicit and bounded.
        if(auto base=dynamic_cast<const VarExpr*>(m.base.get())){
            if(base->name=="next" && m.field=="next"){
                emit(OP_CONST,slvm_add_const_int(vm_,kProtectedSystemDegree));
                return;
            }
        }
        if(auto a=dynamic_cast<const MemberAccess*>(m.base.get())){
            if(auto b=dynamic_cast<const MemberAccess*>(a->base.get())){
                if(auto root=dynamic_cast<const VarExpr*>(b->base.get())){
                    if(root->name=="next" && b->field=="next" &&
                       a->field=="next" && m.field=="next"){
                        emit(OP_CONST,slvm_add_const_int(vm_,kStaticExtensionViewpointDegree));
                        return;
                    }
                }
            }
        }
        int off=-1;memberLayout(m.base.get(),m.field,off);emitExpr(m.base.get());emit(OP_GETFIELD,off);
    }
    // Lower a fluent `.method(args)` call. Munction reach verbs consume the
    // receiver's reach handle and push it back (so the chain keeps flowing);
    // closeWithReceipt/reception push a String. This is the source surface for
    // the reach-composition sentence (syntax 1.3).
    void emitMethodCall(const MethodCall& mc){
        const std::string& m=mc.method;
        auto oneArgStr=[&](const char* verb){ if(mc.args.size()!=1) throw std::runtime_error("Semantic error: Munction "+std::string(verb)+"(...) takes exactly one argument"); };
        auto noArg=[&](const char* verb){ if(!mc.args.empty()) throw std::runtime_error("Semantic error: Munction "+std::string(verb)+"() takes no arguments"); };
        // The reach opener `Munction.start(name)` begins a chain and yields a
        // reach handle. It parses as MethodCall(VarExpr("Munction"), "start").
        if(m=="start"){
            if(auto recv=dynamic_cast<const VarExpr*>(mc.receiver.get())){
                if(recv->name=="Munction"){
                    if(syntax_<SyntaxVersion{1,3}) throw std::runtime_error("Semantic error: Munction requires #sleela 1.3");
                    oneArgStr("start"); emitExpr(mc.args[0].get()); emit(OP_MUN_START); return;
                }
            }
        }
        // The Munction reach verbs. Each expects the receiver to evaluate to a
        // reach handle; the op leaves the handle (or a String) on the stack.
        if(m=="connect"||m=="enable"||m=="send"||m=="thatch"||m=="consume"||m=="latch"||m=="closeWithReceipt"||m=="close"||m=="reception"){
            if(syntax_<SyntaxVersion{1,3}) throw std::runtime_error("Semantic error: Munction requires #sleela 1.3");
            emitExpr(mc.receiver.get());
            if(m=="connect"){oneArgStr("connect");emitExpr(mc.args[0].get());emit(OP_MUN_CONNECT);return;}
            if(m=="enable"){oneArgStr("enable");emitExpr(mc.args[0].get());emit(OP_MUN_ENABLE);return;}
            if(m=="send"){oneArgStr("send");emitExpr(mc.args[0].get());emit(OP_MUN_SEND);return;}
            if(m=="thatch"){oneArgStr("thatch");emitExpr(mc.args[0].get());emit(OP_MUN_THATCH);return;}
            if(m=="consume"){noArg("consume");emit(OP_MUN_CONSUME);return;}
            if(m=="latch"){noArg("latch");emit(OP_MUN_LATCH);return;}
            if(m=="reception"){noArg("reception");emit(OP_MUN_RECEPTION);return;}
            /* close / closeWithReceipt */ noArg(m.c_str());emit(OP_MUN_CLOSE);return;
        }
        throw std::runtime_error("Semantic error: unknown fluent method '."+m+"()' (Munction verbs: connect/enable/send/thatch/consume/latch/closeWithReceipt/reception)");
    }
    void emitVar(const VarExpr& v){
        // `next` is the one-step system relation (System Degree 1).
        if(v.name=="next"){ emit(OP_CONST,slvm_add_const_int(vm_,kNextSystemDegree)); return; }
        int slot=ctx_->slotOf(v.name);if(slot>=0){emit(OP_LOADL,slot);return;}int g=fieldSlot(v.name);if(g>=0){if(fieldProtected_[v.name] && fieldOwner_[v.name]!=currentClass_) throw std::runtime_error("protected field access denied");emit(OP_LOADG,g);return;}throw std::runtime_error("Semantic error: use of undeclared variable '"+v.name+"'");
    }
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

        if(n=="structPack"||n=="structUnpack"){
            if(syntax_<SyntaxVersion{1,2})throw std::runtime_error("Semantic error: struct transport built-ins require #sleela 1.2");
        }
        if(n=="structPack"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: structPack(instance) takes exactly one argument");emitExpr(c.args[0].get());emit(OP_STRUCTPACK);return true;}
        if(n=="structUnpack"){
            if(c.args.size()!=2)throw std::runtime_error("Semantic error: structUnpack(TypeName, json) takes exactly two arguments");
            auto typeVar=dynamic_cast<const VarExpr*>(c.args[0].get());
            if(!typeVar)throw std::runtime_error("Semantic error: structUnpack first argument must be a struct type name");
            auto it=structLayout_.find(typeVar->name);
            if(it==structLayout_.end())throw std::runtime_error("Semantic error: structUnpack of unknown struct type '"+typeVar->name+"'");
            emitExpr(c.args[1].get());emit(OP_STRUCTUNPACK,it->second.typeIndex);return true;
        }

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
        if(n=="timeUtcNanos"){if(!c.args.empty())throw std::runtime_error("Semantic error: timeUtcNanos() takes no arguments");emit(OP_TIME_UTC_NS);return true;}
        if(n=="timeMonotonicNanos"){if(!c.args.empty())throw std::runtime_error("Semantic error: timeMonotonicNanos() takes no arguments");emit(OP_TIME_MONO_NS);return true;}
        if(n=="timePrecisionMillis"){if(!c.args.empty())throw std::runtime_error("Semantic error: timePrecisionMillis() takes no arguments");emit(OP_TIME_PRECISION_MS);return true;}
        if(n=="timeLocation"){if(!c.args.empty())throw std::runtime_error("Semantic error: timeLocation() takes no arguments");emit(OP_TIME_LOCATION);return true;}
        if(n=="timeHttpDate"){if(!c.args.empty())throw std::runtime_error("Semantic error: timeHttpDate() takes no arguments");emit(OP_TIME_HTTP_DATE);return true;}
        if(n=="timeJson"){if(!c.args.empty())throw std::runtime_error("Semantic error: timeJson() takes no arguments");emit(OP_TIME_JSON);return true;}
        if(n=="timeNtp"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: timeNtp(host) takes one argument");emitExpr(c.args[0].get());emit(OP_TIME_NTP);return true;}
        if(n=="timeSetLocation"){if(c.args.size()!=2)throw std::runtime_error("Semantic error: timeSetLocation(country, timezone) takes two arguments");emitExpr(c.args[0].get());emitExpr(c.args[1].get());emit(OP_TIME_SET_LOCATION);return true;}

        // ---- Munction (syntax 1.3): the reach-composition opener. ----
        // Munction.start(name) opens a reach and yields a reach handle; the
        // fluent verbs (.connect/.send/...) are lowered by emitMethodCall.
        if(n=="Munction.start"){
            if(syntax_<SyntaxVersion{1,3})throw std::runtime_error("Semantic error: Munction requires #sleela 1.3");
            if(c.args.size()!=1)throw std::runtime_error("Semantic error: Munction.start(name) takes exactly one argument");
            emitExpr(c.args[0].get());emit(OP_MUN_START);return true;
        }

        // ---- Synchro (syntax 1.3): honest packet dispatch + measurement. ----
        if(n=="synchroOpen"||n=="synchroDispatch"||n=="synchroReport"||n=="synchroClose"||
           n=="synchroSent"||n=="synchroReceived"||n=="synchroMean"||n=="synchroMin"||
           n=="synchroMax"||n=="synchroP95"||n=="synchroLoss"){
            if(syntax_<SyntaxVersion{1,3})throw std::runtime_error("Semantic error: Synchro built-ins require #sleela 1.3");
        }
        if(n=="synchroOpen"){if(c.args.size()!=2)throw std::runtime_error("Semantic error: synchroOpen(host, port) takes two arguments");emitExpr(c.args[0].get());emitExpr(c.args[1].get());emit(OP_SYN_OPEN);return true;}
        if(n=="synchroDispatch"){if(c.args.size()!=3)throw std::runtime_error("Semantic error: synchroDispatch(handle, len, timeoutMs) takes three arguments");emitExpr(c.args[0].get());emitExpr(c.args[1].get());emitExpr(c.args[2].get());emit(OP_SYN_DISPATCH);return true;}
        if(n=="synchroReport"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: synchroReport(handle) takes one argument");emitExpr(c.args[0].get());emit(OP_SYN_REPORT);return true;}
        if(n=="synchroClose"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: synchroClose(handle) takes one argument");emitExpr(c.args[0].get());emit(OP_SYN_CLOSE);return true;}
        {
            struct { const char* name; int sel; } synStats[] = {
                {"synchroSent",0},{"synchroReceived",1},{"synchroMean",2},{"synchroMin",3},
                {"synchroMax",4},{"synchroP95",5},{"synchroLoss",6}
            };
            for(const auto& st:synStats){ if(n==st.name){ if(c.args.size()!=1)throw std::runtime_error("Semantic error: "+n+"(handle) takes one argument");emitExpr(c.args[0].get());emit(OP_SYN_STAT,st.sel);return true; } }
        }

        // ---- Best-of (syntax 1.3): configurable route/accuracy selection ----
        if(n=="bestOfNew"||n=="bestOfWeight"||n=="bestOfMinVersion"||n=="bestOfCostBudget"||
           n=="bestOfCandidate"||n=="bestOfRecord"||n=="bestOfScore"||n=="bestOfBest"||
           n=="bestOfChoice"||n=="bestOfReport"||n=="bestOfClose"||
           n=="bestOfMean"||n=="bestOfLoss"||n=="bestOfJitter"||n=="bestOfCertainty"||
           n=="bestOfCandidateArch"||n=="bestOfArchRealized"||
           n=="bestOfArch"||n=="bestOfArchParam"||n=="bestOfArchState"){
            if(syntax_<SyntaxVersion{1,3})throw std::runtime_error("Semantic error: best-of built-ins require #sleela 1.3");
        }
        auto emitArgs=[&](const Call& call){ for(const auto& a:call.args) emitExpr(a.get()); };
        if(n=="audioNew"){
            if(c.args.size()!=2)throw std::runtime_error("Semantic error: audioNew(sampleRate, outputPath) takes two arguments");
            emitExpr(c.args[0].get());emitExpr(c.args[1].get());emit(OP_AUDIO_NEW);return true;
        }
        if(n=="audioAdd"){
            if(c.args.size()!=4)throw std::runtime_error("Semantic error: audioAdd(handle, path, startSeconds, gainDb) takes four arguments");
            emitArgs(c);emit(OP_AUDIO_ADD);return true;
        }
        if(n=="audioControls"){
            if(c.args.size()!=8)throw std::runtime_error("Semantic error: audioControls(handle, bass, mid, treble, masterGain, pan, leftGain, rightGain) takes eight arguments");
            emitArgs(c);emit(OP_AUDIO_CONTROLS);return true;
        }
        if(n=="audioValidate"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: audioValidate(handle) takes one argument");emitExpr(c.args[0].get());emit(OP_AUDIO_VALIDATE);return true;}
        if(n=="audioRender"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: audioRender(handle) takes one argument");emitExpr(c.args[0].get());emit(OP_AUDIO_RENDER);return true;}
        if(n=="audioClose"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: audioClose(handle) takes one argument");emitExpr(c.args[0].get());emit(OP_AUDIO_CLOSE);return true;}
        if(n=="audioPlatform"){if(!c.args.empty())throw std::runtime_error("Semantic error: audioPlatform() takes no arguments");emit(OP_AUDIO_PLATFORM);return true;}

        if(n=="bestOfNew"){if(!c.args.empty())throw std::runtime_error("Semantic error: bestOfNew() takes no arguments");emit(OP_BEST_NEW);return true;}
        if(n=="bestOfWeight"){if(c.args.size()!=3)throw std::runtime_error("Semantic error: bestOfWeight(handle, axis, weight) takes three arguments");emitArgs(c);emit(OP_BEST_WEIGHT);return true;}
        if(n=="bestOfMinVersion"){if(c.args.size()!=2)throw std::runtime_error("Semantic error: bestOfMinVersion(handle, minVersion) takes two arguments");emitArgs(c);emit(OP_BEST_MINVER);return true;}
        if(n=="bestOfCostBudget"){if(c.args.size()!=2)throw std::runtime_error("Semantic error: bestOfCostBudget(handle, budget) takes two arguments");emitArgs(c);emit(OP_BEST_BUDGET);return true;}
        if(n=="bestOfCandidate"){if(c.args.size()!=10)throw std::runtime_error("Semantic error: bestOfCandidate(handle, name, route, timeoutMs, payloadLen, gapMs, flags, version, cost, replays) takes ten arguments");emitArgs(c);emit(OP_BEST_CAND);return true;}
        if(n=="bestOfRecord"){if(c.args.size()!=3)throw std::runtime_error("Semantic error: bestOfRecord(handle, idx, rttUs) takes three arguments");emitArgs(c);emit(OP_BEST_RECORD);return true;}
        if(n=="bestOfScore"){if(c.args.size()!=2)throw std::runtime_error("Semantic error: bestOfScore(handle, idx) takes two arguments");emitArgs(c);emit(OP_BEST_SCORE);return true;}
        if(n=="bestOfBest"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: bestOfBest(handle) takes one argument");emitArgs(c);emit(OP_BEST_BEST);return true;}
        if(n=="bestOfChoice"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: bestOfChoice(handle) takes one argument");emitArgs(c);emit(OP_BEST_CHOICE);return true;}
        if(n=="bestOfReport"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: bestOfReport(handle) takes one argument");emitArgs(c);emit(OP_BEST_REPORT);return true;}
        if(n=="bestOfCandidateArch"){if(c.args.size()!=5)throw std::runtime_error("Semantic error: bestOfCandidateArch(handle, idx, architecture, param, realized) takes five arguments");emitArgs(c);emit(OP_BEST_ARCH);return true;}
        if(n=="bestOfArchRealized"){if(c.args.size()!=3)throw std::runtime_error("Semantic error: bestOfArchRealized(handle, idx, realized) takes three arguments");emitArgs(c);emit(OP_BEST_ARCH_STATE);return true;}
        if(n=="bestOfClose"){if(c.args.size()!=1)throw std::runtime_error("Semantic error: bestOfClose(handle) takes one argument");emitArgs(c);emit(OP_BEST_CLOSE);return true;}
        {
            struct { const char* name; int sel; } bestStats[] = {
                {"bestOfMean",0},{"bestOfLoss",1},{"bestOfJitter",2},{"bestOfCertainty",3},
                {"bestOfArch",4},{"bestOfArchParam",5},{"bestOfArchState",6}
            };
            for(const auto& st:bestStats){ if(n==st.name){ if(c.args.size()!=2)throw std::runtime_error("Semantic error: "+n+"(handle, idx) takes two arguments");emitArgs(c);emit(OP_BEST_STAT,st.sel);return true; } }
        }

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
    void emitCall(const Call& c){ if(protectedMethods_.count(c.callee) && methodOwner_[c.callee]!=currentClass_) throw std::runtime_error("protected method access denied");
        if(tryEmitBuiltin(c))return; auto it=funcIndex_.find(c.callee); if(it==funcIndex_.end())throw std::runtime_error("Semantic error: call to unknown method '"+c.callee+"'");
        const Method* target=methods_[it->second].method; if((int)c.args.size()!=(int)target->params.size())throw std::runtime_error("Semantic error: method '"+c.callee+"' expects "+std::to_string(target->params.size())+" argument(s), got "+std::to_string(c.args.size()));
        for(const auto&a:c.args)emitExpr(a.get());emit(OP_CALL,it->second);
    }
};

} // anonymous namespace

int compile(const Program& prog, SLVM* vm, const catalog::Catalog* cat, const SyntaxVersion& syntax){
    SemanticResult semantic = analyzeSemantics(prog, syntax);
    if(!semantic.ok()){
        throw std::runtime_error(semantic.errors.front());
    }
    Compiler c(prog,vm,cat,syntax);
    return c.run();
}

} // namespace sleela
