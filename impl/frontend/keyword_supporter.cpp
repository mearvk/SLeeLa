// ===========================================================================
// keyword_supporter.cpp -- see keyword_supporter.h.
// ===========================================================================
#include "keyword_supporter.h"
#include "../../ledger/ledger_core.h"   // ledger::sha256_hex
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fstream>
#include <sstream>
#include <map>

namespace sleela::frontend {

const std::vector<std::string>& supporterKeywords() noexcept {
    static const std::vector<std::string> kw = {
        "try", "catch", "throw", "synchronized", "super"
    };
    return kw;
}

namespace {

// A short, stable digest over the identifying tuple. We keep 16 hex chars (64
// bits) -- long enough to avoid collisions across a repo, short enough for a
// readable folder name.
std::string makeHash(const std::string& keyword, const std::string& relPath,
                     const std::string& cls, const std::string& method, int ordinal) {
    std::string seed = keyword + "|" + relPath + "|" + cls + "::" + method + "|" +
                       std::to_string(ordinal);
    std::string full = ledger::sha256_hex(seed);
    return full.substr(0, 16);
}

// Per-method occurrence counters, so the ordinal (and therefore the hash) is
// deterministic for a given keyword within a given method regardless of how the
// walker recurses.
struct Counters {
    // keyword -> count seen so far in the current method
    int next(const std::string& keyword) { return counts_[keyword]++; }
private:
    std::map<std::string, int> counts_;
};

class Walker {
public:
    Walker(const std::string& relPath, std::vector<KeywordSupporterRecord>& out)
        : relPath_(relPath), out_(out) {}

    void walkProgram(const Program& prog) {
        for (const auto& cls : prog.classes) {
            for (const auto& m : cls.methods) {
                cls_ = cls.name;
                method_ = m.name.empty() ? std::string("<init>") : m.name;
                counters_ = Counters{};
                if (m.body) walkBlock(*m.body);
            }
        }
    }

private:
    void record(const std::string& keyword) {
        KeywordSupporterRecord r;
        r.keyword = keyword;
        r.enclosingClass = cls_;
        r.enclosingMethod = method_;
        r.ordinal = counters_.next(keyword);
        r.relSourcePath = relPath_;
        r.hash = makeHash(keyword, relPath_, cls_, method_, r.ordinal);
        out_.push_back(std::move(r));
    }

    void walkBlock(const Block& b) {
        for (const auto& s : b.stmts) walkStmt(s.get());
    }

    void walkStmt(const Stmt* s) {
        if (!s) return;
        // Keyword-bearing statements first, then recurse into their sub-trees.
        if (auto t = dynamic_cast<const TryStmt*>(s)) {
            record("try");
            if (t->body) walkBlock(*t->body);
            for (const auto& c : t->catches) { record("catch"); if (c.body) walkBlock(*c.body); }
            if (t->finallyBlock) walkBlock(*t->finallyBlock);
            return;
        }
        if (auto th = dynamic_cast<const ThrowStmt*>(s)) { record("throw"); walkExpr(th->value.get()); return; }
        if (auto sy = dynamic_cast<const SynchronizedStmt*>(s)) {
            record("synchronized");
            walkExpr(sy->monitor.get());
            if (sy->body) walkBlock(*sy->body);
            return;
        }
        // Non-keyword statements: recurse into nested statements/expressions so
        // keywords buried inside control flow are still discovered.
        if (auto b = dynamic_cast<const Block*>(s)) { walkBlock(*b); return; }
        if (auto v = dynamic_cast<const VarDecl*>(s)) { walkExpr(v->init.get()); return; }
        if (auto a = dynamic_cast<const Assign*>(s)) { walkExpr(a->value.get()); return; }
        if (auto fa = dynamic_cast<const FieldAssign*>(s)) { walkExpr(fa->base.get()); walkExpr(fa->value.get()); return; }
        if (auto es = dynamic_cast<const ExprStmt*>(s)) { walkExpr(es->expr.get()); return; }
        if (auto ps = dynamic_cast<const PrintStmt*>(s)) { walkExpr(ps->expr.get()); return; }
        if (auto rs = dynamic_cast<const ReturnStmt*>(s)) { walkExpr(rs->value.get()); return; }
        if (auto i = dynamic_cast<const IfStmt*>(s)) { walkExpr(i->cond.get()); walkStmt(i->thenS.get()); walkStmt(i->elseS.get()); return; }
        if (auto w = dynamic_cast<const WhileStmt*>(s)) { walkExpr(w->cond.get()); walkStmt(w->body.get()); return; }
        if (auto f = dynamic_cast<const ForStmt*>(s)) { walkStmt(f->init.get()); walkExpr(f->cond.get()); walkStmt(f->update.get()); walkStmt(f->body.get()); return; }
        if (auto d = dynamic_cast<const DoStmt*>(s)) { walkStmt(d->body.get()); walkExpr(d->cond.get()); return; }
        if (auto as = dynamic_cast<const AssertStmt*>(s)) { walkExpr(as->cond.get()); walkExpr(as->message.get()); return; }
        if (auto y = dynamic_cast<const YieldStmt*>(s)) { walkExpr(y->value.get()); return; }
        if (auto sw = dynamic_cast<const SwitchStmt*>(s)) {
            walkExpr(sw->selector.get());
            for (const auto& c : sw->cases) {
                for (const auto& lab : c.labels) walkExpr(lab.get());
                for (const auto& st : c.statements) walkStmt(st.get());
            }
            return;
        }
        // VarDecl-less / leaf statements (Break, Continue, GcHint): nothing to do.
    }

