// ===========================================================================
// ast.h  --  Abstract syntax tree for the Java-like Sleela subset.
// ===========================================================================
#ifndef SLEELA_AST_H
#define SLEELA_AST_H

#include <memory>
#include <string>
#include <vector>

namespace sleela {

// ---- Expressions ----------------------------------------------------------
struct Expr { virtual ~Expr() = default; };
using ExprP = std::unique_ptr<Expr>;

struct IntLit    : Expr { long long value; explicit IntLit(long long v):value(v){} };
struct DoubleLit : Expr { double value;    explicit DoubleLit(double v):value(v){} };
struct BoolLit   : Expr { bool value;      explicit BoolLit(bool v):value(v){} };
struct StrLit    : Expr { std::string value; explicit StrLit(std::string v):value(std::move(v)){} };
struct NullLit   : Expr {};

struct VarExpr   : Expr { std::string name; explicit VarExpr(std::string n):name(std::move(n)){} };

struct Unary : Expr {
    std::string op;          // "-" or "!"
    ExprP operand;
    Unary(std::string o, ExprP e):op(std::move(o)),operand(std::move(e)){}
};

struct Binary : Expr {
    std::string op;          // + - * / % == != < <= > >= && ||
    ExprP lhs, rhs;
    Binary(std::string o, ExprP l, ExprP r):op(std::move(o)),lhs(std::move(l)),rhs(std::move(r)){}
};

struct Call : Expr {
    std::string callee;
    std::vector<ExprP> args;
    explicit Call(std::string c):callee(std::move(c)){}
};

// ---- Statements -----------------------------------------------------------
struct Stmt { virtual ~Stmt() = default; };
using StmtP = std::unique_ptr<Stmt>;

struct VarDecl : Stmt {       // int x = expr;   (type is informational)
    std::string type;
    std::string name;
    ExprP init;               // may be null
};

struct Assign : Stmt {        // x = expr;
    std::string name;
    ExprP value;
};

struct ExprStmt : Stmt { ExprP expr; };

struct PrintStmt : Stmt { ExprP expr; };

struct ReturnStmt : Stmt { ExprP value; };  // value may be null

struct Block : Stmt { std::vector<StmtP> stmts; };

struct IfStmt : Stmt {
    ExprP cond;
    StmtP thenS;
    StmtP elseS;              // may be null
};

struct WhileStmt : Stmt {
    ExprP cond;
    StmtP body;
};

struct ForStmt : Stmt {       // for (init; cond; update) body
    StmtP init;               // VarDecl or Assign or null
    ExprP cond;               // may be null (=> true)
    StmtP update;             // Assign or ExprStmt or null
    StmtP body;
};

// ---- Declarations ---------------------------------------------------------
struct Param { std::string type; std::string name; };

struct Method {
    std::string retType;
    std::string name;
    std::vector<Param> params;
    std::unique_ptr<Block> body;
};

// A class-level field. Fields are shared state: they compile to core globals
// (which are thread-safe), so they are visible across spawned threads.
struct Field {
    std::string type;
    std::string name;
    ExprP init;               // may be null (defaults to null/0)
};

struct ClassDecl {
    std::string name;
    std::vector<Field> fields;
    std::vector<Method> methods;
};

struct Program {
    std::vector<ClassDecl> classes;
};

} // namespace sleela

#endif // SLEELA_AST_H
