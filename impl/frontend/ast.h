// ===========================================================================
// ast.h  --  Abstract syntax tree for the Java-compatible Sleela surface.
// ===========================================================================
// Java transition note:
// The AST deliberately retains Java authorship metadata instead of discarding
// it during parsing.  Runtime lowering may ignore metadata today, but the
// loader, reflection layer, class-file bridge, and conformance tools can inspect
// the same information later.
//
// SLeeLa does not claim that metadata alone implements Java behavior.  It is
// the source-of-truth boundary between Java-shaped authorship and SLeeLa
// execution semantics.
#ifndef SLEELA_AST_H
#define SLEELA_AST_H
#include <memory>
#include <string>
#include <vector>
#include "../annotation/Annotation.hpp"

namespace sleela {

enum class JavaTypeKind {
    Class,
    Interface,
    Enum,
    Record,
    Annotation,
    Value,
    Unknown
};

enum JavaModifier : unsigned {
    JavaPublic       = 1u << 0,
    JavaProtected    = 1u << 1,
    JavaPrivate      = 1u << 2,
    JavaStatic       = 1u << 3,
    JavaFinal        = 1u << 4,
    JavaAbstract     = 1u << 5,
    JavaNative       = 1u << 6,
    JavaSynchronized = 1u << 7,
    JavaVolatile     = 1u << 8,
    JavaTransient    = 1u << 9,
    JavaStrictfp     = 1u << 10,
    JavaSealed       = 1u << 11,
    JavaNonSealed    = 1u << 12,
    JavaDefault      = 1u << 13
};

struct JavaTypeMetadata {
    JavaTypeKind kind = JavaTypeKind::Class;
    unsigned modifiers = 0;
    std::string qualifiedName;
    std::string superclass;
    std::vector<std::string> interfaces;
    std::vector<std::string> typeParameters;
    std::string sourceVersion = "28";
    std::string apiStatus = "standard";
    bool preview = false;
    bool incubator = false;
    bool internal = false;
    std::vector<annotation::Annotation> annotations;
    std::vector<annotation::Annotation> typeAnnotations;
};

struct JavaMemberMetadata {
    unsigned modifiers = 0;
    std::vector<std::string> typeParameters;
    std::vector<std::string> thrownTypes;
    bool constructor = false;
    bool varargs = false;
    bool synthetic = false;
    bool bridge = false;
    std::vector<annotation::Annotation> annotations;
    std::vector<annotation::Annotation> typeAnnotations;
};

struct Expr { virtual ~Expr() = default; }; using ExprP = std::unique_ptr<Expr>;
struct IntLit:Expr{long long value;explicit IntLit(long long v):value(v){}};
struct DoubleLit:Expr{double value;explicit DoubleLit(double v):value(v){}};
struct BoolLit:Expr{bool value;explicit BoolLit(bool v):value(v){}};
struct StrLit:Expr{std::string value;explicit StrLit(std::string v):value(std::move(v)){}};
struct NullLit:Expr{};
struct VarExpr:Expr{std::string name;explicit VarExpr(std::string n):name(std::move(n)){}};
struct Unary:Expr{std::string op;ExprP operand;Unary(std::string o,ExprP e):op(std::move(o)),operand(std::move(e)){}};
struct Binary:Expr{std::string op;ExprP lhs,rhs;Binary(std::string o,ExprP l,ExprP r):op(std::move(o)),lhs(std::move(r)){}};
struct Call:Expr{std::string callee;std::vector<ExprP> args;explicit Call(std::string c):callee(std::move(c)){}};
struct MethodCall:Expr{ExprP receiver;std::string method;std::vector<ExprP> args;MethodCall(ExprP r,std::string m):receiver(std::move(r)),method(std::move(m)){}};
struct NewExpr:Expr{std::string typeName;explicit NewExpr(std::string t):typeName(std::move(t)){}};
struct MemberAccess:Expr{ExprP base;std::string field;MemberAccess(ExprP b,std::string f):base(std::move(b)),field(std::move(f)){}};

struct Stmt{virtual ~Stmt()=default;};using StmtP=std::unique_ptr<Stmt>;
struct VarDecl:Stmt{std::string type,name;ExprP init;};
struct Assign:Stmt{std::string name;ExprP value;};
struct FieldAssign:Stmt{ExprP base;std::string field;ExprP value;};
struct ExprStmt:Stmt{ExprP expr;}; struct PrintStmt:Stmt{ExprP expr;};
struct ReturnStmt:Stmt{ExprP value;}; struct Block:Stmt{std::vector<StmtP> stmts;};
struct IfStmt:Stmt{ExprP cond;StmtP thenS,elseS;};
struct WhileStmt:Stmt{ExprP cond;StmtP body;}; struct ForStmt:Stmt{StmtP init;ExprP cond;StmtP update,body;};

struct Param{std::string type,name; std::vector<annotation::Annotation> annotations; std::vector<annotation::Annotation> typeAnnotations;};
struct Method {
    std::string retType,name;
    std::vector<Param> params;
    std::unique_ptr<Block> body;
    bool isStatic=false;
    bool isProtected=false;
    JavaMemberMetadata java;
};
struct Field {
    std::string type,name;
    std::vector<annotation::Annotation> annotations;
    std::vector<annotation::Annotation> typeAnnotations;
    ExprP init;
    bool isStatic=false;
    bool isProtected=false;
    JavaMemberMetadata java;
};
struct ClassDecl {
    std::string name;
    std::vector<Field> fields;
    std::vector<Method> methods;
    JavaTypeMetadata java;
};

struct StructDecl{std::string name;std::vector<Field> fields;};

struct Program {
    annotation::DocumentAnnotations annotations;
    std::vector<std::string> imports;
    std::vector<StructDecl> structs;
    std::vector<ClassDecl> classes;
};

} // namespace sleela
#endif // SLEELA_AST_H