    void walkExpr(const Expr* e) {
        if (!e) return;
        if (dynamic_cast<const SuperExpr*>(e)) { record("super"); return; }
        if (auto u = dynamic_cast<const Unary*>(e)) { walkExpr(u->operand.get()); return; }
        if (auto b = dynamic_cast<const Binary*>(e)) { walkExpr(b->lhs.get()); walkExpr(b->rhs.get()); return; }
        if (auto c = dynamic_cast<const Call*>(e)) { for (const auto& a : c->args) walkExpr(a.get()); return; }
        if (auto mc = dynamic_cast<const MethodCall*>(e)) { walkExpr(mc->receiver.get()); for (const auto& a : mc->args) walkExpr(a.get()); return; }
        if (auto ne = dynamic_cast<const NewExpr*>(e)) { for (const auto& a : ne->args) walkExpr(a.get()); return; }
        if (auto ma = dynamic_cast<const MemberAccess*>(e)) { walkExpr(ma->base.get()); return; }
        if (auto ae = dynamic_cast<const AssignmentExpr*>(e)) { walkExpr(ae->target.get()); walkExpr(ae->value.get()); return; }
        if (auto ce = dynamic_cast<const ConditionalExpr*>(e)) { walkExpr(ce->cond.get()); walkExpr(ce->thenE.get()); walkExpr(ce->elseE.get()); return; }
        if (auto io = dynamic_cast<const InstanceOfExpr*>(e)) { walkExpr(io->value.get()); return; }
        if (auto cx = dynamic_cast<const CastExpr*>(e)) { walkExpr(cx->operand.get()); return; }
        if (auto aa = dynamic_cast<const ArrayAccess*>(e)) { walkExpr(aa->base.get()); walkExpr(aa->index.get()); return; }
        if (auto mr = dynamic_cast<const MethodReferenceExpr*>(e)) { walkExpr(mr->base.get()); return; }
        if (auto gc = dynamic_cast<const GuideChainExpr*>(e)) { walkExpr(gc->head.get()); return; }
        // Leaf expressions (literals, VarExpr, ThisExpr, NullLit, RanRefExpr).
    }

