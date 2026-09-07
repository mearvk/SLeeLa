// ===========================================================================
// mem_model.cpp -- AST -> stp_mem_model analysis.
// ===========================================================================
#include "mem_model.h"

#include <set>
#include <string>

namespace sleela {
namespace {

struct Counter {
    unsigned instrs = 0;
    int spawnTargets = 0;
    std::set<std::string> spawnNames;
    std::set<long long> lockSlots;
    std::set<long long> mailSlots;

    // Try to read an integer literal argument (for lock/send/recv slot ids).
    static bool intArg(const Expr* e, long long& out) {
        if (auto* il = dynamic_cast<const IntLit*>(e)) { out = il->value; return true; }
        return false;
    }

    void expr(const Expr* e) {
        if (!e) return;
        instrs++;
        if (auto* b = dynamic_cast<const Binary*>(e)) { expr(b->lhs.get()); expr(b->rhs.get()); }
        else if (auto* u = dynamic_cast<const Unary*>(e)) { expr(u->operand.get()); }
        else if (auto* c = dynamic_cast<const Call*>(e)) {
            for (auto& a : c->args) expr(a.get());
            const std::string& n = c->callee;
            if (n == "spawn") {
                // spawn(method): the (single) arg is the target name (VarExpr).
                if (!c->args.empty()) {
                    if (auto* v = dynamic_cast<const VarExpr*>(c->args[0].get()))
                        spawnNames.insert(v->name);
                }
                spawnTargets++;
            } else if (n == "lock" || n == "unlock") {
                long long slot;
                if (!c->args.empty() && intArg(c->args[0].get(), slot)) lockSlots.insert(slot);
            } else if (n == "send" || n == "recv") {
                long long slot;
                if (!c->args.empty() && intArg(c->args[0].get(), slot)) mailSlots.insert(slot);
            }
        }
    }

    void stmt(const Stmt* s) {
        if (!s) return;
        instrs++;
        if (auto* b = dynamic_cast<const Block*>(s)) { for (auto& st : b->stmts) stmt(st.get()); }
        else if (auto* v = dynamic_cast<const VarDecl*>(s)) { expr(v->init.get()); }
        else if (auto* a = dynamic_cast<const Assign*>(s)) { expr(a->value.get()); }
        else if (auto* e = dynamic_cast<const ExprStmt*>(s)) { expr(e->expr.get()); }
        else if (auto* p = dynamic_cast<const PrintStmt*>(s)) { expr(p->expr.get()); instrs++; }
        else if (auto* r = dynamic_cast<const ReturnStmt*>(s)) { expr(r->value.get()); }
        else if (auto* i = dynamic_cast<const IfStmt*>(s)) { expr(i->cond.get()); stmt(i->thenS.get()); stmt(i->elseS.get()); }
        else if (auto* w = dynamic_cast<const WhileStmt*>(s)) { expr(w->cond.get()); stmt(w->body.get()); }
        else if (auto* f = dynamic_cast<const ForStmt*>(s)) { stmt(f->init.get()); expr(f->cond.get()); stmt(f->update.get()); stmt(f->body.get()); }
    }
};

} // namespace

stp_mem_model analyze_mem_model(const Program& prog, long long sysdepth, long long degreemax) {
    stp_mem_model mm;
    mm.globals = mm.functions = mm.code_len = mm.max_threads = mm.locks = mm.mailboxes = 0;
    mm.est_heap = 0;
    mm.sysdepth = sysdepth;
    mm.degreemax = degreemax;

    Counter c;
    for (const auto& cls : prog.classes) {
        mm.globals += (unsigned) cls.fields.size();
        mm.functions += (unsigned) cls.methods.size();
        for (const auto& fld : cls.fields) c.expr(fld.init.get());
        for (const auto& m : cls.methods) if (m.body) c.stmt(m.body.get());
    }

    mm.code_len = c.instrs;
    // The main thread plus one line of execution per distinct spawn target,
    // bounded by the core's 128-thread cap.
    unsigned threads = 1u + (unsigned) c.spawnNames.size();
    if (c.spawnNames.empty() && c.spawnTargets > 0) threads = 1u + (unsigned) c.spawnTargets;
    if (threads > 128) threads = 128;
    mm.max_threads = threads;
    mm.locks = (unsigned) c.lockSlots.size();
    mm.mailboxes = (unsigned) c.mailSlots.size();

    // Rough heap estimate: 64 KiB base + 4 KiB per global + 8 KiB per function
    // + 256 KiB per potential thread (matches the core's per-thread stacks).
    mm.est_heap = (uint64_t) (64u << 10)
                + (uint64_t) mm.globals * (4u << 10)
                + (uint64_t) mm.functions * (8u << 10)
                + (uint64_t) mm.max_threads * (256u << 10);
    return mm;
}

} // namespace sleela
