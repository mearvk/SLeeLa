// ===========================================================================
// sst_sema.cpp  --  Semantic analysis: resolve, validate, topo-order.
// ===========================================================================
#include "sst_sema.h"

#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace nordshrift {

namespace {

[[noreturn]] void fail(const std::string& msg) {
    throw std::runtime_error("Nordshrift semantic error: " + msg);
}

// Walk every statement in a block, invoking fn on each CallStmt found.
void forEachCall(const Stmt* s, const std::function<void(const CallStmt&)>& fn) {
    if (auto b = dynamic_cast<const Block*>(s)) {
        for (auto& st : b->stmts) forEachCall(st.get(), fn);
    } else if (auto i = dynamic_cast<const IfStmt*>(s)) {
        forEachCall(i->thenS.get(), fn);
        if (i->elseS) forEachCall(i->elseS.get(), fn);
    } else if (auto w = dynamic_cast<const WhileStmt*>(s)) {
        forEachCall(w->body.get(), fn);
    } else if (auto c = dynamic_cast<const CallStmt*>(s)) {
        fn(*c);
    }
}

} // namespace

void analyze(Sheet& sheet) {
    const int n = (int)sheet.components.size();

    // ---- 1. validate uses targets + thread/repeat annotations ----------
    for (auto& c : sheet.components) {
        for (auto& u : c.uses)
            if (!sheet.index.count(u))
                fail("component '" + c.name + "' uses unknown component '" + u + "'");
        if (c.threads() < 1)
            fail("component '" + c.name + "' has invalid 'threads' (must be >= 1)");
        if (c.repeat() < 0)
            fail("component '" + c.name + "' has invalid 'repeat' (must be >= 0)");
    }

    // ---- 2. validate every call target ---------------------------------
    for (auto& c : sheet.components) {
        for (auto& a : c.attaches) {
            forEachCall(a.body.get(), [&](const CallStmt& call) {
                const Component* tc = sheet.find(call.comp);
                if (!tc)
                    fail("in component '" + c.name + "', call targets unknown component '" + call.comp + "'");
                if (!tc->findAttach(call.attach))
                    fail("in component '" + c.name + "', component '" + call.comp +
                         "' has no attach '" + call.attach + "'");
            });
        }
    }

    // ---- 3. exactly one main -------------------------------------------
    int mains = 0;
    for (auto& c : sheet.components) if (c.findAttach("main")) mains++;
    if (mains == 0) fail("no component defines a 'main' attach (need an entry point)");
    if (mains > 1)  fail("more than one component defines a 'main' attach");

    // ---- 4. topological order over `uses` edges (deps first) -----------
    // edge A -> B means A uses B, so B must be ordered before A.
    enum { WHITE, GRAY, BLACK };
    std::vector<int> color(n, WHITE);
    std::vector<int> order;
    order.reserve(n);
    std::vector<std::string> stack;   // for cycle diagnostics

    std::function<void(int)> dfs = [&](int idx) {
        color[idx] = GRAY;
        stack.push_back(sheet.components[idx].name);
        for (auto& u : sheet.components[idx].uses) {
            int j = sheet.index[u];
            if (color[j] == GRAY) {
                std::string cyc;
                for (auto& s : stack) cyc += s + " -> ";
                cyc += u;
                fail("cyclic 'uses' dependency: " + cyc);
            }
            if (color[j] == WHITE) dfs(j);
        }
        stack.pop_back();
        color[idx] = BLACK;
        order.push_back(idx);   // post-order => dependencies first
    };

    for (int i = 0; i < n; i++)
        if (color[i] == WHITE) dfs(i);

    sheet.order = order;   // dependency-first activation order
}

} // namespace nordshrift