    std::string relPath_;
    std::vector<KeywordSupporterRecord>& out_;
    std::string cls_, method_;
    Counters counters_;
};

// mkdir -p for a single path component chain (POSIX). Returns true if the
// directory exists after the call.
bool ensureDir(const std::string& path) {
    if (path.empty()) return true;
    std::string acc;
    for (size_t i = 0; i < path.size(); ++i) {
        acc.push_back(path[i]);
        bool atSep = (path[i] == '/');
        bool atEnd = (i + 1 == path.size());
        if ((atSep || atEnd) && acc != "/" && !acc.empty()) {
            std::string dir = atSep ? acc.substr(0, acc.size() - 1) : acc;
            if (!dir.empty() && mkdir(dir.c_str(), 0755) != 0 && errno != EEXIST) return false;
        }
    }
    return true;
}

bool fileExists(const std::string& path) {
    struct stat st; return stat(path.c_str(), &st) == 0;
}

std::string supportStub(const KeywordSupporterRecord& r) {
    std::ostringstream o;
    o << "<< ---- Descriptive metadata (version 1.11 nested-friendly accessors) ----\n"
      << "   metaVersion      \"1.0.0\"\n"
      << "   metaId           none\n"
      << "   metaAuthor       unknown        < fill in the developer who owns this occurrence >\n"
      << "   metaDate         unknown        < fill in YYYY-MM-DD when documented >\n"
      << "   metaCopyright    none\n"
      << "   metaTrademark    none\n"
      << "   metaCorporation  none\n"
      << "   metaOrigin       none\n"
      << "   metaBirthDate    unknown\n"
      << "   metaScienceRating 1\n"
      << "   metaNationalSchedule none\n"
      << ">>\n\n"
      << "# Supporter document -- `" << r.keyword << "`\n\n"
      << "This document supports one specific use of the `" << r.keyword
      << "` keyword in SLeeLa source. It is linked to that occurrence by the\n"
      << "`@supporter(\"" << r.keyword << ":" << r.hash << "\")` annotation emitted for it.\n\n"
      << "| Field | Value |\n"
      << "|-------|-------|\n"
      << "| Keyword | `" << r.keyword << "` |\n"
      << "| Hash | `" << r.hash << "` |\n"
      << "| Source | `" << r.relSourcePath << "` |\n"
      << "| Enclosing | `" << r.enclosingClass << "::" << r.enclosingMethod << "` |\n"
      << "| Occurrence | #" << r.ordinal << " of `" << r.keyword
      << "` in that method |\n\n"
      << "> This stub was generated automatically. Fill in the sections below; it\n"
      << "> is never overwritten once it exists.\n\n"
      << "## Intent\n\n"
      << "_Why does this `" << r.keyword << "` exist here? What is it protecting or expressing?_\n\n"
      << "## Behavior\n\n"
      << "_What is the expected runtime behavior (control flow, values, side effects)?_\n\n"
      << "## Invariants\n\n"
      << "_What must hold before and after? What must never happen?_\n\n"
      << "## Notes\n\n"
      << "_References, related occurrences, open questions._\n";
    return o.str();
}

} // namespace

std::vector<KeywordSupporterRecord>
collectKeywordSupporters(const Program& prog, const std::string& relSourcePath) {
    std::vector<KeywordSupporterRecord> out;
    Walker w(relSourcePath, out);
    w.walkProgram(prog);
    return out;
}

void attachSupporterAnnotations(Program& prog,
                                const std::vector<KeywordSupporterRecord>& records) {
    for (const auto& r : records) {
        std::string value = r.keyword + ":" + r.hash;
        // Skip if an identical supporter annotation is already present.
        bool dup = false;
        for (const auto& a : prog.annotations.all()) {
            if (a.name == "supporter" && a.value == value) { dup = true; break; }
        }
        if (!dup) prog.annotations.add(std::string("supporter"), value);
    }
}

bool materializeSupporterFolders(const std::vector<KeywordSupporterRecord>& records,
                                 const std::string& docsRoot) {
    if (records.empty()) return true;
    const std::string base = docsRoot + "/keyword-supporters";
    if (!ensureDir(base)) return false;

    for (const auto& r : records) {
        std::string dir = base + "/" + r.keyword + "/" + r.hash;
        if (!ensureDir(dir)) return false;
        std::string doc = dir + "/SUPPORT.md";
        if (!fileExists(doc)) {               // never overwrite developer content
            std::ofstream f(doc);
            if (!f) return false;
            f << supportStub(r);
        }
    }

    // Refresh a simple index listing every known occurrence. The index is
    // generated output, so it IS rewritten each run.
    std::ofstream idx(base + "/INDEX.md");
    if (idx) {
        idx << "# Keyword supporter documents\n\n"
            << "Generated index of `try` / `catch` / `throw` / `synchronized` / `super`\n"
            << "occurrences and their supporter folders. Each row's folder holds a\n"
            << "`SUPPORT.md` for a developer to fill out; this index is regenerated\n"
            << "on each compile.\n\n"
            << "| Keyword | Hash | Source | Enclosing | # |\n"
            << "|---------|------|--------|-----------|---|\n";
        for (const auto& r : records) {
            idx << "| `" << r.keyword << "` | [`" << r.hash << "`](" << r.keyword
                << "/" << r.hash << "/SUPPORT.md) | `" << r.relSourcePath << "` | `"
                << r.enclosingClass << "::" << r.enclosingMethod << "` | " << r.ordinal
                << " |\n";
        }
    }
    return true;
}

} // namespace sleela::frontend
