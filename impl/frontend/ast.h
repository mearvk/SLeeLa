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
    JavaDefault      = 1u << 13,
    // Sleela `const`: a developer-intent modifier meaning "I intend to keep this
    // variable constant until the System Design is complete." It is a design-time
    // marker (recorded on the declaration's modifier set), NOT a hard immutability
    // guarantee like `final`; it composes freely with public/static/final.
    JavaConst        = 1u << 14,
    // Sleela memory-tier modifiers: designate which memory tier a function/
    // program should run against, so the VM routes "usual activities" to the
    // DDR2 tier and competency/accuracy-critical work to the DDR5 tier.
    //   usualmem    -> DDR2 tier (usual activities)
    //   accuratemem -> DDR5 tier (competency / accuracy)
    // Unmarked declarations inherit the program's main tier (DDR5 by default).
    // They compose with other modifiers and are mutually exclusive by intent.
    JavaMemUsual     = 1u << 15,   // usualmem    (DDR2)
    JavaMemAccurate  = 1u << 16    // accuratemem (DDR5)
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

struct Block; struct Stmt; using StmtP=std::unique_ptr<Stmt>;
struct Expr { virtual ~Expr() = default; }; using ExprP = std::unique_ptr<Expr>;
struct IntLit:Expr{long long value;explicit IntLit(long long v):value(v){}};
struct DoubleLit:Expr{double value;explicit DoubleLit(double v):value(v){}};
struct BoolLit:Expr{bool value;explicit BoolLit(bool v):value(v){}};
struct StrLit:Expr{std::string value;explicit StrLit(std::string v):value(std::move(v)){}};
struct NullLit:Expr{};
struct VarExpr:Expr{std::string name;explicit VarExpr(std::string n):name(std::move(n)){}};
struct Unary:Expr{std::string op;ExprP operand;Unary(std::string o,ExprP e):op(std::move(o)),operand(std::move(e)){}};
struct Binary:Expr{std::string op;ExprP lhs,rhs;Binary(std::string o,ExprP l,ExprP r):op(std::move(o)),lhs(std::move(l)),rhs(std::move(r)){}};
struct Method;
// `resolved` is the overload chosen by semantic analysis (syntax 3.0 overload
// resolution); the compiler binds the call to exactly that declaration.
struct Call:Expr{std::string callee;std::vector<ExprP> args;mutable const Method* resolved=nullptr;explicit Call(std::string c):callee(std::move(c)){}};
struct MethodCall:Expr{ExprP receiver;std::string method;std::vector<ExprP> args;mutable const Method* resolved=nullptr;/* set by semantic analysis when the receiver is a String: the call is the static JavaStringMethods.<method>(receiver, args...) recorded in resolved */mutable bool stringReceiver=false;/* Java constructor invocation statement (SLEELA.syntax 43.6): super(args) / this(args) inside a constructor; receiver is SuperExpr/ThisExpr and resolved is the target constructor */bool superConstructor=false;bool thisConstructor=false;MethodCall(ExprP r,std::string m):receiver(std::move(r)),method(std::move(m)){}};
struct NewExpr:Expr{std::string typeName;std::vector<ExprP> args;mutable const Method* resolved=nullptr;explicit NewExpr(std::string t):typeName(std::move(t)){}};
// `ran::<word>` -- the System / already-Ran-in-RAM reference. The `ran::`
// qualifier names a system meaning explicitly, so ordinary words (mem, gc, ...)
// stay free as identifiers. `word` is the system name after `ran::` (e.g.
// "mem"). A value form such as `ran::mem` evaluates to a system quantity.
struct RanRefExpr:Expr{std::string word;explicit RanRefExpr(std::string w):word(std::move(w)){}};
struct MemberAccess:Expr{ExprP base;std::string field;MemberAccess(ExprP b,std::string f):base(std::move(b)),field(std::move(f)){}};
// --- Java expression forms (surface completeness). -------------------------
// Compound/plain assignment as an expression (e.g. a = b, a += b). `op` is the
// assignment operator text; `target` must be an lvalue (VarExpr/MemberAccess).
struct AssignmentExpr:Expr{std::string op;ExprP target,value;AssignmentExpr(std::string o,ExprP t,ExprP v):op(std::move(o)),target(std::move(t)),value(std::move(v)){}};
// Ternary conditional `cond ? thenE : elseE`.
struct ConditionalExpr:Expr{ExprP cond,thenE,elseE;ConditionalExpr(ExprP c,ExprP t,ExprP e):cond(std::move(c)),thenE(std::move(t)),elseE(std::move(e)){}};
// `value instanceof Type`.
struct InstanceOfExpr:Expr{ExprP value;std::string typeName;InstanceOfExpr(ExprP v,std::string t):value(std::move(v)),typeName(std::move(t)){}};
// Java cast `(Type) operand`.
struct CastExpr:Expr{std::string typeName;ExprP operand;CastExpr(std::string t,ExprP e):typeName(std::move(t)),operand(std::move(e)){}};
// `super` / `this` primary expressions.
struct SuperExpr:Expr{};
// Sleela.home lineage references (SLEELA.syntax section 42). Resolved at compile
// time against the loaded classes: Parents walks `depth` superclass levels up
// (clamped to the root; depth 0 is the VM), Vm is Sleela.VM/System/OS, and the
// Init* kinds give the starting value of the per-class lineage fields.
struct LineageExpr:Expr{enum Kind{Parents,Vm,InitParent,InitChild,InitRoot,Boot};Kind kind;int depth=0;explicit LineageExpr(Kind k,int d=0):kind(k),depth(d){}};
struct ThisExpr:Expr{};
// Array element access `base[index]`.
struct ArrayAccess:Expr{ExprP base,index;ArrayAccess(ExprP b,ExprP i):base(std::move(b)),index(std::move(i)){}};
// Method reference `base::method`.
struct MethodReferenceExpr:Expr{ExprP base;std::string method;MethodReferenceExpr(ExprP b,std::string m):base(std::move(b)),method(std::move(m)){}};

