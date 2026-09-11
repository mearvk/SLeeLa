// ===========================================================================
// ast.h  --  Abstract syntax tree for the Java-like Sleela subset.
// ===========================================================================
#ifndef SLEELA_AST_H
#define SLEELA_AST_H
#include <memory>
#include <string>
#include <vector>
namespace sleela {
struct Expr { virtual ~Expr() = default; }; using ExprP = std::unique_ptr<Expr>;
struct IntLit:Expr{long long value;explicit IntLit(long long v):value(v){}};
struct DoubleLit:Expr{double value;explicit DoubleLit(double v):value(v){}};
struct BoolLit:Expr{bool value;explicit BoolLit(bool v):value(v){}};
struct StrLit:Expr{std::string value;explicit StrLit(std::string v):value(std::move(v)){}};
struct NullLit:Expr{};
struct VarExpr:Expr{std::string name;explicit VarExpr(std::string n):name(std::move(n)){}};
struct Unary:Expr{std::string op;ExprP operand;Unary(std::string o,ExprP e):op(std::move(o)),operand(std::move(e)){}};
struct Binary:Expr{std::string op;ExprP lhs,rhs;Binary(std::string o,ExprP l,ExprP r):op(std::move(o)),lhs(std::move(l)),rhs(std::move(r)){}};
struct Call:Expr{std::string callee;std::vector<ExprP> args;explicit Call(std::string c):callee(std::move(c)){}};
struct Stmt{virtual ~Stmt()=default;};using StmtP=std::unique_ptr<Stmt>;
struct VarDecl:Stmt{std::string type,name;ExprP init;};
struct Assign:Stmt{std::string name;ExprP value;};
struct ExprStmt:Stmt{ExprP expr;}; struct PrintStmt:Stmt{ExprP expr;};
struct ReturnStmt:Stmt{ExprP value;}; struct Block:Stmt{std::vector<StmtP> stmts;};
struct IfStmt:Stmt{ExprP cond;StmtP thenS,elseS;};
struct WhileStmt:Stmt{ExprP cond;StmtP body;};
struct ForStmt:Stmt{StmtP init;ExprP cond;StmtP update,body;};
struct Param{std::string type,name;};
struct Method{std::string retType,name;std::vector<Param> params;std::unique_ptr<Block> body;};
struct Field{std::string type,name;ExprP init;};
struct ClassDecl{std::string name;std::vector<Field> fields;std::vector<Method> methods;};
struct Program {
    // Explicit module dependencies. The compiler validates these against the
    // native module registry before lowering the program.
    std::vector<std::string> imports;
    std::vector<ClassDecl> classes;
};
} // namespace sleela
#endif // SLEELA_AST_H
