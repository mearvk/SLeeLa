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
unsigned Parser::parseJavaModifiers(){
    unsigned m=0;
    bool again=true;
    while(again){again=false;switch(cur().kind){
        case Tok::KwPublic:m|=JavaPublic;break; case Tok::KwProtected:m|=JavaProtected;break;
        case Tok::KwPrivate:m|=JavaPrivate;break; case Tok::KwStatic:m|=JavaStatic;break;
        case Tok::KwFinal:m|=JavaFinal;break; case Tok::KwAbstract:m|=JavaAbstract;break;
        case Tok::KwNative:m|=JavaNative;break; case Tok::KwSynchronized:m|=JavaSynchronized;break;
        case Tok::KwVolatile:m|=JavaVolatile;break; case Tok::KwTransient:m|=JavaTransient;break;
        case Tok::KwStrictfp:m|=JavaStrictfp;break; case Tok::KwSealed:m|=JavaSealed;break;
        case Tok::KwNonSealed:m|=JavaNonSealed;break; case Tok::KwDefault:m|=JavaDefault;break;
        default: continue;
    } i_++; again=true;}
    return m;
}
JavaTypeKind Parser::tokenTypeKind(Tok k) const {
    if(k==Tok::KwInterface)return JavaTypeKind::Interface;
    if(k==Tok::KwEnum)return JavaTypeKind::Enum;
    if(k==Tok::KwRecord)return JavaTypeKind::Record;
    return JavaTypeKind::Class;
}
std::string Parser::parseQualifiedName(){
    std::string n=expect(Tok::Ident,"qualified name").text;
    while(accept(Tok::Dot)) n+="."+expect(Tok::Ident,"identifier after '.'").text;
    return n;
}
std::vector<std::string> Parser::parseTypeList(Tok terminator){
    std::vector<std::string> out;
    if(check(terminator)) return out;
    do { out.push_back(parseQualifiedName()); } while(accept(Tok::Comma));
    return out;
}
std::vector<std::string> Parser::parseTypeParameters(){
    std::vector<std::string> out; if(!accept(Tok::Lt)) return out;
    do { std::string p=expect(Tok::Ident,"type parameter name").text;
        if(accept(Tok::KwExtends)){ p += " extends "; p += parseGenericType(); while(accept(Tok::AndAnd)) p += " & " + parseGenericType(); }
        out.push_back(p);
    } while(accept(Tok::Comma)); expect(Tok::Gt,"'>'"); return out;
}
std::string Parser::parseGenericType(){
    std::string t=parseQualifiedName();
    if(accept(Tok::Lt)){ t += "<"; do {
        if(accept(Tok::Question)){ t += "?"; if(accept(Tok::KwExtends)) t += " extends " + parseGenericType(); else if(accept(Tok::KwSuper)) t += " super " + parseGenericType(); }
        else t += parseGenericType();
    } while(accept(Tok::Comma)); expect(Tok::Gt,"'>'"); t += ">"; }
    return t;
}
void Parser::parseThrows(std::vector<std::string>& out){
    if(!accept(Tok::KwThrows)) return;
    do { out.push_back(parseQualifiedName()); } while(accept(Tok::Comma));
}
annotation::Annotation Parser::parseAnnotation(annotation::UseSite site){
    expect(Tok::At,"'@'");
    std::string name=parseQualifiedName();
    std::string value;
    bool marker=true;
    if(accept(Tok::LParen)){
        marker=false; int depth=1;
        while(depth>0 && !check(Tok::Eof)){
            if(check(Tok::LParen)){ value += "("; ++depth; ++i_; }
            else if(check(Tok::RParen)){ --depth; ++i_; if(depth>0) value += ")"; }
            else { if(!value.empty()) value += " "; value += cur().text; ++i_; }
        }
        if(depth!=0) error("unterminated annotation '"+name+"'");
    }
    return annotation::Annotation{name,value,site,marker};
}
std::vector<annotation::Annotation> Parser::parseAnnotations(annotation::UseSite site){
    std::vector<annotation::Annotation> out;
    while(check(Tok::At)) out.push_back(parseAnnotation(site));
    return out;
}
std::vector<annotation::Annotation> Parser::parseTypeAnnotations(){
    return parseAnnotations(annotation::UseSite::TypeUse);
}
Program Parser::parseProgram(){
    collectStructNames(); Program p; bool contentStarted=false;
    while(!check(Tok::Eof)){
        if(check(Tok::At)){
            auto anns=parseAnnotations(annotation::UseSite::Declaration);
            if(check(Tok::KwClass)||check(Tok::KwInterface)||check(Tok::KwEnum)||check(Tok::KwRecord)){
                contentStarted=true; unsigned mods=parseJavaModifiers(); p.classes.push_back(parseClass(mods,std::move(anns)));
            } else {
                if(contentStarted) error("declaration annotations must precede their declaration");
                for(auto &a:anns) p.annotations.add(std::move(a));
            }
        } else if(accept(Tok::KwImport)){ contentStarted=true;
            bool parsedDynamite=false;
            if(check(Tok::Ident)&&cur().text=="dynamite"&&peek(1).kind==Tok::Ident&&peek(1).text=="connector"){
                i_+=2; std::string first; std::string path; bool named=false;
                if(check(Tok::Ident)&&peek(1).kind==Tok::Assign){ first=cur().text; i_+=2; named=true; }
                while(!check(Tok::Semicolon)&&!check(Tok::Eof)){ path += cur().text; ++i_; }
                expect(Tok::Semicolon,"';'");
                if(path.empty()) error("Dynamite Connector import requires a source path");
                p.dynamiteImports.push_back({named?first:"",path}); parsedDynamite=true;
            } else if(check(Tok::DoubleColon)){
                size_t save=i_; ++i_;
                if(check(Tok::Ident)&&cur().text=="dynamite"){ ++i_;
                    if(check(Tok::DoubleColon)){ ++i_;
                        if(check(Tok::Ident)&&cur().text=="connector"){ ++i_;
                            if(check(Tok::DoubleColon)){ ++i_;
                                std::string reference;
                                if(check(Tok::Ident)&&peek(1).kind==Tok::DoubleColon){ reference=cur().text; i_+=2; }
                                std::string path;
                                while(!check(Tok::Semicolon)&&!check(Tok::Eof)){ path += cur().text; ++i_; }
                                expect(Tok::Semicolon,"';'");
                                if(path.empty()) error("Dynamite Connector import requires a source path");
                                p.dynamiteImports.push_back({reference,path}); parsedDynamite=true;
                            }
                        }
                    }
                }
                if(!parsedDynamite) i_=save;
            }
            if(!parsedDynamite && p.dynamiteImports.size()>1) {
                // No-op: ordinary imports remain independent of Dynamite references.
            }
            if(!parsedDynamite && check(Tok::Ident)&&cur().text=="permissible"&&peek(1).kind==Tok::Ident&&peek(1).text=="connector") {
                i_+=2; std::string first; std::string path; bool named=false;
                if(check(Tok::Ident)&&peek(1).kind==Tok::Assign){first=cur().text;i_+=2;named=true;}
                while(!check(Tok::Semicolon)&&!check(Tok::Eof)){path+=cur().text;++i_;}
                expect(Tok::Semicolon,"';'"); if(path.empty()) error("Permissible Connector import requires a source path");
                p.permissibleImports.push_back({named?first:"",path}); parsedDynamite=true;
            } else if(!parsedDynamite && check(Tok::DoubleColon)) {
                size_t save=i_; ++i_;
                if(check(Tok::Ident)&&cur().text=="permissible"){++i_;
                    if(check(Tok::DoubleColon)){++i_;
                        if(check(Tok::Ident)&&cur().text=="connector"){++i_;
                            if(check(Tok::DoubleColon)){++i_; std::string reference;
                                if(check(Tok::Ident)&&peek(1).kind==Tok::DoubleColon){reference=cur().text;i_+=2;}
                                std::string path; while(!check(Tok::Semicolon)&&!check(Tok::Eof)){path+=cur().text;++i_;}
                                expect(Tok::Semicolon,"';'"); if(path.empty()) error("Permissible Connector import requires a source path");
                                p.permissibleImports.push_back({reference,path}); parsedDynamite=true;
                            }
                        }
                    }
                }
                if(!parsedDynamite)i_=save;
            }
            if(!parsedDynamite) {
                p.imports.push_back(expect(Tok::Ident,"module name").text);
                expect(Tok::Semicolon,"';'");
            }
        }
        else if(check(Tok::KwStruct)){ contentStarted=true; p.structs.push_back(parseStruct()); }
        else {
            unsigned mods=parseJavaModifiers();
            if(check(Tok::KwClass)||check(Tok::KwInterface)||check(Tok::KwEnum)||check(Tok::KwRecord)){
                contentStarted=true; p.classes.push_back(parseClass(mods));
            } else error("expected annotation, import, struct, or Java type declaration at top level");
        }
    }
    if(p.classes.empty()) error("program contains no classes");
    return p;
}
StructDecl Parser::parseStruct(){expect(Tok::KwStruct,"'struct'");StructDecl s;s.name=expect(Tok::Ident,"struct name").text;expect(Tok::LBrace,"'{'");while(!check(Tok::RBrace)&&!check(Tok::Eof)){Field f;f.type=parseType();f.name=expect(Tok::Ident,"field name").text;expect(Tok::Semicolon,"';'");s.fields.push_back(std::move(f));}expect(Tok::RBrace,"'}'");return s;}
ClassDecl Parser::parseClass(unsigned classModifiers,std::vector<annotation::Annotation> annotations){
    Tok kind=cur().kind; ClassDecl c; c.java.kind=tokenTypeKind(kind); c.java.modifiers=classModifiers; c.java.annotations=std::move(annotations);
    if(kind!=Tok::KwClass&&kind!=Tok::KwInterface&&kind!=Tok::KwEnum&&kind!=Tok::KwRecord) error("expected Java type declaration");
    i_++; c.name=expect(Tok::Ident,"type name").text; c.java.qualifiedName=c.name; c.java.typeParameters=parseTypeParameters();
    if(accept(Tok::KwExtends)) c.java.superclass=parseQualifiedName();
    if(accept(Tok::KwImplements)) c.java.interfaces=parseTypeList(Tok::LBrace);
    expect(Tok::LBrace,"'{'");
    while(!check(Tok::RBrace)&&!check(Tok::Eof)){
        auto annotations=parseAnnotations(annotation::UseSite::Declaration);
        unsigned mods=parseJavaModifiers();
        if(!isTypeStart() && !(check(Tok::Ident)&&peek(1).kind==Tok::LParen)) error("expected a type/member declaration");
        if(check(Tok::Ident)&&peek(1).kind==Tok::LParen&&cur().text==c.name){
            Method m; m.isStatic=false; m.isProtected=(mods&JavaProtected)!=0; m.java.modifiers=mods; m.java.constructor=true; m.java.annotations=std::move(annotations);
            m.retType=c.name; m.name=expect(Tok::Ident,"constructor name").text; expect(Tok::LParen,"'('");
            if(!check(Tok::RParen)){do{Param p; p.annotations=parseAnnotations(annotation::UseSite::Parameter); p.type=parseType(&p.typeAnnotations); p.name=expect(Tok::Ident,"parameter name").text; m.params.push_back(std::move(p));}while(accept(Tok::Comma));}
            expect(Tok::RParen,"')'"); parseThrows(m.java.thrownTypes); m.body=parseBlock(); c.methods.push_back(std::move(m)); continue;
        }
        if(!isTypeStart()) error("expected a field or method type");
        size_t save=i_; parseType(); if(!check(Tok::Ident)){i_=save; error("expected a field or method name");}
        Tok after=peek(1).kind; i_=save;
        bool stat=(mods&JavaStatic)!=0, prot=(mods&JavaProtected)!=0;
        if(after==Tok::LParen)c.methods.push_back(parseMethod(stat,prot,mods,std::move(annotations)));
        else c.fields.push_back(parseField(stat,prot,mods,std::move(annotations)));
    }
    expect(Tok::RBrace,"'}'"); return c;
}
Field Parser::parseField(bool isStatic,bool isProtected,unsigned modifiers,std::vector<annotation::Annotation> annotations){Field f;f.isStatic=isStatic;f.isProtected=isProtected;f.java.modifiers=modifiers;f.annotations=std::move(annotations);f.type=parseType(&f.typeAnnotations);f.name=expect(Tok::Ident,"field name").text;if(accept(Tok::Assign))f.init=parseExpr();expect(Tok::Semicolon,"';'");return f;}
// A type is one of the scalar keywords, or an identifier naming a declared struct.
bool Parser::isTypeStart()const{Tok k=cur().kind;if(k==Tok::KwVoid||k==Tok::KwIntT||k==Tok::KwDoubleT||k==Tok::KwBoolT||k==Tok::KwStringT)return true;return k==Tok::Ident;}
std::string Parser::parseType(std::vector<annotation::Annotation>* typeAnnotations){
    if(typeAnnotations){auto a=parseTypeAnnotations(); typeAnnotations->insert(typeAnnotations->end(),a.begin(),a.end());}
    if(!isTypeStart()) error("expected a type");
    std::string t;
    if(check(Tok::Ident)) t=parseGenericType(); else { t=cur().text; i_++; }
    while(accept(Tok::LBracket)){ if(typeAnnotations){auto a=parseTypeAnnotations(); typeAnnotations->insert(typeAnnotations->end(),a.begin(),a.end());} expect(Tok::RBracket,"']'");t+="[]"; }
    return t;
}
Method Parser::parseMethod(bool isStatic,bool isProtected,unsigned modifiers,std::vector<annotation::Annotation> annotations){
    Method m;m.isStatic=isStatic;m.isProtected=isProtected;m.java.modifiers=modifiers;m.java.annotations=std::move(annotations);
    m.java.typeParameters=parseTypeParameters();m.retType=parseType(&m.java.typeAnnotations);m.name=expect(Tok::Ident,"method name").text;expect(Tok::LParen,"'('");
    if(!check(Tok::RParen)){do{Param p;p.annotations=parseAnnotations(annotation::UseSite::Parameter);p.type=parseType(&p.typeAnnotations);p.name=expect(Tok::Ident,"parameter name").text;m.params.push_back(std::move(p));}while(accept(Tok::Comma));}
    expect(Tok::RParen,"')'");parseThrows(m.java.thrownTypes);
    m.body=parseBlock();return m;
}
// Parse one "simple statement" without its trailing ';': a local variable
// declaration (`Type name [= expr]`), an assignment to a variable or struct
// field, or a bare expression statement. Used by parseStatement() and by the
// for-statement init/update clauses.
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
ExprP Parser::parseExpr(){return parseAssignment();}
ExprP Parser::parseAssignment(){
    ExprP e=parseConditional();
    if(check(Tok::Assign)||check(Tok::PlusAssign)||check(Tok::MinusAssign)||check(Tok::StarAssign)||check(Tok::SlashAssign)||check(Tok::PercentAssign)){
        std::string op=cur().text; i_++; return std::make_unique<AssignmentExpr>(op,std::move(e),parseAssignment());
    }
    return e;
}
ExprP Parser::parseConditional(){
    ExprP e=parseOr();
    if(accept(Tok::Question)){ExprP t=parseExpr();expect(Tok::Colon,"':'");return std::make_unique<ConditionalExpr>(std::move(e),std::move(t),parseConditional());}
    return e;
}
ExprP Parser::parseOr(){ExprP e=parseAnd();while(accept(Tok::OrOr))e=std::make_unique<Binary>("||",std::move(e),parseAnd());return e;}
ExprP Parser::parseAnd(){ExprP e=parseBitOr();while(accept(Tok::AndAnd))e=std::make_unique<Binary>("&&",std::move(e),parseBitOr());return e;}
ExprP Parser::parseBitOr(){ExprP e=parseBitXor();while(accept(Tok::BitOr))e=std::make_unique<Binary>("|",std::move(e),parseBitXor());return e;}
ExprP Parser::parseBitXor(){ExprP e=parseBitAnd();while(accept(Tok::BitXor))e=std::make_unique<Binary>("^",std::move(e),parseBitAnd());return e;}
ExprP Parser::parseBitAnd(){ExprP e=parseEquality();while(accept(Tok::BitAnd))e=std::make_unique<Binary>("&",std::move(e),parseEquality());return e;}
ExprP Parser::parseEquality(){ExprP e=parseComparison();for(;;){if(accept(Tok::EqEq))e=std::make_unique<Binary>("==",std::move(e),parseComparison());else if(accept(Tok::NotEq))e=std::make_unique<Binary>("!=",std::move(e),parseComparison());else break;}return e;}
ExprP Parser::parseComparison(){
    ExprP e=parseShift();
    for(;;){
        if(accept(Tok::Lt))e=std::make_unique<Binary>("<",std::move(e),parseShift());
        else if(accept(Tok::Le))e=std::make_unique<Binary>("<=",std::move(e),parseShift());
        else if(accept(Tok::Gt))e=std::make_unique<Binary>(">",std::move(e),parseShift());
        else if(accept(Tok::Ge))e=std::make_unique<Binary>(">=",std::move(e),parseShift());
        else if(accept(Tok::KwInstanceof)){std::string type=parseType();e=std::make_unique<InstanceOfExpr>(std::move(e),type);}
        else break;
    }
    return e;
}
ExprP Parser::parseShift(){ExprP e=parseAdditive();for(;;){if(accept(Tok::ShiftLeft))e=std::make_unique<Binary>("<<",std::move(e),parseAdditive());else if(accept(Tok::ShiftRight))e=std::make_unique<Binary>(">>",std::move(e),parseAdditive());else if(accept(Tok::UnsignedShiftRight))e=std::make_unique<Binary>(">>>",std::move(e),parseAdditive());else break;}return e;}
ExprP Parser::parseAdditive(){ExprP e=parseMultiplicative();for(;;){if(accept(Tok::Plus))e=std::make_unique<Binary>("+",std::move(e),parseMultiplicative());else if(accept(Tok::Minus))e=std::make_unique<Binary>("-",std::move(e),parseMultiplicative());else break;}return e;}
ExprP Parser::parseMultiplicative(){ExprP e=parseUnary();for(;;){if(accept(Tok::Star))e=std::make_unique<Binary>("*",std::move(e),parseUnary());else if(accept(Tok::Slash))e=std::make_unique<Binary>("/",std::move(e),parseUnary());else if(accept(Tok::Percent))e=std::make_unique<Binary>("%",std::move(e),parseUnary());else break;}return e;}
ExprP Parser::parseUnary(){
    if(accept(Tok::Minus))return std::make_unique<Unary>("-",parseUnary());
    if(accept(Tok::Plus))return std::make_unique<Unary>("+",parseUnary());
    if(accept(Tok::Not))return std::make_unique<Unary>("!",parseUnary());
    if(accept(Tok::BitNot))return std::make_unique<Unary>("~",parseUnary());
    if(accept(Tok::Increment))return std::make_unique<Unary>("++",parseUnary());
    if(accept(Tok::Decrement))return std::make_unique<Unary>("--",parseUnary());
    return parsePrimary();
}
ExprP Parser::parsePrimary(){
    const Token&t=cur();
    switch(t.kind){
    case Tok::Int:i_++;return parsePostfix(std::make_unique<IntLit>(std::stoll(t.text)));
    case Tok::Double:i_++;return parsePostfix(std::make_unique<DoubleLit>(std::stod(t.text)));
    case Tok::Str:i_++;return parsePostfix(std::make_unique<StrLit>(t.text));
    case Tok::KwTrue:i_++;return parsePostfix(std::make_unique<BoolLit>(true));
    case Tok::KwFalse:i_++;return parsePostfix(std::make_unique<BoolLit>(false));
    case Tok::KwNull:i_++;return parsePostfix(std::make_unique<NullLit>());
    case Tok::KwSuper:i_++;return parsePostfix(std::make_unique<SuperExpr>());
    case Tok::KwThis:i_++;return parsePostfix(std::make_unique<ThisExpr>());
    case Tok::LParen:{
        i_++;
        // Java cast disambiguation for a simple/qualified type: (Type) expression.
        if(check(Tok::Ident)||check(Tok::KwIntT)||check(Tok::KwDoubleT)||check(Tok::KwBoolT)||check(Tok::KwStringT)){
            size_t save=i_; std::string type;
            try { type=parseType(); if(check(Tok::RParen)){i_++; return std::make_unique<CastExpr>(type,parseUnary());} }
            catch(const std::exception&) {}
            i_=save;
        }
        ExprP e=parseExpr();expect(Tok::RParen,"')'");return parsePostfix(std::move(e));
    }
    case Tok::KwNew:{
        i_++;std::string type=parseGenericType();auto n=std::make_unique<NewExpr>(type);
        if(accept(Tok::LBracket)){
            n->typeName += "[]"; n->args.push_back(parseExpr()); expect(Tok::RBracket,"']'");
            return parsePostfix(std::move(n));
        }
        expect(Tok::LParen,"'('");if(!check(Tok::RParen)){do{n->args.push_back(parseExpr());}while(accept(Tok::Comma));}expect(Tok::RParen,"')'");
        return parsePostfix(std::move(n));
    }
    case Tok::Ident:{
        std::string name=t.text;i_++;
        ExprP base=std::make_unique<VarExpr>(name);
        while(accept(Tok::Dot)){
            std::string part=expect(Tok::Ident,"identifier after '.'").text;
            if(check(Tok::DoubleColon)){i_++;base=std::make_unique<MethodReferenceExpr>(std::move(base),part);continue;}
            if(check(Tok::LParen)){
                i_++;auto c=std::make_unique<MethodCall>(std::move(base),part);
                if(!check(Tok::RParen)){do{c->args.push_back(parseExpr());}while(accept(Tok::Comma));}
                expect(Tok::RParen,"')'");base=std::move(c);
            }else base=std::make_unique<MemberAccess>(std::move(base),part);
        }
        if(accept(Tok::LParen)){
            auto c=std::make_unique<Call>(name);
            if(!check(Tok::RParen)){do{c->args.push_back(parseExpr());}while(accept(Tok::Comma));}
            expect(Tok::RParen,"')'");base=std::move(c);
        }
        if(accept(Tok::DoubleColon)){
            std::string member=expect(Tok::Ident,"method name after '::'").text;
            base=std::make_unique<MethodReferenceExpr>(std::move(base),member);
        }
        if(accept(Tok::Dot)&&check(Tok::Ident)&&peek(1).kind==Tok::Ident&&peek(2).kind==Tok::Dot){
            // reserved for future qualified class literals; do not consume ambiguous forms.
            i_--;
        }
        return parsePostfix(std::move(base));
    }
    default:error(std::string("unexpected token '")+(t.text.empty()?tokName(t.kind):t.text)+"' in expression");
    }
}
ExprP Parser::parsePostfix(ExprP base){
    for(;;){
        if(accept(Tok::LBracket)){ExprP idx=parseExpr();expect(Tok::RBracket,"']'");base=std::make_unique<ArrayAccess>(std::move(base),std::move(idx));continue;}
        if(accept(Tok::Increment)){base=std::make_unique<Unary>("post++",std::move(base));continue;}
        if(accept(Tok::Decrement)){base=std::make_unique<Unary>("post--",std::move(base));continue;}
        if(!check(Tok::Dot)||peek(1).kind!=Tok::Ident)break;
        i_++;std::string member=expect(Tok::Ident,"member name after '.'").text;
        if(accept(Tok::LParen)){
            auto mc=std::make_unique<MethodCall>(std::move(base),member);
            if(!check(Tok::RParen)){do{mc->args.push_back(parseExpr());}while(accept(Tok::Comma));}
            expect(Tok::RParen,"')'");base=std::move(mc);
        }else if(accept(Tok::DoubleColon)){
            base=std::make_unique<MethodReferenceExpr>(std::move(base),member);
        }else base=std::make_unique<MemberAccess>(std::move(base),member);
    }
    return base;
}