// Guide chain (syntax 2.3): `new T(...) :: Guide :: Guide ...`.
// The head is a constructor-style call (NewExpr); each guide is the name of a
// zero-argument method on the head's class, applied in order. The chain
// evaluates to the head instance (fluent), so a perfect Constructor can seat
// its Guides inline. See SLEELA.syntax section 17 and lib/philosophers.
struct GuideChainExpr:Expr{ExprP head;std::vector<std::string> guides;explicit GuideChainExpr(ExprP h):head(std::move(h)){}};

struct Stmt{virtual ~Stmt()=default;};using StmtP=std::unique_ptr<Stmt>;
struct VarDecl:Stmt{std::string type,name;ExprP init;};
struct Assign:Stmt{std::string name;ExprP value;};
struct FieldAssign:Stmt{ExprP base;std::string field;ExprP value;};
struct ExprStmt:Stmt{ExprP expr;ExprStmt()=default;explicit ExprStmt(ExprP e):expr(std::move(e)){}};
struct PrintStmt:Stmt{ExprP expr;};
// GC-hint statement (syntax 1.9): `x = gc N;` / `x = mem N;`. `target` names the
// value the developer is done using; `aggressiveness` is the 0..100 cleanup
// strength. Both the `gc` and `mem` spellings produce this node. It lowers to a
// single OP_GC_HINT with the aggressiveness as its operand.
struct GcHintStmt:Stmt{std::string target;std::string spelling;int aggressiveness=0;bool ranQualified=false;};
struct ReturnStmt:Stmt{ExprP value;};
struct Block:Stmt{std::vector<StmtP> stmts;};
struct IfStmt:Stmt{ExprP cond;StmtP thenS,elseS;};
// loopSleepMs: milliseconds to sleep per iteration, set by the @loopDelay /
// @loopBudget loop-timing annotations (0 = no delay). The compiler emits an
// OP_SLEEP with this operand at the top of each iteration's body.
//
// LoopSpill: the @spill loop annotation. A spill marks the loop entry as a
// timer-start checkpoint -- it "counts as a timer start itself", letting the
// loop pause once before the first iteration begins (spillPauseMs), as opposed
// to the per-iteration @loopDelay/@loopBudget sleep. A spill may also carry up
// to two descriptive subjects -- the judges and witnesses "of performs" that
// observe the loop's performance -- which are retained as loop metadata.
//   hasSpill       : a @spill annotation was present on the loop
//   spillPauseMs   : one-shot pause (ms) emitted ONCE before the loop top
//   spillJudges    : subject observing the loop as a judge   ("" if none)
//   spillWitnesses : subject observing the loop as a witness ("" if none)
struct LoopSpill{bool hasSpill=false;long spillPauseMs=0;std::string spillJudges,spillWitnesses;};
struct WhileStmt:Stmt{ExprP cond;StmtP body;long loopSleepMs=0;LoopSpill spill;};
struct ForStmt:Stmt{StmtP init;ExprP cond;StmtP update,body;ExprP threadStable,heapHealthy;long loopSleepMs=0;LoopSpill spill;};
// --- Java statement forms (surface completeness). --------------------------
// Enhanced for (SLEELA.syntax 43.6): `for (T v : xs) body`. Over an array it
// walks the elements by index; over an object it calls xs.iterator(), then
// hasNext()/next() on the result. Semantic analysis records which, and the
// resolved methods, in the mutable fields.
struct ForEachStmt:Stmt{std::string type,name;ExprP iterable;StmtP body;mutable bool overIterable=false;mutable std::string iterableType,iteratorType;mutable const Method* iteratorM=nullptr;mutable const Method* hasNextM=nullptr;mutable const Method* nextM=nullptr;};
struct DoStmt:Stmt{StmtP body;ExprP cond;long loopSleepMs=0;LoopSpill spill;};
struct BreakStmt:Stmt{std::string label;};
struct ContinueStmt:Stmt{std::string label;};
struct ThrowStmt:Stmt{ExprP value;};
struct AssertStmt:Stmt{ExprP cond;ExprP message;};
struct YieldStmt:Stmt{ExprP value;};
struct SynchronizedStmt:Stmt{ExprP monitor;std::unique_ptr<Block> body;};
// `type` is the declared text (alternatives joined by '|'); `types` holds each
// alternative of a multi-catch `catch (A | B e)` (SLEELA.syntax 43.6).
struct CatchClause{std::string type,variable;std::unique_ptr<Block> body;std::vector<std::string> types;};
struct TryStmt:Stmt{std::unique_ptr<Block> body;std::vector<CatchClause> catches;std::unique_ptr<Block> finallyBlock;};
struct SwitchCase{bool isDefault=false;std::vector<ExprP> labels;std::vector<StmtP> statements;};
struct SwitchStmt:Stmt{ExprP selector;std::vector<SwitchCase> cases;};

