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

// Per-method compilation state: maps local variable names to frame slots.
struct MethodCtx {
    std::map<std::string, int> locals;   // name -> slot index
    int nextSlot = 0;

    int slotOf(const std::string& name) const {
        auto it = locals.find(name);
        return it == locals.end() ? -1 : it->second;
    }
    int declare(const std::string& name) {
        auto it = locals.find(name);
        if (it != locals.end()) return it->second;   // shadow/redeclare -> reuse
        int s = nextSlot++;
        locals[name] = s;
        return s;
    }
};

class Compiler {
public:
    Compiler(const Program& prog, SLVM* vm) : prog_(prog), vm_(vm) {}

    int run() {
        // Pass 0: declare a core global for every class field. Fields are
        // shared state (thread-safe globals), so threads see the same value.
        for (const auto& cls : prog_.classes) {
            for (const auto& f : cls.fields) {
                if (fieldGlobal_.count(f.name))
                    throw std::runtime_error("Semantic error: duplicate field '" + f.name + "'");
                fieldGlobal_[f.name] = slvm_declare_global(vm_, f.name.c_str());
                fields_.push_back(&f);
            }
        }

        // Pass 1: assign a core function index to every method and count locals.
        // We flatten all methods across all classes into the core function table.
        for (const auto& cls : prog_.classes) {
            for (const auto& m : cls.methods) {
                MethodInfo mi;
                mi.method = &m;
                mi.nlocals = countLocals(m);
                // reserve the function index (entry ip is set by begin_func at emit time)
                funcIndex_[m.name] = (int)methods_.size();
                methods_.push_back(mi);
            }
        }
        if (funcIndex_.find("main") == funcIndex_.end())
            throw std::runtime_error("Semantic error: no 'main' method found");

        // Pass 2: emit each method's body.
        for (auto& mi : methods_) emitMethod(mi);

        int entry = funcIndex_["main"];
        slvm_set_entry(vm_, entry);
        return entry;
    }

private:
    struct MethodInfo {
        const Method* method;
        int nlocals;
    };

    const Program& prog_;
    SLVM* vm_;
    std::map<std::string, int> funcIndex_;   // method name -> core func index
    std::vector<MethodInfo> methods_;
    std::map<std::string, int> fieldGlobal_; // field name -> core global slot
    std::vector<const Field*> fields_;       // fields, in declaration order
    MethodCtx* ctx_ = nullptr;

    int fieldSlot(const std::string& name) const {
        auto it = fieldGlobal_.find(name);
        return it == fieldGlobal_.end() ? -1 : it->second;
    }

    // ---- local counting (params + all declared vars) --------------------
    int countLocals(const Method& m) {
        int n = (int)m.params.size();
        countInBlock(*m.body, n);
        return n;
    }
    void countInBlock(const Block& b, int& n) {
        for (const auto& s : b.stmts) countInStmt(s.get(), n);
    }
    void countInStmt(const Stmt* s, int& n) {
        if (auto d = dynamic_cast<const VarDecl*>(s)) { (void)d; n++; }
        else if (auto b = dynamic_cast<const Block*>(s)) countInBlock(*b, n);
        else if (auto i = dynamic_cast<const IfStmt*>(s)) {
            countInStmt(i->thenS.get(), n);
            if (i->elseS) countInStmt(i->elseS.get(), n);
        }
        else if (auto w = dynamic_cast<const WhileStmt*>(s)) countInStmt(w->body.get(), n);
        else if (auto f = dynamic_cast<const ForStmt*>(s)) {
            if (f->init) countInStmt(f->init.get(), n);
            countInStmt(f->body.get(), n);
        }
    }

    // ---- emission helpers ----------------------------------------------
    int here() { return slvm_here(vm_); }
    int emit(SLOp op, int a = 0) { return slvm_emit(vm_, op, a); }
    void patch(int at, int target) { slvm_patch(vm_, at, target); }

    // ---- per-method emission -------------------------------------------
    void emitMethod(MethodInfo& mi) {
        const Method& m = *mi.method;
        MethodCtx ctx;
        // params occupy the first slots, in order
        for (const auto& p : m.params) ctx.declare(p.name);

        // begin_func records the entry ip = current code length
        int nargs = (int)m.params.size();
        slvm_begin_func(vm_, m.name.c_str(), nargs, mi.nlocals);

        ctx_ = &ctx;
        // Initialize shared fields once, at the top of main().
        if (m.name == "main") {
            for (const Field* f : fields_) {
                if (f->init) emitExpr(f->init.get());
                else         emit(OP_CONST, addNullConst());
                emit(OP_STOREG, fieldGlobal_[f->name]);
            }
        }
        emitBlock(*m.body);
        ctx_ = nullptr;

        // implicit return (for void methods / fall-off): push null, RET
        emit(OP_CONST, addNullConst());
        emit(OP_RET);
        slvm_end_func(vm_);
    }

    int addNullConst() {
        // represent null as a dedicated const; reuse a cached one
        if (nullConst_ < 0) {
            SLExchangeArg a{}; a.value = slval_null();
            slcore_exchange(vm_, SLX_ADD_CONST, &a);
            nullConst_ = a.out;
        }
        return nullConst_;
    }
    int nullConst_ = -1;

