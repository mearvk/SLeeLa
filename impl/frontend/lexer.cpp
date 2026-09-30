// ===========================================================================
// lexer.cpp  --  Implementation of the Java-compatible Sleela tokenizer.
// ===========================================================================
#include "lexer.h"
#include <cctype>
#include <stdexcept>
#include <unordered_map>
namespace sleela {
const char* tokName(Tok t) {
    switch(t) {
        case Tok::Int:return "int-literal"; case Tok::Double:return "double-literal"; case Tok::Str:return "string-literal";
        case Tok::Ident:return "identifier"; case Tok::At:return "@";
        case Tok::KwClass:return "class"; case Tok::KwInterface:return "interface"; case Tok::KwEnum:return "enum"; case Tok::KwRecord:return "record";
        case Tok::KwStatic:return "static"; case Tok::KwProtected:return "protected"; case Tok::KwPublic:return "public"; case Tok::KwPrivate:return "private";
        case Tok::KwFinal:return "final"; case Tok::KwAbstract:return "abstract"; case Tok::KwNative:return "native";
        case Tok::KwSynchronized:return "synchronized"; case Tok::KwVolatile:return "volatile"; case Tok::KwTransient:return "transient";
        case Tok::KwStrictfp:return "strictfp"; case Tok::KwSealed:return "sealed"; case Tok::KwNonSealed:return "non-sealed"; case Tok::KwDefault:return "default";
        case Tok::KwExtends:return "extends"; case Tok::KwImplements:return "implements"; case Tok::KwThrows:return "throws"; case Tok::KwSuper:return "super"; case Tok::KwThis:return "this"; case Tok::KwInstanceof:return "instanceof";
        case Tok::KwVoid:return "void"; case Tok::KwIntT:return "int"; case Tok::KwDoubleT:return "double"; case Tok::KwBoolT:return "boolean"; case Tok::KwStringT:return "String";
        case Tok::KwIf:return "if"; case Tok::KwElse:return "else"; case Tok::KwWhile:return "while"; case Tok::KwFor:return "for"; case Tok::KwReturn:return "return";
        case Tok::KwTrue:return "true"; case Tok::KwFalse:return "false"; case Tok::KwPrint:return "print"; case Tok::KwNull:return "null";
        case Tok::KwImport:return "import"; case Tok::KwStruct:return "struct"; case Tok::KwNew:return "new";
        case Tok::LParen:return "("; case Tok::RParen:return ")"; case Tok::LBrace:return "{"; case Tok::RBrace:return "}";
        case Tok::LBracket:return "["; case Tok::RBracket:return "]"; case Tok::Semicolon:return ";"; case Tok::Comma:return ","; case Tok::Dot:return ".";
        case Tok::Assign:return "="; case Tok::Plus:return "+"; case Tok::Minus:return "-"; case Tok::Star:return "*"; case Tok::Slash:return "/";
        case Tok::Percent:return "%"; case Tok::PlusAssign:return "+="; case Tok::MinusAssign:return "-="; case Tok::StarAssign:return "*="; case Tok::SlashAssign:return "/="; case Tok::PercentAssign:return "%=";
        case Tok::Increment:return "++"; case Tok::Decrement:return "--"; case Tok::ShiftLeft:return "<<"; case Tok::ShiftRight:return ">>"; case Tok::UnsignedShiftRight:return ">>>";
        case Tok::EqEq:return "=="; case Tok::NotEq:return "!="; case Tok::Lt:return "<"; case Tok::Le:return "<=";
        case Tok::Gt:return ">"; case Tok::Ge:return ">="; case Tok::AndAnd:return "&&"; case Tok::OrOr:return "||"; case Tok::BitAnd:return "&"; case Tok::BitOr:return "|"; case Tok::BitXor:return "^"; case Tok::Not:return "!"; case Tok::BitNot:return "~"; case Tok::Question:return "?"; case Tok::Colon:return ":"; case Tok::DoubleColon:return "::"; case Tok::Arrow:return "->"; case Tok::Eof:return "<eof>";
    }
    return "?";
}
char Lexer::peek(int off) const { size_t p=pos_+(size_t)off; return p<src_.size()?src_[p]:'\0'; }
char Lexer::advance(){char c=src_[pos_++];if(c=='\n'){line_++;col_=1;}else col_++;return c;}
bool Lexer::match(char c){if(peek()==c){advance();return true;}return false;}
void Lexer::error(const std::string& msg) const {throw std::runtime_error("Lexical error (line "+std::to_string(line_)+", col "+std::to_string(col_)+"): "+msg);}
void Lexer::skipTrivia(){for(;;){char c=peek();if(c==' '||c=='\t'||c=='\r'||c=='\n')advance();else if(c=='#')while(!atEnd()&&peek()!='\n')advance();else if(c=='/'&&peek(1)=='/')while(!atEnd()&&peek()!='\n')advance();else if(c=='/'&&peek(1)=='*'){advance();advance();while(!atEnd()&&!(peek()=='*'&&peek(1)=='/'))advance();if(atEnd())error("unterminated block comment");advance();advance();}else break;}}
Token Lexer::makeNumber(){int L=line_,C=col_;std::string s;bool d=false;while(std::isdigit((unsigned char)peek()))s+=advance();if(peek()=='.'&&std::isdigit((unsigned char)peek(1))){d=true;s+=advance();while(std::isdigit((unsigned char)peek()))s+=advance();}return Token{d?Tok::Double:Tok::Int,s,L,C};}
Token Lexer::makeString(){int L=line_,C=col_;advance();std::string out;while(!atEnd()&&peek()!='"'){char c=advance();if(c=='\\'){char e=advance();switch(e){case'n':out+='\n';break;case't':out+='\t';break;case'r':out+='\r';break;case'\\':out+='\\';break;case'"':out+='"';break;default:out+=e;break;}}else out+=c;}if(atEnd())error("unterminated string literal");advance();return Token{Tok::Str,out,L,C};}
static const std::unordered_map<std::string,int> kContextualBuiltins13={
    {"Munction",1},{"start",1},{"connect",1},{"enable",1},{"send",1},{"thatch",1},{"consume",1},{"latch",1},{"closeWithReceipt",1},{"reception",1},
    {"synchroOpen",1},{"synchroDispatch",1},{"synchroReport",1},{"synchroClose",1},{"synchroSent",1},{"synchroReceived",1},{"synchroMean",1},{"synchroMin",1},{"synchroMax",1},{"synchroP95",1},{"synchroLoss",1},
    {"bestOfNew",1},{"bestOfWeight",1},{"bestOfMinVersion",1},{"bestOfCostBudget",1},{"bestOfCandidate",1},{"bestOfRecord",1},{"bestOfScore",1},{"bestOfBest",1},{"bestOfChoice",1},{"bestOfReport",1},{"bestOfClose",1},
    {"bestOfMean",1},{"bestOfLoss",1},{"bestOfJitter",1},{"bestOfCertainty",1},{"bestOfCandidateArch",1},{"bestOfArchRealized",1},{"bestOfArch",1},{"bestOfArchParam",1},{"bestOfArchState",1}
};
bool isContextualBuiltin(const std::string& name){return kContextualBuiltins13.find(name)!=kContextualBuiltins13.end();}
Token Lexer::makeIdentOrKeyword(){
    static const std::unordered_map<std::string,Tok> kw={
        {"class",Tok::KwClass},{"interface",Tok::KwInterface},{"enum",Tok::KwEnum},{"record",Tok::KwRecord},
        {"static",Tok::KwStatic},{"protected",Tok::KwProtected},{"public",Tok::KwPublic},{"private",Tok::KwPrivate},
        {"final",Tok::KwFinal},{"abstract",Tok::KwAbstract},{"native",Tok::KwNative},{"synchronized",Tok::KwSynchronized},
        {"volatile",Tok::KwVolatile},{"transient",Tok::KwTransient},{"strictfp",Tok::KwStrictfp},{"sealed",Tok::KwSealed},{"non-sealed",Tok::KwNonSealed},
        {"default",Tok::KwDefault},{"extends",Tok::KwExtends},{"implements",Tok::KwImplements},{"throws",Tok::KwThrows},{"super",Tok::KwSuper},{"this",Tok::KwThis},{"instanceof",Tok::KwInstanceof},
        {"void",Tok::KwVoid},{"int",Tok::KwIntT},{"double",Tok::KwDoubleT},{"boolean",Tok::KwBoolT},{"String",Tok::KwStringT},
        {"if",Tok::KwIf},{"else",Tok::KwElse},{"while",Tok::KwWhile},{"for",Tok::KwFor},{"return",Tok::KwReturn},{"true",Tok::KwTrue},{"false",Tok::KwFalse},
        {"print",Tok::KwPrint},{"null",Tok::KwNull},{"import",Tok::KwImport},{"struct",Tok::KwStruct},{"new",Tok::KwNew}
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
            case';':emit(Tok::Semicolon,";");break;case',':emit(Tok::Comma,",");break;case'.':emit(Tok::Dot,".");break;case'+':emit(Tok::Plus,"+");break;
            case'-':emit(Tok::Minus,"-");break;case'*':emit(Tok::Star,"*");break;case'/':emit(Tok::Slash,"/");break;case'%':emit(Tok::Percent,"%");break;
            case'=':if(match('='))emit(Tok::EqEq,"==");else if(match('>'))emit(Tok::Arrow,"->");else emit(Tok::Assign,"=");break;
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
            case'?':emit(Tok::Question,"?");break;case':':if(match(':'))emit(Tok::DoubleColon,"::");else emit(Tok::Colon,":");break;
            default:error(std::string("unexpected character '")+c+"'");
        }
    }
    return toks;
}
} // namespace sleela
