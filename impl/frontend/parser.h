// ===========================================================================
// parser.h  --  Recursive-descent parser: tokens -> Sleela AST.
// ===========================================================================
#ifndef SLEELA_PARSER_H
#define SLEELA_PARSER_H
#include "ast.h"
#include "lexer.h"
#include <set>
namespace sleela {
class Parser {
public:
    explicit Parser(std::vector<Token> toks) : toks_(std::move(toks)) {}
    Program parseProgram();
    annotation::Annotation parseAnnotation(annotation::UseSite site=annotation::UseSite::Declaration);
    std::vector<annotation::Annotation> parseAnnotations(annotation::UseSite site);
    std::vector<annotation::Annotation> parseTypeAnnotations();
private:
    std::vector<Token> toks_; size_t i_=0;
    std::set<std::string> structNames_;
    // Syntax 3.1 surface seen while parsing; copied to Program::syntax31Uses.
    std::set<std::string> uses31_;
    ExprP parseArrayLiteral(const std::string& arrayType);
    ExprP parseInitializer(const std::string& declType);
    // Nested classes (syntax 1.11) hoisted out of their enclosing class during
    // parseClass, in declaration order, each already named "Outer.Inner" and
    // flagged isNested. parseProgram appends these to Program.classes and
    // enforces the per-document cap of 42.
    std::vector<ClassDecl> hoistedNested_;
    bool inInterface_=false;  // parsing an interface body (bodiless methods are abstract)
    bool usesLineage_=false;  // the class being parsed uses Sleela.home / super(...)
    // Java signature surface (syntax 3.0): the type variables of the enclosing
    // generic class / method declarations, erased to Any in parsed types.
    std::vector<std::vector<std::string>> typeVars_;
    std::string javaErase(const std::string& source);
    bool acceptEllipsis();
    void splitAngleClosers();
    void collectStructNames();
    const Token& peek(int off=0) const; const Token& cur() const { return toks_[i_]; }
    bool check(Tok k) const { return cur().kind==k; } bool accept(Tok k);
    const Token& expect(Tok k,const char* what); [[noreturn]] void error(const std::string& msg) const;

    unsigned parseJavaModifiers();
    JavaTypeKind tokenTypeKind(Tok k) const;
    std::string parseQualifiedName();
    static std::string mangleTypeName(const std::string& t);
    std::vector<std::string> parseTypeList(Tok terminator);
    std::vector<std::string> parseTypeParameters();
    std::string parseGenericType();
    std::string parseTypeArgument();
    void parseThrows(std::vector<std::string>& out);
    StructDecl parseStruct();
    ClassDecl parseClass(unsigned classModifiers=0, std::vector<annotation::Annotation> annotations={}, int depth=1);
    DocumentExtension parseDocumentExtension();
    Field parseField(bool isStatic,bool isProtected,unsigned modifiers=0, std::vector<annotation::Annotation> annotations={});
    Method parseMethod(bool isStatic,bool isProtected,unsigned modifiers=0, std::vector<annotation::Annotation> annotations={});
    bool isTypeStart() const;
    std::string parseType(std::vector<annotation::Annotation>* typeAnnotations=nullptr);
    StmtP parseStatement(); std::unique_ptr<Block> parseBlock(); StmtP parseSimpleStatement();
    // Resolve the per-iteration sleep (milliseconds) a loop's leading timing
    // annotations (@loopDelay / @loopBudget) request. Returns 0 when none apply.
    long loopTimingMillis(const std::vector<annotation::Annotation>& anns);
    // Resolve a loop's leading @spill annotation into a LoopSpill: the one-shot
    // pause (ms) that a spill's timer-start requests, plus up to two descriptive
    // subjects (judges / witnesses "of performs"). Returns hasSpill=false when no
    // @spill annotation is present. Errors on a malformed or over-subscribed spill.
    LoopSpill loopSpill(const std::vector<annotation::Annotation>& anns);
    ExprP parseExpr();
    StmtP parseIf(); StmtP parseWhile(); StmtP parseDo(); StmtP parseFor(); StmtP parseSwitch(); StmtP parseTry(); StmtP parseSynchronized(); ExprP parseAssignment(); ExprP parseGuideChain(); ExprP parseStructOffload(); ExprP parseConditional(); ExprP parseOr(); ExprP parseAnd(); ExprP parseBitOr(); ExprP parseBitXor(); ExprP parseBitAnd(); ExprP parseEquality();
    ExprP parseComparison(); ExprP parseShift(); ExprP parseAdditive(); ExprP parseMultiplicative();
    ExprP parseUnary(); ExprP parsePostfix(ExprP base); ExprP parsePrimary();
};
} // namespace sleela
#endif // SLEELA_PARSER_H