    // ---- statements -----------------------------------------------------
    void emitStmt(const Stmt* s) {
        if (auto b = dynamic_cast<const Block*>(s))      { emitBlock(*b); return; }
        if (auto d = dynamic_cast<const VarDecl*>(s))    { emitVarDecl(*d); return; }
        if (auto a = dynamic_cast<const Assign*>(s))     { emitAssign(*a); return; }
        if (auto e = dynamic_cast<const ExprStmt*>(s))   { emitExpr(e->expr.get()); emit(OP_POP); return; }
        if (auto p = dynamic_cast<const PrintStmt*>(s))  { emitExpr(p->expr.get()); emit(OP_PRINT); return; }
        if (auto r = dynamic_cast<const ReturnStmt*>(s)) { emitReturn(*r); return; }
        if (auto i = dynamic_cast<const IfStmt*>(s))     { emitIf(*i); return; }
        if (auto w = dynamic_cast<const WhileStmt*>(s))  { emitWhile(*w); return; }
        if (auto f = dynamic_cast<const ForStmt*>(s))    { emitFor(*f); return; }
        throw std::runtime_error("Semantic error: unknown statement kind");
    }

    void emitBlock(const Block& b) { for (const auto& s : b.stmts) emitStmt(s.get()); }

    void emitVarDecl(const VarDecl& d) {
        int slot = ctx_->declare(d.name);
        if (d.init) emitExpr(d.init.get());
        else        emit(OP_CONST, addNullConst());
        emit(OP_STOREL, slot);
    }

    void emitAssign(const Assign& a) {
        int slot = ctx_->slotOf(a.name);
        if (slot >= 0) {                       // a local (shadows any field)
            emitExpr(a.value.get());
            emit(OP_STOREL, slot);
            return;
        }
        int g = fieldSlot(a.name);             // otherwise a shared field
        if (g >= 0) {
            emitExpr(a.value.get());
            emit(OP_STOREG, g);
            return;
        }
        throw std::runtime_error("Semantic error: assignment to undeclared variable '" + a.name + "'");
    }

    void emitReturn(const ReturnStmt& r) {
        if (r.value) emitExpr(r.value.get());
        else         emit(OP_CONST, addNullConst());
        emit(OP_RET);
    }

    void emitIf(const IfStmt& s) {
        emitExpr(s.cond.get());
        int jf = emit(OP_JMPF, 0);        // to else/end
        emitStmt(s.thenS.get());
        if (s.elseS) {
            int jend = emit(OP_JMP, 0);   // over else
            patch(jf, here());
            emitStmt(s.elseS.get());
            patch(jend, here());
        } else {
            patch(jf, here());
        }
    }

    void emitWhile(const WhileStmt& s) {
        int top = here();
        emitExpr(s.cond.get());
        int jf = emit(OP_JMPF, 0);
        emitStmt(s.body.get());
        emit(OP_JMP, top);
        patch(jf, here());
    }

    void emitFor(const ForStmt& s) {
        if (s.init) emitStmt(s.init.get());
        int top = here();
        int jf = -1;
        if (s.cond) { emitExpr(s.cond.get()); jf = emit(OP_JMPF, 0); }
        emitStmt(s.body.get());
        if (s.update) emitStmt(s.update.get());
        emit(OP_JMP, top);
        if (jf >= 0) patch(jf, here());
    }

    // ---- expressions ----------------------------------------------------
    void emitExpr(const Expr* e) {
        if (auto x = dynamic_cast<const IntLit*>(e))    { emit(OP_CONST, slvm_add_const_int(vm_, x->value)); return; }
        if (auto x = dynamic_cast<const DoubleLit*>(e)) { emit(OP_CONST, slvm_add_const_double(vm_, x->value)); return; }
        if (auto x = dynamic_cast<const BoolLit*>(e))   { emit(OP_CONST, slvm_add_const_bool(vm_, x->value ? 1 : 0)); return; }
        if (auto x = dynamic_cast<const StrLit*>(e))    { emit(OP_CONST, slvm_add_const_str(vm_, x->value.c_str())); return; }
        if (dynamic_cast<const NullLit*>(e))            { emit(OP_CONST, addNullConst()); return; }
        if (auto x = dynamic_cast<const VarExpr*>(e))   { emitVar(*x); return; }
        if (auto x = dynamic_cast<const Unary*>(e))     { emitUnary(*x); return; }
        if (auto x = dynamic_cast<const Binary*>(e))    { emitBinary(*x); return; }
        if (auto x = dynamic_cast<const Call*>(e))      { emitCall(*x); return; }
        throw std::runtime_error("Semantic error: unknown expression kind");
    }

    void emitVar(const VarExpr& v) {
        int slot = ctx_->slotOf(v.name);
        if (slot >= 0) { emit(OP_LOADL, slot); return; }   // local (shadows field)
        int g = fieldSlot(v.name);
        if (g >= 0) { emit(OP_LOADG, g); return; }         // shared field
        throw std::runtime_error("Semantic error: use of undeclared variable '" + v.name + "'");
    }

