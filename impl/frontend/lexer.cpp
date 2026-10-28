// ===============================================================================
// lexer.cpp  --  Implementation of the Java-compatible Sleela tokenizer.
// ===============================================================================
#include "lexer.h"
#include <cctype>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
namespace sleela {
namespace {
std::string utf8FromCodePoint(uint32_t cp) {
    std::string out;
    if (cp <= 0x7F) {
        out.push_back(static_cast<char>(cp));
    } else if (cp <= 0x7FF) {
        out.push_back(static_cast<char>(0xC0 | ((cp >> 6) & 0x1F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp <= 0xFFFF) {
        if (cp >= 0xD800 && cp <= 0xDFFF) {
            throw std::runtime_error("invalid Unicode surrogate half in string literal");
        }
        out.push_back(static_cast<char>(0xE0 | ((cp >> 12) & 0x0F)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp <= 0x10FFFF) {
        out.push_back(static_cast<char>(0xF0 | ((cp >> 18) & 0x07)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else {
        throw std::runtime_error("invalid Unicode code point in string literal");
    }
    return out;
}
}
const char* tokName(Tok t) {
    switch(t) {
        case Tok::Int:return "int-literal"; case Tok::Double:return "double-literal"; case Tok::Str:return "string-literal";
        case Tok::Ident:return "identifier"; case Tok::At:return "@";
        case Tok::KwClass:return "class"; case Tok::KwInterface:return "interface"; case Tok::KwEnum:return "enum"; case Tok::KwRecord:return "record"; case Tok::KwStruct:return "struct";
        case Tok::KwStatic:return "static"; case Tok::KwProtected:return "protected"; case Tok::KwPublic:return "public"; case Tok::KwPrivate:return "private";
        case Tok::KwFinal:return "final"; case Tok::KwAbstract:return "abstract"; case Tok::KwNative:return "native";
        case Tok::KwSynchronized:return "synchronized"; case Tok::KwVolatile:return "volatile"; case Tok::KwTransient:return "transient";
        case Tok::KwStrictfp:return "strictfp"; case Tok::KwSealed:return "sealed"; case Tok::KwNonSealed:return "non-sealed"; case Tok::KwConst:return "const"; case Tok::KwUsualMem:return "usualmem"; case Tok::KwUsualGc:return "usualgc"; case Tok::KwAccurateMem:return "accuratemem";
        case Tok::KwPackage:return "package"; case Tok::KwRequire:return "require"; case Tok::KwImport:return "import"; case Tok::KwYield:return "yield";
        case Tok::KwIf:return "if"; case Tok::KwElse:return "else"; case Tok::KwSwitch:return "switch"; case Tok::KwCase:return "case"; case Tok::KwDefault:return "default";
        case Tok::KwWhile:return "while"; case Tok::KwFor:return "for"; case Tok::KwDo:return "do"; case Tok::KwBreak:return "break"; case Tok::KwContinue:return "continue";
        case Tok::KwReturn:return "return"; case Tok::KwThrow:return "throw"; case Tok::KwTry:return "try"; case Tok::KwCatch:return "catch"; case Tok::KwFinally:return "finally"; case Tok::KwAssert:return "assert";
        case Tok::KwExtends:return "extends"; case Tok::KwImplements:return "implements"; case Tok::KwThrows:return "throws"; case Tok::KwSuper:return "super"; case Tok::KwThis:return "this"; case Tok::KwInstanceof:return "instanceof";
        case Tok::KwNew:return "new"; case Tok::KwNull:return "null"; case Tok::KwTrue:return "true"; case Tok::KwFalse:return "false"; case Tok::KwPrint:return "print";
        case Tok::KwVoid:return "void"; case Tok::KwIntT:return "int"; case Tok::KwLongT:return "long"; case Tok::KwInt64T:return "int64"; case Tok::KwInt128T:return "int128"; case Tok::KwDoubleT:return "double"; case Tok::KwBoolT:return "boolean"; case Tok::KwStringT:return "String";
        case Tok::LParen:return "("; case Tok::RParen:return ")"; case Tok::LBrace:return "{"; case Tok::RBrace:return "}";
        case Tok::LBracket:return "["; case Tok::RBracket:return "]"; case Tok::Semicolon:return ";"; case Tok::Comma:return ","; case Tok::Dot:return ".";
        case Tok::Assign:return "="; case Tok::Plus:return "+"; case Tok::Minus:return "-"; case Tok::Star:return "*"; case Tok::Slash:return "/";
        case Tok::Percent:return "%"; case Tok::PlusAssign:return "+="; case Tok::MinusAssign:return "-="; case Tok::StarAssign:return "*="; case Tok::SlashAssign:return "/="; case Tok::PercentAssign:return "%=";
        case Tok::Increment:return "++"; case Tok::Decrement:return "--"; case Tok::ShiftLeft:return "<<"; case Tok::ShiftRight:return ">>"; case Tok::UnsignedShiftRight:return ">>>";
        case Tok::EqEq:return "=="; case Tok::NotEq:return "!="; case Tok::Lt:return "<"; case Tok::Le:return "<=";
        case Tok::Gt:return ">"; case Tok::Ge:return ">="; case Tok::AndAnd:return "&&"; case Tok::OrOr:return "||"; case Tok::BitAnd:return "&"; case Tok::BitOr:return "|"; case Tok::BitXor:return "^"; case Tok::BitNot:return "~";
        case Tok::Not:return "!"; case Tok::Question:return "?"; case Tok::Colon:return ":"; case Tok::DoubleColon:return "::"; case Tok::TripleColon:return ":::"; case Tok::Arrow:return "->";
        case Tok::Eof:return "<eof>";
    }
    return "?";
}
char Lexer::peek(int off) const { size_t p=pos_+(size_t)off; return p<src_.size()?src_[p]: '\0'; }
char Lexer::advance(){char c=src_[pos_++];if(c=='\n'){line_++;col_=1;}else col_++;return c;}
bool Lexer::match(char c){if(peek()==c){advance();return true;}return false;}
void Lexer::error(const std::string& msg) const {throw std::runtime_error("Lexical error (line "+std::to_string(line_)+", col "+std::to_string(col_)+"): "+msg);}
void Lexer::skipTrivia(){
    auto atLinePrefix=[&](){size_t p=pos_;while(p>0&&src_[p-1]!='\n'){char b=src_[p-1];if(b!=' '&&b!='\t'&&b!='\r')return false;--p;}return true;};
    for(;;){char c=peek();
        if(c==' '||c=='\t'||c=='\r'||c=='\n')advance();
        else if(c=='#')while(!atEnd()&&peek()!='\n')advance();
        else if(c=='/'&&peek(1)=='/')while(!atEnd()&&peek()!='\n')advance();
        else if(atLinePrefix()&&c=='<'&&peek(1)=='<'){advance();advance();bool closed=false;while(!atEnd()){if(peek()=='>'&&peek(1)=='>'){advance();advance();closed=true;break;}advance();}if(!closed)error("unterminated angle-bracket block comment");}
        else if(atLinePrefix()&&c=='<'&&peek(1)!='<'){size_t e=pos_+1;while(e<src_.size()&&src_[e]!='\n'&&src_[e]!='>')++e;if(e<src_.size()&&src_[e]=='>')while(!atEnd()&&peek()!='\n')advance();else break;}
        else break;
    }
}
Token Lexer::makeNumber(){
    int L=line_,C=col_;std::string s;
    // Radix-prefixed integer literals: 0x/0X hex, 0b/0B binary, 0o/0O octal.
    // The prefix is kept in the token text; the parser decodes the value.
    if(peek()=='0'){
        char p1=peek(1);
        auto isHex=[](char c){return (c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F');};
        if(p1=='x'||p1=='X'){
            s+=advance();s+=advance();                 // consume "0x"
            if(!isHex(peek()))error("hexadecimal literal has no digits after '0x'");
            while(isHex(peek())||peek()=='_'){char c=advance();if(c!='_')s+=c;}
            return Token{Tok::Int,s,L,C};
        }
        if(p1=='b'||p1=='B'){
            s+=advance();s+=advance();                 // consume "0b"
            if(peek()!='0'&&peek()!='1')error("binary literal has no digits after '0b'");
            while(peek()=='0'||peek()=='1'||peek()=='_'){char c=advance();if(c!='_')s+=c;}
            return Token{Tok::Int,s,L,C};
        }
        if(p1=='o'||p1=='O'){
            advance();advance();                       // consume "0o" (not kept)
            s+='0';                                     // keep a leading 0 so stoll base 0 reads octal
            if(peek()<'0'||peek()>'7')error("octal literal has no digits after '0o'");
            while((peek()>='0'&&peek()<='7')||peek()=='_'){char c=advance();if(c!='_')s+=c;}
            return Token{Tok::Int,s,L,C};
        }
    }
    bool d=false;while(std::isdigit((unsigned char)peek()))s+=advance();if(peek()=='.'&&std::isdigit((unsigned char)peek(1))){d=true;s+=advance();while(std::isdigit((unsigned char)peek()))s+=advance();}if(peek()=='e'||peek()=='E'){s+=advance();if(peek()=='+'||peek()=='-')s+=advance();while(std::isdigit((unsigned char)peek()))s+=advance();}return Token{d||peek()=='e'||peek()=='E'?Tok::Double:Tok::Int,s,L,C};}
Token Lexer::makeString(){int L=line_,C=col_;advance();std::string out;while(!atEnd()&&peek()!='"'){char c=advance();if(c=='\\'){if(atEnd())error("unterminated escape sequence");char e=advance();switch(e){case'n':out+='\n';break;case't':out+='\t';break;case'r':out+='\r';break;case'b':out+='\b';break;case'f':out+='\f';break;case'0':out+='\0';break;case'\\':out+='\\';break;case'"':out+='"';break;case'u':{uint32_t cp=0;for(int i=0;i<4;i++){if(atEnd())error("unterminated unicode escape");char hex=advance();int v=0;if(hex>='0'&&hex<='9')v=hex-'0';else if(hex>='a'&&hex<='f')v=10+(hex-'a');else if(hex>='A'&&hex<='F')v=10+(hex-'A');else error("invalid unicode escape");cp=(cp<<4)|v;}out+=utf8FromCodePoint(cp);break;}default:out+=e;break;}}else out+=c;}if(atEnd())error("unterminated string literal");advance();return Token{Tok::Str,out,L,C};}
static const std::unordered_map<std::string,int> kContextualBuiltins13={
    {"Munction",1},{"start",1},{"connect",1},{"enable",1},{"send",1},{"thatch",1},{"consume",1},{"latch",1},{"closeWithReceipt",1},{"reception",1},
    {"synchroOpen",1},{"synchroDispatch",1},{"synchroReport",1},{"synchroClose",1},{"synchroSent",1},{"synchroReceived",1},{"synchroMean",1},{"synchroMin",1},{"synchroMax",1},{"synchroP95",1},{"synchroLoss",1},
    {"bestOfNew",1},{"bestOfWeight",1},{"bestOfMinVersion",1},{"bestOfCostBudget",1},{"bestOfCandidate",1},{"bestOfRecord",1},{"bestOfScore",1},{"bestOfBest",1},{"bestOfChoice",1},{"bestOfReport",1},{"bestOfClose",1},
    {"bestOfMean",1},{"bestOfLoss",1},{"bestOfJitter",1},{"bestOfCertainty",1},{"bestOfCandidateArch",1},{"bestOfArchRealized",1},{"bestOfArch",1},{"bestOfArchParam",1},{"bestOfArchState",1},
    /* Idea System(TM) realtime canvas (idea/realtime) native-bridge built-ins. */
    {"ideaCanvasCreate",1},{"ideaCanvasDestroy",1},{"ideaCanvasCols",1},{"ideaCanvasRows",1},{"ideaCanvasCount",1},
    {"ideaCanvasSetParams",1},{"ideaCanvasExcite",1},{"ideaCanvasStep",1},{"ideaCanvasAdvance",1},{"ideaCanvasTimestep",1},
    {"ideaCanvasIntensityAt",1},{"ideaCanvasHasRenderer",1},{"ideaCanvasOpenWindow",1},{"ideaCanvasShouldClose",1},{"ideaCanvasPresent",1},{"ideaCanvasRun",1},
    {"ideaCanvasSetBackendPolicy",1},{"ideaCanvasPolicy",1},{"ideaCanvasActiveBackend",1},{"ideaCanvasGpuAvailable",1},{"ideaCanvasMeasuredHz",1},{"ideaCanvasMigrate",1},
    /* GC-hint words (syntax 1.9). `gc` and `mem` are CONTEXTUAL, not reserved:
     * they tokenize as ordinary identifiers and are only special as the whole
     * right-hand side of an assignment (`x = gc N;` / `x = mem N;`), so they
     * remain usable as identifiers everywhere else. The parser recognises the
     * hint positionally; this registry entry documents them as contextual.
     * `ran` is the System / already-Ran-in-RAM namespace qualifier: `ran::mem`,
     * `ran::gc` name the system meaning explicitly so any word (mem, gc, ...)
     * stays usable as a class/variable name. `ran` is contextual too -- it is
     * special only immediately before `::`. */
    {"gc",1},{"mem",1},{"ran",1}
};
bool isContextualBuiltin(const std::string& name){return kContextualBuiltins13.find(name)!=kContextualBuiltins13.end();}
// True for the two GC-hint spellings recognised as the RHS of `x = gc/mem N;`.
bool isGcHintWord(const std::string& name){return name=="gc"||name=="mem";}
Token Lexer::makeIdentOrKeyword(){
    static const std::unordered_map<std::string,Tok> kw={
        {"class",Tok::KwClass},{"interface",Tok::KwInterface},{"enum",Tok::KwEnum},{"record",Tok::KwRecord},{"struct",Tok::KwStruct},
        {"static",Tok::KwStatic},{"protected",Tok::KwProtected},{"public",Tok::KwPublic},{"private",Tok::KwPrivate},
        {"final",Tok::KwFinal},{"abstract",Tok::KwAbstract},{"native",Tok::KwNative},{"synchronized",Tok::KwSynchronized},
        {"volatile",Tok::KwVolatile},{"transient",Tok::KwTransient},{"strictfp",Tok::KwStrictfp},{"sealed",Tok::KwSealed},{"non-sealed",Tok::KwNonSealed},{"const",Tok::KwConst},{"usualmem",Tok::KwUsualMem},{"usualgc",Tok::KwUsualGc},
        {"package",Tok::KwPackage},{"require",Tok::KwRequire},{"import",Tok::KwImport},{"yield",Tok::KwYield},
        {"if",Tok::KwIf},{"else",Tok::KwElse},{"switch",Tok::KwSwitch},{"case",Tok::KwCase},{"default",Tok::KwDefault},
        {"while",Tok::KwWhile},{"for",Tok::KwFor},{"do",Tok::KwDo},{"break",Tok::KwBreak},{"continue",Tok::KwContinue},
        {"return",Tok::KwReturn},{"throw",Tok::KwThrow},{"try",Tok::KwTry},{"catch",Tok::KwCatch},{"finally",Tok::KwFinally},
        {"default",Tok::KwDefault},{"extends",Tok::KwExtends},{"implements",Tok::KwImplements},{"throws",Tok::KwThrows},{"super",Tok::KwSuper},{"this",Tok::KwThis},{"instanceof",Tok::KwInstanceof},
        {"new",Tok::KwNew},{"null",Tok::KwNull},{"true",Tok::KwTrue},{"false",Tok::KwFalse},{"print",Tok::KwPrint},
        {"void",Tok::KwVoid},{"int",Tok::KwIntT},{"long",Tok::KwLongT},{"int64",Tok::KwInt64T},{"int128",Tok::KwInt128T},{"double",Tok::KwDoubleT},{"boolean",Tok::KwBoolT},{"String",Tok::KwStringT}
    };
    int L=line_,C=col_;std::string s;while(std::isalnum((unsigned char)peek())||peek()=='_'||peek()=='-')s+=advance();
    auto it=kw.find(s);return Token{it!=kw.end()?it->second:Tok::Ident,s,L,C};
}
std::vector<Token> Lexer::tokenize(){
    std::vector<Token> toks;
    for(;;){skipTrivia();if(atEnd()){toks.push_back(Token{Tok::Eof,"",line_,col_});break;}int L=line_,C=col_;char c=peek();
        if(std::isdigit((unsigned char)c)){toks.push_back(makeNumber());continue;} if(c=='"'){toks.push_back(makeString());continue;}
        if(std::isalpha((unsigned char)c)||c=='_'){toks.push_back(makeIdentOrKeyword());continue;}
        advance();auto emit=[&](Tok k,const char*txt){toks.push_back(Token{k,txt,L,C});};
        switch(c){
            case'@':emit(Tok::At,"@");break;case'(':emit(Tok::LParen,"(");break;case')':emit(Tok::RParen,")");break;
            case'{':emit(Tok::LBrace,"{");break;case'}':emit(Tok::RBrace,"}");break;case'[':emit(Tok::LBracket,"[");break;case']':emit(Tok::RBracket,"]");break;
            case';':emit(Tok::Semicolon,";");break;case',':emit(Tok::Comma,",");break;case'.':emit(Tok::Dot,".");break;
            case'=':if(match('='))emit(Tok::EqEq,"==");else emit(Tok::Assign,"=");break;
            case'!':emit(match('=')?Tok::NotEq:Tok::Not,"!");break;
            case'+':if(match('+'))emit(Tok::Increment,"++");else if(match('='))emit(Tok::PlusAssign,"+=");else emit(Tok::Plus,"+");break;
            case'-':if(match('-'))emit(Tok::Decrement,"--");else if(match('='))emit(Tok::MinusAssign,"-=");else if(match('>'))emit(Tok::Arrow,"->");else emit(Tok::Minus,"-");break;
            case'*':emit(match('=')?Tok::StarAssign:Tok::Star,"*");break;case'/':emit(match('=')?Tok::SlashAssign:Tok::Slash,"/");break;
            case'%':emit(match('=')?Tok::PercentAssign:Tok::Percent,"%");break;
            case'<':if(match('<'))emit(Tok::ShiftLeft,"<<");else emit(match('=')?Tok::Le:Tok::Lt,"<");break;
            case'>':if(match('>')){if(match('>'))emit(Tok::UnsignedShiftRight,">>>");else emit(Tok::ShiftRight,">>");}else emit(match('=')?Tok::Ge:Tok::Gt,">");break;
            case'&':if(match('&'))emit(Tok::AndAnd,"&&");else emit(Tok::BitAnd,"&");break;
            case'|':if(match('|'))emit(Tok::OrOr,"||");else emit(Tok::BitOr,"|");break;
            case'^':emit(Tok::BitXor,"^");break;case'~':emit(Tok::BitNot,"~");break;
            case'?':emit(Tok::Question,"?");break;case':':if(match(':')){if(match(':'))emit(Tok::TripleColon,":::");else emit(Tok::DoubleColon,"::");}else emit(Tok::Colon,":");break;
            default:error(std::string("unexpected character '")+c+"'");
        }
    }
    return toks;
}
} // namespace sleela
