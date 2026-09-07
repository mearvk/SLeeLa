// ===========================================================================
// sst_ast.h  --  Nordshrift component graph + functional-attach IR.
//
// A .sst program is a set of Components. Each component has:
//   - scalar properties  (structural data: port: 8080; level: "info";)
//   - functional attaches (named blocks of behavior: on-start: { ... })
//   - uses edges          (combination / addressing of sibling components)
// The attach IR is a small Turing-complete expr/stmt tree shared by all three
// emitters (Java / Sleela / C).
// ===========================================================================
#ifndef NORDSHRIFT_SST_AST_H
#define NORDSHRIFT_SST_AST_H

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace nordshrift {

// ---- attach expressions ---------------------------------------------------
struct Expr { virtual ~Expr() = default; };
using ExprP = std::unique_ptr<Expr>;

struct NumLit  : Expr { double value; bool isInt; NumLit(double v,bool i):value(v),isInt(i){} };
struct StrLit  : Expr { std::string value; explicit StrLit(std::string v):value(std::move(v)){} };
struct BoolLit : Expr { bool value; explicit BoolLit(bool v):value(v){} };
struct VarRef  : Expr { std::string name; explicit VarRef(std::string n):name(std::move(n)){} };
struct PropRef : Expr { std::string name; explicit PropRef(std::string n):name(std::move(n)){} };
struct Unary   : Expr { std::string op; ExprP e; Unary(std::string o,ExprP x):op(std::move(o)),e(std::move(x)){} };
struct Binary  : Expr { std::string op; ExprP l,r; Binary(std::string o,ExprP a,ExprP b):op(std::move(o)),l(std::move(a)),r(std::move(b)){} };

// ---- attach statements ----------------------------------------------------
struct Stmt { virtual ~Stmt() = default; };
using StmtP = std::unique_ptr<Stmt>;

struct LetStmt    : Stmt { std::string name; ExprP init; };
struct AssignStmt : Stmt { std::string name; ExprP value; };
struct PrintStmt  : Stmt { ExprP expr; };
struct IfStmt     : Stmt { ExprP cond; StmtP thenS; StmtP elseS; };
struct WhileStmt  : Stmt { ExprP cond; StmtP body; };
struct CallStmt   : Stmt { std::string comp; std::string attach; };  // call C.f();
struct Block      : Stmt { std::vector<StmtP> stmts; };

// ---- component members ----------------------------------------------------
enum class ValKind { Number, Str, Bool, Ident };
struct PropValue {
    ValKind kind;
    std::string s;   // string / ident text
    double num = 0;  // numeric
    bool isInt = true;
    bool b = false;
};

struct Attach {
    std::string name;                 // e.g. "on-start", "main", "tick", "log"
    std::unique_ptr<Block> body;
};

struct Component {
    std::string name;
    std::map<std::string, PropValue> props;   // structural properties
    std::vector<Attach> attaches;             // functional attaches (methods)
    std::vector<std::string> uses;            // combination edges

    // convenience lookups (filled by parser/sema)
    bool hasProp(const std::string& k) const { return props.count(k) != 0; }
    const Attach* findAttach(const std::string& n) const {
        for (auto& a : attaches) if (a.name == n) return &a;
        return nullptr;
    }
    long long threads() const {
        auto it = props.find("threads");
        if (it != props.end() && it->second.kind == ValKind::Number)
            return (long long)it->second.num;
        return 1;
    }
    long long repeat() const {
        auto it = props.find("repeat");
        if (it != props.end() && it->second.kind == ValKind::Number)
            return (long long)it->second.num;
        return 1;
    }
};

struct Sheet {
    std::vector<Component> components;
    // component order after topological sort (indices into components)
    std::vector<int> order;
    // name -> index
    std::map<std::string, int> index;

    const Component* find(const std::string& n) const {
        auto it = index.find(n);
        return it == index.end() ? nullptr : &components[it->second];
    }
    // The component holding the `main` attach (entry point).
    const Component* mainComponent() const {
        for (auto& c : components) if (c.findAttach("main")) return &c;
        return nullptr;
    }
};

} // namespace nordshrift

#endif // NORDSHRIFT_SST_AST_H