struct Param{std::string type,name; std::vector<annotation::Annotation> annotations{}; std::vector<annotation::Annotation> typeAnnotations{};};
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
    // Nested-class support (syntax 1.11). The "rule of two": a document nests
    // one level deep (the enclosing class and one nested class = depth 2). A
    // nested class may not itself contain an inline nested class; to go deeper
    // it holds a reference (a field typed as the deeper class, declared at top
    // level). The parser does not store nested classes inside their enclosing
    // ClassDecl (that would require ClassDecl to be copyable, which it is not --
    // Field/Method carry move-only expression bodies); instead each nested
    // class is hoisted directly into Program.classes under the qualified name
    // "Outer.Inner". These two fields mark a hoisted nested class for tooling.
    bool isNested=false;           // true once hoisted from an enclosing class
    std::string enclosingName;     // the enclosing class name when isNested
    // Provenance (source-library resolution). False for the user's own local
    // (non-/lib) classes; set true for a class merged in from the /lib source
    // tree by loadReferencedLibraryUnits. Records where a class came from so
    // tooling (and any future layout-budget diagnostics) can distinguish a
    // user's own class from one pulled in transitively from a large /lib.
    bool fromLibrary=false;
};

struct StructDecl{std::string name;std::vector<Field> fields;bool fromLibrary=false;};

struct DynamiteImport { std::string referenceName; std::string sourcePath; };
struct PermissibleImport { std::string referenceName; std::string sourcePath; };

// --- Document extension (syntax 1.10): `extends to ... <grouper>;` ----------
// A `.sleela` document may extend *to* one or more other documents, organized
// by a grouper (organization term). This is the document-sheet meaning of
// `extends`, distinct from a class's OOD `extends` of a base type:
//
//   extends to Other linear;            // linear reals: an ordered chain
//   extends to { A, B, C } group;       // an unordered grouped set
//   extends to { A, B } services Hub;   // a group fronted by a Server of Services
//
// The grouper names the structure the referenced documents form (see
// Grouper below); `server` holds the Server-of-Services document name for the
// `services` grouper (empty otherwise).
enum class Grouper {
    Linear,     // ordered chain of document references (congruent-linear reals)
    Group,      // unordered set gathered under one organizer
    Services    // a group fronted by one Server-of-Services document
};
struct DocumentExtension {
    Grouper grouper = Grouper::Linear;
    std::vector<std::string> targets;  // referenced document names
    std::string server;                // Server-of-Services name (services only)
};

