// ===========================================================================
// parser.cpp  --  Recursive-descent parser for the Java-like Sleela subset.
// ===========================================================================
#include "parser.h"
#include <stdexcept>
namespace sleela {
const Token& Parser::peek(int off) const { size_t p=i_+(size_t)off; if(p>=toks_.size()) return toks_.back(); return toks_[p]; }
bool Parser::accept(Tok k){if(check(k)){i_++;return true;}return false;}
const Token& Parser::expect(Tok k,const char* what){if(!check(k)) error(std::string("expected ")+what+" but found '"+(cur().text.empty()?tokName(cur().kind):cur().text)+"'");return toks_[i_++];}
void Parser::error(const std::string& msg) const {const Token&t=cur();throw std::runtime_error("Syntax error (line "+std::to_string(t.line)+", col "+std::to_string(t.col)+"): "+msg);}
// Pre-pass: record every `struct Name` so that Name is recognised as a type
// name throughout parsing, regardless of declaration order (C/C++-like: a
// struct type may be referenced before or after its definition here).
void Parser::collectStructNames(){for(size_t k=0;k+1<toks_.size();++k){if(toks_[k].kind==Tok::KwStruct&&toks_[k+1].kind==Tok::Ident)structNames_.insert(toks_[k+1].text);}}
Program Parser::parseProgram(){collectStructNames();Program p;while(!check(Tok::Eof)){if(accept(Tok::KwImport)){p.imports.push_back(expect(Tok::Ident,"module name").text);expect(Tok::Semicolon,"';'");}else if(check(Tok::KwStruct))p.structs.push_back(parseStruct());else if(check(Tok::KwClass))p.classes.push_back(parseClass());else error("expected 'import', 'struct', or 'class' at top level");}if(p.classes.empty())error("program contains no classes");return p;}
StructDecl Parser::parseStruct(){expect(Tok::KwStruct,"'struct'");StructDecl s;s.name=expect(Tok::Ident,"struct name").text;expect(Tok::LBrace,"'{'");while(!check(Tok::RBrace)&&!check(Tok::Eof)){Field f;f.type=parseType();f.name=expect(Tok::Ident,"field name").text;expect(Tok::Semicolon,"';'");s.fields.push_back(std::move(f));}expect(Tok::RBrace,"'}'");return s;}
ClassDecl Parser::parseClass(){expect(Tok::KwClass,"'class'");ClassDecl c;c.name=expect(Tok::Ident,"class name").text;expect(Tok::LBrace,"'{'");while(!check(Tok::RBrace)&&!check(Tok::Eof)){size_t save=i_;bool isStatic=accept(Tok::KwStatic);bool isProtected=accept(Tok::KwProtected);if(!isTypeStart())error("expected a type for a field or method");i_++;if(!check(Tok::Ident))error("expected a field or method name");Tok after=peek(1).kind;i_=save;if(after==Tok::LParen)c.methods.push_back(parseMethod(isStatic,isProtected));else c.fields.push_back(parseField(isStatic,isProtected));}expect(Tok::RBrace,"'}'");return c;}
Field Parser::parseField(bool isStatic,bool isProtected){Field f;f.isStatic=isStatic;f.isProtected=isProtected;f.type=parseType();f.name=expect(Tok::Ident,"field name").text;if(accept(Tok::Assign))f.init=parseExpr();expect(Tok::Semicolon,"';'");return f;}
// A type is one of the scalar keywords, or an identifier naming a declared struct.
bool Parser::isTypeStart()const{Tok k=cur().kind;if(k==Tok::KwVoid||k==Tok::KwIntT||k==Tok::KwDoubleT||k==Tok::KwBoolT||k==Tok::KwStringT)return true;return k==Tok::Ident&&structNames_.count(cur().text)>0;}
std::string Parser::parseType(){const Token&t=cur();if(!isTypeStart())error("expected a type");i_++;return t.text;}
Method Parser::parseMethod(bool isStatic,bool isProtected){Method m;m.isStatic=isStatic;m.isProtected=isProtected;m.retType=parseType();m.name=expect(Tok::Ident,"method name").text;expect(Tok::LParen,"'('");if(!check(Tok::RParen)){do{Param p;p.type=parseType();p.name=expect(Tok::Ident,"parameter name").text;m.params.push_back(p);}while(accept(Tok::Comma));}expect(Tok::RParen,"')'");m.body=parseBlock();return m;}
std::unique_ptr<Block> Parser::parseBlock(){expect(Tok::LBrace,"'{'");auto b=std::make_unique<Block>();while(!check(Tok::RBrace)&&!check(Tok::Eof))b->stmts.push_back(parseStatement());expect(Tok::RBrace,"'}'");return b;}
StmtP Parser::parseStatement(){if(check(Tok::LBrace))return parseBlock();if(accept(Tok::KwIf)){auto s=std::make_unique<IfStmt>();expect(Tok::LParen,"'('");s->cond=parseExpr();expect(Tok::RParen,"')'");s->thenS=parseStatement();if(accept(Tok::KwElse))s->elseS=parseStatement();return s;}if(accept(Tok::KwWhile)){auto s=std::make_unique<WhileStmt>();expect(Tok::LParen,"'('");s->cond=parseExpr();expect(Tok::RParen,"')'");s->body=parseStatement();return s;}if(accept(Tok::KwFor)){auto s=std::make_unique<ForStmt>();expect(Tok::LParen,"'('");if(!check(Tok::Semicolon))s->init=parseSimpleStatement();expect(Tok::Semicolon,"';'");if(!check(Tok::Semicolon))s->cond=parseExpr();expect(Tok::Semicolon,"';'");if(!check(Tok::RParen))s->update=parseSimpleStatement();expect(Tok::RParen,"')'");s->body=parseStatement();return s;}if(accept(Tok::KwReturn)){auto s=std::make_unique<ReturnStmt>();if(!check(Tok::Semicolon))s->value=parseExpr();expect(Tok::Semicolon,"';'");return s;}if(accept(Tok::KwPrint)){auto s=std::make_unique<PrintStmt>();expect(Tok::LParen,"'('");s->expr=parseExpr();expect(Tok::RParen,"')'");expect(Tok::Semicolon,"';'");return s;}StmtP s=parseSimpleStatement();expect(Tok::Semicolon,"';'");return s;}
StmtP Parser::parseSimpleStatement(){
    // Declaration: `Type name [= expr]`. A struct type name is a valid Type.
    // Disambiguate from `structVar.field = ...` / `structVar = ...` by requiring
    // the token after a bare-identifier "type" to be another identifier.
    if(isTypeStart()&&!(cur().kind==Tok::Ident&&peek(1).kind!=Tok::Ident)){auto d=std::make_unique<VarDecl>();d->type=parseType();d->name=expect(Tok::Ident,"variable name").text;if(accept(Tok::Assign))d->init=parseExpr();return d;}
    // Otherwise parse an expression; if `=` follows, it is an assignment whose
    // target must be an lvalue (a plain name or a struct member access).
    ExprP lhs=parseExpr();
    if(check(Tok::Assign)){
        i_++;
        if(auto v=dynamic_cast<VarExpr*>(lhs.get())){auto a=std::make_unique<Assign>();a->name=v->name;a->value=parseExpr();return a;}
        if(auto m=dynamic_cast<MemberAccess*>(lhs.get())){auto fa=std::make_unique<FieldAssign>();fa->base=std::move(m->base);fa->field=m->field;fa->value=parseExpr();return fa;}
        error("assignment target must be a variable or a struct field");
    }
    auto e=std::make_unique<ExprStmt>();e->expr=std::move(lhs);return e;
}
ExprP Parser::parseExpr(){return parseOr();}
ExprP Parser::parseOr(){ExprP e=parseAnd();while(check(Tok::OrOr)){i_++;e=std::make_unique<Binary>("||",std::move(e),parseAnd());}return e;}
ExprP Parser::parseAnd(){ExprP e=parseEquality();while(check(Tok::AndAnd)){i_++;e=std::make_unique<Binary>("&&",std::move(e),parseEquality());}return e;}
ExprP Parser::parseEquality(){ExprP e=parseComparison();for(;;){if(check(Tok::EqEq)){i_++;e=std::make_unique<Binary>("==",std::move(e),parseComparison());}else if(check(Tok::NotEq)){i_++;e=std::make_unique<Binary>("!=",std::move(e),parseComparison());}else break;}return e;}
ExprP Parser::parseComparison(){ExprP e=parseAdditive();for(;;){if(check(Tok::Lt)){i_++;e=std::make_unique<Binary>("<",std::move(e),parseAdditive());}else if(check(Tok::Le)){i_++;e=std::make_unique<Binary>("<=",std::move(e),parseAdditive());}else if(check(Tok::Gt)){i_++;e=std::make_unique<Binary>(">",std::move(e),parseAdditive());}else if(check(Tok::Ge)){i_++;e=std::make_unique<Binary>(">=",std::move(e),parseAdditive());}else break;}return e;}
ExprP Parser::parseAdditive(){ExprP e=parseMultiplicative();for(;;){if(check(Tok::Plus)){i_++;e=std::make_unique<Binary>("+",std::move(e),parseMultiplicative());}else if(check(Tok::Minus)){i_++;e=std::make_unique<Binary>("-",std::move(e),parseMultiplicative());}else break;}return e;}
ExprP Parser::parseMultiplicative(){ExprP e=parseUnary();for(;;){if(check(Tok::Star)){i_++;e=std::make_unique<Binary>("*",std::move(e),parseUnary());}else if(check(Tok::Slash)){i_++;e=std::make_unique<Binary>("/",std::move(e),parseUnary());}else if(check(Tok::Percent)){i_++;e=std::make_unique<Binary>("%",std::move(e),parseUnary());}else break;}return e;}
ExprP Parser::parseUnary(){if(check(Tok::Minus)){i_++;return std::make_unique<Unary>("-",parseUnary());}if(check(Tok::Not)){i_++;return std::make_unique<Unary>("!",parseUnary());}return parsePrimary();}
ExprP Parser::parsePrimary(){const Token&t=cur();switch(t.kind){case Tok::Int:i_++;return std::make_unique<IntLit>(std::stoll(t.text));case Tok::Double:i_++;return std::make_unique<DoubleLit>(std::stod(t.text));case Tok::Str:i_++;return std::make_unique<StrLit>(t.text);case Tok::KwTrue:i_++;return std::make_unique<BoolLit>(true);case Tok::KwFalse:i_++;return std::make_unique<BoolLit>(false);case Tok::KwNull:i_++;return std::make_unique<NullLit>();case Tok::LParen:{i_++;ExprP e=parseExpr();expect(Tok::RParen,"')'");return e;}
case Tok::KwNew:{i_++;const Token&tn=expect(Tok::Ident,"struct type name after 'new'");if(!structNames_.count(tn.text))error("'new' requires a declared struct type, got '"+tn.text+"'");expect(Tok::LParen,"'('");expect(Tok::RParen,"')'");return std::make_unique<NewExpr>(tn.text);}
case Tok::Ident:{
    std::string name=t.text;i_++;
    ExprP base;
    // A dotted chain that ends in '(' is a qualified call name (namespaced
    // builtins like Foo.bar()); we keep the flat-callee behaviour for those.
    // Otherwise the first identifier is a value and each '.field' is a member
    // access on a struct.
    if(check(Tok::Dot)){
        // Look ahead: does this dotted chain terminate in a call?
        size_t save=i_;bool isCall=false;while(check(Tok::Dot)){i_++;if(!check(Tok::Ident))break;i_++;if(check(Tok::LParen)){isCall=true;break;}}
        i_=save;
        if(isCall){while(accept(Tok::Dot)){name += "." + expect(Tok::Ident,"identifier after '.'").text;}expect(Tok::LParen,"'('");auto c=std::make_unique<Call>(name);if(!check(Tok::RParen)){do{c->args.push_back(parseExpr());}while(accept(Tok::Comma));}expect(Tok::RParen,"')'");base=std::move(c);}
        else{
            // Pure member-access chain: base.f1.f2 ...
            ExprP e=std::make_unique<VarExpr>(name);
            while(accept(Tok::Dot)){std::string field=expect(Tok::Ident,"field name after '.'").text;e=std::make_unique<MemberAccess>(std::move(e),field);}
            base=std::move(e);
        }
    }
    else if(accept(Tok::LParen)){auto c=std::make_unique<Call>(name);if(!check(Tok::RParen)){do{c->args.push_back(parseExpr());}while(accept(Tok::Comma));}expect(Tok::RParen,"')'");base=std::move(c);}
    else base=std::make_unique<VarExpr>(name);
    // Fluent postfix: consume any `.method(args)` suffixes on the result. This
    // makes chains like Munction.start(x).connect(y).send(z).closeWithReceipt()
    // parse as nested MethodCall nodes. A trailing `.field` (no call) after a
    // call is not valid here and falls through to the caller.
    return parsePostfix(std::move(base));
}
default:error(std::string("unexpected token '")+(t.text.empty()?tokName(t.kind):t.text)+"' in expression");}}
// Consume `.method(args)` suffixes on `base`, folding each into a MethodCall.
// Only a dot immediately followed by `ident (` is a postfix call; a dot
// followed by a plain field is left for the caller (member access is handled
// in the Ident primary above). This is what enables the fluent chain form.
ExprP Parser::parsePostfix(ExprP base){
    for(;;){
        if(!check(Tok::Dot)) break;
        // Look ahead: is this `.ident(` (a method call) rather than `.field`?
        if(peek(1).kind!=Tok::Ident || peek(2).kind!=Tok::LParen) break;
        i_++; // consume '.'
        std::string method=expect(Tok::Ident,"method name after '.'").text;
        expect(Tok::LParen,"'('");
        auto mc=std::make_unique<MethodCall>(std::move(base),method);
        if(!check(Tok::RParen)){do{mc->args.push_back(parseExpr());}while(accept(Tok::Comma));}
        expect(Tok::RParen,"')'");
        base=std::move(mc);
    }
    return base;
}
} // namespace sleela
