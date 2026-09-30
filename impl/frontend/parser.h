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
    void collectStructNames();
    const Token& peek(int off=0) const; const Token& cur() const { return toks_[i_]; }
    bool check(Tok k) const { return cur().kind==k; } bool accept(Tok k);
    const Token& expect(Tok k,const char* what); [[noreturn]] void error(const std::string& msg) const;

    unsigned parseJavaModifiers();
    JavaTypeKind tokenTypeKind(Tok k) const;
    std::string parseQualifiedName();
    std::vector<std::string> parseTypeList(Tok terminator);
    std::vector<std::string> parseTypeParameters();
    std::string parseGenericType();
    void parseThrows(std::vector<std::string>& out);
    StructDecl parseStruct();
    ClassDecl parseClass(unsigned classModifiers=0, std::vector<annotation::Annotation> annotations={});
    Field parseField(bool isStatic,bool isProtected,unsigned modifiers=0, std::vector<annotation::Annotation> annotations={});
    Method parseMethod(bool isStatic,bool isProtected,unsigned modifiers=0, std::vector<annotation::Annotation> annotations={});
    bool isTypeStart() const;
    std::string parseType(std::vector<annotation::Annotation>* typeAnnotations=nullptr);
    StmtP parseStatement(); std::unique_ptr<Block> parseBlock(); StmtP parseSimpleStatement();
    ExprP parseExpr(); ExprP parseOr(); ExprP parseAnd(); ExprP parseEquality();
    ExprP parseComparison(); ExprP parseAdditive(); ExprP parseMultiplicative();
    ExprP parseUnary(); ExprP parsePrimary(); ExprP parsePostfix(ExprP base);
};
} // namespace sleela
#endif // SLEELA_PARSER_H