struct Program {
    annotation::DocumentAnnotations annotations;
    std::vector<std::string> imports;
    std::vector<DynamiteImport> dynamiteImports;
    std::vector<PermissibleImport> permissibleImports;
    std::vector<StructDecl> structs;
    std::vector<ClassDecl> classes;
    std::vector<DocumentExtension> documentExtensions;
    // Number of nested classes hoisted from enclosing classes in this document
    // (syntax 1.11). Capped by the parser at 42 per document.
    int nestedClassCount=0;
    // Author marker (syntax 2.4): set true when the document declares the bare
    // top-level word `author` or its synonym `authorial`. It marks the document
    // as handled by professional council and, operationally, permits duplicate
    // fields/methods (each member then carries a declaration-order id and an
    // SHA-256 brander) and requests the originated source document on compile.
    bool authorMarked=false;
    // Material marker (syntax 2.5): set true when the document declares the bare
    // top-level word `material`. DECLARATIVE ONLY -- it has no effect on
    // parsing, typing, or codegen. It claims the document is material (well
    // considered, present to its author as its own Source -- "the thing,
    // Material") and marks membership in the set of material documents: those
    // concerning state, progress, or the securing of change. The set is, by
    // convention, not freely changed out of.
    bool materialMarked=false;
    // Memory marker (syntax 2.5): set true when the document declares the bare
    // top-level word `memory`. DECLARATIVE ONLY -- like `material` it has no
    // effect on parsing, typing, or codegen. It claims the document is a
    // document of memory: authored by and for the Graduates of Supply of the
    // New York State Department of Commerce -- persons of 1501+ IQ with
    // 51,000,000 years of superb worker-class service to graduation. It marks
    // membership in the set of memory documents: those the Graduates of Supply
    // hold as kept, remembered standing. It is a contextual word, not a
    // reserved keyword, and -- like author/authorial and material -- gated to
    // #sleela 2.5 (the current edition; no edition bump).
    bool memoryMarked=false;
    // Determine marker (syntax 2.6): set true when the document declares the
    // bare top-level word `determine`. DECLARATIVE ONLY -- like material/memory
    // it has no effect on parsing, typing, or codegen. It is a claim about the
    // document, recorded here and gated to #sleela 2.6. The ideal (stated as-is
    // in SLEELA.syntax section 23): a determine document is open to be processed
    // to further by a qualified hand -- a doctor, physician, or ghost -- toward
    // completion. The procurement of term is held ideal as real for the
    // torturement (completion by legal hand) as required by further processor
    // review. Science practitioners should and could can use this marker. It is
    // a contextual word, not a reserved keyword, so `determine` stays an
    // ordinary identifier anywhere else (and in any document below 2.6).
    bool determineMarked=false;
    // Object marker (syntax 2.6): set true when the document declares the bare
    // top-level word `object`. DECLARATIVE ONLY -- like the other Natural
    // Process markers it has no effect on parsing, typing, or codegen. It names
    // the document an Object: a settled thing standing in the catalogue of what
    // is, to which its author and the field relate (SLEELA.syntax section 24).
    // Contextual, not a reserved keyword, gated to #sleela 2.6.
    bool objectMarked=false;
    // Field marker (syntax 2.6): set true when the document declares the bare
    // top-level word `field` at document scope BEFORE any content. DECLARATIVE
    // ONLY. It names the document the Field: the ground of relation in which
    // objects stand and move (SLEELA.syntax section 24). Recognised only at
    // top-level pre-content scope, so the class-member `field-decl` grammar and
    // every `.field` access are unaffected. Gated to #sleela 2.6.
    bool fieldMarked=false;
    // Pure marker (syntax 3.0): set true when the document declares the bare
    // top-level word `pure`. DECLARATIVE ONLY -- like the other Natural Process
    // / Preonem markers it has no effect on parsing, typing, or codegen. It is
    // a claim that the document is steered by, and stands on, its OWN honesty:
    // what it holds is clean, unmixed, and true to itself (SLEELA.syntax
    // section 27). Contextual, not a reserved keyword, so `pure` stays an
    // ordinary identifier anywhere else (and in any document below 3.0).
    bool pureMarked=false;
    // Gasoline marker (syntax 3.0): set true when the document declares the bare
    // top-level word `gasoline`. DECLARATIVE ONLY. It is the near-synonym of
    // `pure` -- the same claim of a document steered by its own honesty, in the
    // sense of a refined, usable grade (SLEELA.syntax section 27). The two
    // markers mean nearly the same thing and compose with the others.
    // Contextual, not a reserved keyword, gated to #sleela 3.0.
    bool gasolineMarked=false;
    // Artifact marker (syntax 3.0): set true when the document declares the bare
    // top-level word `artifact`. DECLARATIVE ONLY -- like the other Natural
    // Process / Preonem markers it has no effect on parsing, typing, or codegen.
    // It is a claim that the document is an enacted ARTIFACT: legislature-grade
    // text, graded unto time and space -- binding for the moment and place it is
    // read until superseded by a later enactment of the same body (SLEELA.syntax
    // section 35). Contextual, not a reserved keyword, so `artifact` stays an
    // ordinary identifier anywhere else (and in any document below 3.0).
    bool artifactMarked=false;
    // Spark marker (syntax 3.0, SLEELA.syntax section 43). A Preonem that
    // indicates Java style declarative: the document declares itself SLeeLa
    // constructs written in Java style. Java support
    // for it is handled by the interpreter and VM (frontend java_references.cpp,
    // the compiler, the VM opcodes), never by a SLeeLa housing class. Recorded
    // standing only; no change to parsing, typing, or codegen. Contextual, not a
    // reserved keyword.
    bool sparkMarked=false;
};

} // namespace sleela
#endif // SLEELA_AST_H