std::unique_ptr<Block> Parser::parseBlock(){expect(Tok::LBrace,"'{'");auto b=std::make_unique<Block>();while(!check(Tok::RBrace)&&!check(Tok::Eof))b->stmts.push_back(parseStatement());expect(Tok::RBrace,"'}'");return b;}
StmtP Parser::parseIf(){expect(Tok::KwIf,"'if'");expect(Tok::LParen,"'('");auto s=std::make_unique<IfStmt>();s->cond=parseExpr();expect(Tok::RParen,"')'");s->thenS=parseStatement();if(accept(Tok::KwElse))s->elseS=parseStatement();return s;}
StmtP Parser::parseWhile(){expect(Tok::KwWhile,"'while'");expect(Tok::LParen,"'('");auto s=std::make_unique<WhileStmt>();s->cond=parseExpr();expect(Tok::RParen,"')'");s->body=parseStatement();return s;}
StmtP Parser::parseDo(){expect(Tok::KwDo,"'do'");auto s=std::make_unique<DoStmt>();s->body=parseStatement();expect(Tok::KwWhile,"'while'");expect(Tok::LParen,"'('");s->cond=parseExpr();expect(Tok::RParen,"')'");expect(Tok::Semicolon,"';'");return s;}
StmtP Parser::parseFor(){expect(Tok::KwFor,"'for'");expect(Tok::LParen,"'('");auto s=std::make_unique<ForStmt>();if(!check(Tok::Semicolon))s->init=parseSimpleStatement();expect(Tok::Semicolon,"';'");if(!check(Tok::Semicolon))s->cond=parseExpr();expect(Tok::Semicolon,"';'");if(!check(Tok::RParen))s->update=std::make_unique<ExprStmt>(parseExpr());expect(Tok::RParen,"')'");s->body=parseStatement();return s;}
StmtP Parser::parseSynchronized(){expect(Tok::KwSynchronized,"'synchronized'");expect(Tok::LParen,"'('");auto s=std::make_unique<SynchronizedStmt>();s->monitor=parseExpr();expect(Tok::RParen,"')'");s->body=parseBlock();return s;}
StmtP Parser::parseTry(){expect(Tok::KwTry,"'try'");auto s=std::make_unique<TryStmt>();s->body=parseBlock();while(accept(Tok::KwCatch)){expect(Tok::LParen,"'('");CatchClause c; c.type=expect(Tok::Ident,"exception type").text;c.variable=expect(Tok::Ident,"exception variable").text;expect(Tok::RParen,"')'");c.body=parseBlock();s->catches.push_back(std::move(c));}if(accept(Tok::KwFinally))s->finallyBlock=parseBlock();if(s->catches.empty()&&!s->finallyBlock)error("try requires catch or finally");return s;}
StmtP Parser::parseSwitch(){expect(Tok::KwSwitch,"'switch'");expect(Tok::LParen,"'('");auto s=std::make_unique<SwitchStmt>();s->selector=parseExpr();expect(Tok::RParen,"')'");expect(Tok::LBrace,"'{'");while(!check(Tok::RBrace)&&!check(Tok::Eof)){SwitchCase sc;if(accept(Tok::KwDefault)){sc.isDefault=true;expect(Tok::Colon,"':'");}else{expect(Tok::KwCase,"'case'");sc.labels.push_back(parseExpr());while(accept(Tok::Comma))sc.labels.push_back(parseExpr());expect(Tok::Colon,"':'");}while(!check(Tok::KwCase)&&!check(Tok::KwDefault)&&!check(Tok::RBrace)&&!check(Tok::Eof))sc.statements.push_back(parseStatement());s->cases.push_back(std::move(sc));}expect(Tok::RBrace,"'}'");return s;}
StmtP Parser::parseStatement(){
 if(check(Tok::LBrace))return parseBlock();
 if(check(Tok::KwIf))return parseIf();
 if(check(Tok::KwWhile))return parseWhile();
 if(check(Tok::KwDo))return parseDo();
 if(check(Tok::KwFor))return parseFor();
 if(check(Tok::KwSwitch))return parseSwitch();
 if(check(Tok::KwSynchronized))return parseSynchronized();
 if(check(Tok::KwTry))return parseTry();
 if(check(Tok::KwBreak)){i_++;auto s=std::make_unique<BreakStmt>();if(check(Tok::Ident)){s->label=cur().text;i_++;}expect(Tok::Semicolon,"';'");return s;}
 if(check(Tok::KwContinue)){i_++;auto s=std::make_unique<ContinueStmt>();if(check(Tok::Ident)){s->label=cur().text;i_++;}expect(Tok::Semicolon,"';'");return s;}
 if(check(Tok::KwReturn)){i_++;auto s=std::make_unique<ReturnStmt>();if(!check(Tok::Semicolon))s->value=parseExpr();expect(Tok::Semicolon,"';'");return s;}
 if(check(Tok::KwThrow)){i_++;auto s=std::make_unique<ThrowStmt>();s->value=parseExpr();expect(Tok::Semicolon,"';'");return s;}
 if(check(Tok::KwAssert)){i_++;auto s=std::make_unique<AssertStmt>();s->cond=parseExpr();if(accept(Tok::Colon))s->message=parseExpr();expect(Tok::Semicolon,"';'");return s;}
 if(check(Tok::KwYield)){i_++;auto s=std::make_unique<YieldStmt>();s->value=parseExpr();expect(Tok::Semicolon,"';'");return s;}
 if(accept(Tok::KwPrint)){auto s=std::make_unique<PrintStmt>();expect(Tok::LParen,"'('");s->expr=parseExpr();expect(Tok::RParen,"')'");expect(Tok::Semicolon,"';'");return s;}
 // Local declaration, assignment, or bare expression statement.
 StmtP s=parseSimpleStatement();expect(Tok::Semicolon,"';'");return s;
}
} // namespace sleela