    void emitUnary(const Unary& u) {
        emitExpr(u.operand.get());
        if (u.op == "-") emit(OP_NEG);
        else if (u.op == "!") emit(OP_NOT);
        else throw std::runtime_error("Semantic error: unknown unary operator '" + u.op + "'");
    }

    void emitBinary(const Binary& b) {
        // short-circuit && / || : we implement as strict here (both evaluated)
        // via AND/OR opcodes, which is fine for boolean operands.
        emitExpr(b.lhs.get());
        emitExpr(b.rhs.get());
        const std::string& o = b.op;
        if      (o == "+")  emit(OP_ADD);
        else if (o == "-")  emit(OP_SUB);
        else if (o == "*")  emit(OP_MUL);
        else if (o == "/")  emit(OP_DIV);
        else if (o == "%")  emit(OP_MOD);
        else if (o == "==") emit(OP_EQ);
        else if (o == "!=") emit(OP_NE);
        else if (o == "<")  emit(OP_LT);
        else if (o == "<=") emit(OP_LE);
        else if (o == ">")  emit(OP_GT);
        else if (o == ">=") emit(OP_GE);
        else if (o == "&&") emit(OP_AND);
        else if (o == "||") emit(OP_OR);
        else throw std::runtime_error("Semantic error: unknown binary operator '" + o + "'");
    }

    // Threading built-ins are recognized by callee name. They lower directly
    // to the core's concurrency opcodes, so no runtime library is needed.
    //   spawn(methodName)     start methodName on a new thread; yields a thread id
    //   join()                wait for all spawned threads
    //   lock(n) / unlock(n)   acquire / release lock table slot n (compile-time int)
    //   send(slot, value)     send the 2-tuple (slot, value) onto the burble line
    //   recv(slot)            block for a tuple on slot; yields its value
    // Returns true if `c` was a built-in and was emitted.
    bool tryEmitBuiltin(const Call& c) {
        const std::string& n = c.callee;

        auto litInt = [&](const Expr* e, const char* what) -> int {
            auto il = dynamic_cast<const IntLit*>(e);
            if (!il)
                throw std::runtime_error("Semantic error: " + std::string(what) +
                    " must be an integer literal");
            return (int)il->value;
        };

        if (n == "spawn") {
            if (c.args.size() != 1)
                throw std::runtime_error("Semantic error: spawn(method) takes exactly one argument");
            auto var = dynamic_cast<const VarExpr*>(c.args[0].get());
            if (!var)
                throw std::runtime_error("Semantic error: spawn(method) argument must be a method name");
            auto it = funcIndex_.find(var->name);
            if (it == funcIndex_.end())
                throw std::runtime_error("Semantic error: spawn of unknown method '" + var->name + "'");
            if (!methods_[it->second].method->params.empty())
                throw std::runtime_error("Semantic error: spawn target '" + var->name +
                    "' must take no parameters");
            emit(OP_SPAWN, it->second);   // pushes a thread id
            return true;
        }
        if (n == "join") {
            if (!c.args.empty())
                throw std::runtime_error("Semantic error: join() takes no arguments");
            emit(OP_JOINALL);
            emit(OP_CONST, addNullConst());   // join() yields null (expression form)
            return true;
        }
        if (n == "lock" || n == "unlock") {
            if (c.args.size() != 1)
                throw std::runtime_error("Semantic error: " + n + "(id) takes exactly one argument");
            int id = litInt(c.args[0].get(), (n + " id").c_str());
            emit(n == "lock" ? OP_LOCK : OP_UNLOCK, id);
            emit(OP_CONST, addNullConst());   // yields null
            return true;
        }
        if (n == "send") {
            if (c.args.size() != 2)
                throw std::runtime_error("Semantic error: send(slot, value) takes two arguments");
            int slot = litInt(c.args[0].get(), "send slot");
            emitExpr(c.args[1].get());        // value on stack
            emit(OP_SEND, slot);
            emit(OP_CONST, addNullConst());   // yields null
            return true;
        }
        if (n == "recv") {
            if (c.args.size() != 1)
                throw std::runtime_error("Semantic error: recv(slot) takes exactly one argument");
            int slot = litInt(c.args[0].get(), "recv slot");
            emit(OP_RECV, slot);              // pushes the received value
            return true;
        }
        return false;
    }

    void emitCall(const Call& c) {
        if (tryEmitBuiltin(c)) return;

        auto it = funcIndex_.find(c.callee);
        if (it == funcIndex_.end())
            throw std::runtime_error("Semantic error: call to unknown method '" + c.callee + "'");
        const Method* target = methods_[it->second].method;
        if ((int)c.args.size() != (int)target->params.size())
            throw std::runtime_error("Semantic error: method '" + c.callee + "' expects " +
                std::to_string(target->params.size()) + " argument(s), got " +
                std::to_string(c.args.size()));
        for (const auto& a : c.args) emitExpr(a.get());   // push args left-to-right
        emit(OP_CALL, it->second);
    }
};

} // anonymous namespace

int compile(const Program& prog, SLVM* vm) {
    Compiler c(prog, vm);
    return c.run();
}

} // namespace sleela
