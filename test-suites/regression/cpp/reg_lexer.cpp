// reg_lexer.cpp -- regression suite for impl/frontend/lexer.cpp
//
// Contract (SLEELA.syntax lexical grammar, lexer.h): the tokenizer turns source
// text into a vector of Tokens that always ends with Eof, tracks 1-based
// line/column, and reports every lexical error by throwing std::runtime_error.
//
// Pinned grammar facts (each one is a documented rule or a defect that was
// fixed once and must not return):
//   * comments are  #...  //...  < ... > (one line, at line start)  and
//     << ... >> (block, at line start).  C-style /* */ is NOT a comment.
//   * '-' continues an identifier only when an identifier character follows
//     (jdk-25, version-mual, non-sealed); `j--`, `j-=1` and `x - 1` split.
//   * numeric forms: 0x 0b 0o prefixes, '_' separators, L/l, f/F/d/D, exponents.
//   * `gc`, `mem`, `ran` are contextual builtins, never reserved words.
#include "../../../impl/frontend/lexer.h"
#include "../c/regression.h"

#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

using sleela::Lexer;
using sleela::Tok;
using sleela::Token;

namespace {

std::vector<Token> lex(const std::string &src) { return Lexer(src).tokenize(); }

// Returns the error message, or "" when lexing succeeded.
std::string lex_error(const std::string &src) {
    try { (void)lex(src); } catch (const std::runtime_error &e) { return e.what(); }
    return "";
}

bool contains(const std::string &hay, const char *needle) {
    return hay.find(needle) != std::string::npos;
}

// "kind:text" rendering of the stream without the trailing Eof.
std::string kinds(const std::string &src) {
    std::string out;
    for (const Token &t : lex(src)) {
        if (t.kind == Tok::Eof) break;
        if (!out.empty()) out += ' ';
        out += sleela::tokName(t.kind);
    }
    return out;
}

std::string texts(const std::string &src) {
    std::string out;
    for (const Token &t : lex(src)) {
        if (t.kind == Tok::Eof) break;
        if (!out.empty()) out += '|';
        out += t.text;
    }
    return out;
}

struct Vocab { const char *src; Tok kind; const char *text; };

// Every keyword the lexer recognises, independent of lexer.cpp's table.
const Vocab kKeywords[] = {
    {"class", Tok::KwClass, "class"}, {"interface", Tok::KwInterface, "interface"},
    {"enum", Tok::KwEnum, "enum"}, {"record", Tok::KwRecord, "record"}, {"struct", Tok::KwStruct, "struct"},
    {"static", Tok::KwStatic, "static"}, {"protected", Tok::KwProtected, "protected"},
    {"public", Tok::KwPublic, "public"}, {"private", Tok::KwPrivate, "private"},
    {"final", Tok::KwFinal, "final"}, {"abstract", Tok::KwAbstract, "abstract"}, {"native", Tok::KwNative, "native"},
    {"synchronized", Tok::KwSynchronized, "synchronized"}, {"volatile", Tok::KwVolatile, "volatile"},
    {"transient", Tok::KwTransient, "transient"}, {"strictfp", Tok::KwStrictfp, "strictfp"},
    {"sealed", Tok::KwSealed, "sealed"}, {"non-sealed", Tok::KwNonSealed, "non-sealed"},
    {"const", Tok::KwConst, "const"}, {"usualmem", Tok::KwUsualMem, "usualmem"}, {"usualgc", Tok::KwUsualGc, "usualgc"},
    {"package", Tok::KwPackage, "package"}, {"require", Tok::KwRequire, "require"},
    {"import", Tok::KwImport, "import"}, {"yield", Tok::KwYield, "yield"},
    {"if", Tok::KwIf, "if"}, {"else", Tok::KwElse, "else"}, {"switch", Tok::KwSwitch, "switch"},
    {"case", Tok::KwCase, "case"}, {"default", Tok::KwDefault, "default"},
    {"while", Tok::KwWhile, "while"}, {"for", Tok::KwFor, "for"}, {"do", Tok::KwDo, "do"},
    {"break", Tok::KwBreak, "break"}, {"continue", Tok::KwContinue, "continue"},
    {"return", Tok::KwReturn, "return"}, {"throw", Tok::KwThrow, "throw"}, {"try", Tok::KwTry, "try"},
    {"catch", Tok::KwCatch, "catch"}, {"finally", Tok::KwFinally, "finally"},
    {"extends", Tok::KwExtends, "extends"}, {"implements", Tok::KwImplements, "implements"},
    {"throws", Tok::KwThrows, "throws"}, {"super", Tok::KwSuper, "super"}, {"this", Tok::KwThis, "this"},
    {"instanceof", Tok::KwInstanceof, "instanceof"}, {"new", Tok::KwNew, "new"}, {"null", Tok::KwNull, "null"},
    {"true", Tok::KwTrue, "true"}, {"false", Tok::KwFalse, "false"}, {"print", Tok::KwPrint, "print"},
    {"void", Tok::KwVoid, "void"}, {"int", Tok::KwIntT, "int"}, {"long", Tok::KwLongT, "long"},
    {"int64", Tok::KwInt64T, "int64"}, {"int128", Tok::KwInt128T, "int128"}, {"double", Tok::KwDoubleT, "double"},
    {"boolean", Tok::KwBoolT, "boolean"}, {"String", Tok::KwStringT, "String"},
};

// Operators and punctuation; the token text must equal the spelling.
struct OpVocab { const char *src; Tok kind; bool text_checked; };
const OpVocab kOps[] = {
    {"@", Tok::At, true}, {"(", Tok::LParen, true}, {")", Tok::RParen, true}, {"{", Tok::LBrace, true},
    {"}", Tok::RBrace, true}, {"[", Tok::LBracket, true}, {"]", Tok::RBracket, true}, {";", Tok::Semicolon, true},
    {",", Tok::Comma, true}, {".", Tok::Dot, true}, {"=", Tok::Assign, true}, {"+", Tok::Plus, true},
    {"-", Tok::Minus, true}, {"*", Tok::Star, true}, {"/", Tok::Slash, true}, {"%", Tok::Percent, true},
    {"+=", Tok::PlusAssign, true}, {"-=", Tok::MinusAssign, true}, {"*=", Tok::StarAssign, true},
    {"/=", Tok::SlashAssign, true}, {"%=", Tok::PercentAssign, true}, {"&=", Tok::AndAssign, true},
    {"|=", Tok::OrAssign, true}, {"^=", Tok::XorAssign, true}, {"<<=", Tok::ShlAssign, true},
    {">>=", Tok::ShrAssign, true}, {">>>=", Tok::UshrAssign, true}, {"++", Tok::Increment, true},
    {"--", Tok::Decrement, true}, {"<<", Tok::ShiftLeft, true}, {">>", Tok::ShiftRight, true},
    {">>>", Tok::UnsignedShiftRight, true}, {"==", Tok::EqEq, true}, {"!=", Tok::NotEq, true},
    {"<", Tok::Lt, true}, {"<=", Tok::Le, true}, {">", Tok::Gt, true}, {">=", Tok::Ge, true},
    {"&&", Tok::AndAnd, true}, {"||", Tok::OrOr, true}, {"&", Tok::BitAnd, true}, {"|", Tok::BitOr, true},
    {"^", Tok::BitXor, true}, {"!", Tok::Not, true}, {"~", Tok::BitNot, true}, {"?", Tok::Question, true},
    {":", Tok::Colon, true}, {"::", Tok::DoubleColon, true}, {":::", Tok::TripleColon, true}, {"->", Tok::Arrow, true},
};

// ----------------------------------------------------------------------- cases

void t_keywords() {
    for (const Vocab &v : kKeywords) {
        const std::vector<Token> t = lex(v.src);
        REG_REQUIRE(t.size() == 2);
        reg_note_check();
        if (t[0].kind != v.kind || t[0].text != v.text)
            reg_fail(__FILE__, __LINE__, "keyword \"%s\" lexed as %s", v.src, sleela::tokName(t[0].kind));
        REG_CHECK(std::string(sleela::tokName(v.kind)) == v.text);   // name <-> spelling agree
        REG_CHECK(t[1].kind == Tok::Eof);
        // a longer identifier that merely starts with a keyword is an identifier
        const std::string longer = std::string(v.src) + "x";
        const std::vector<Token> u = lex(longer);
        REG_CHECK(u.size() == 2 && u[0].kind == Tok::Ident && u[0].text == longer);
    }
    REG_CHECK(lex("Class")[0].kind == Tok::Ident);                    // case-sensitive
    REG_CHECK(lex("STRING")[0].kind == Tok::Ident);
    REG_CHECK(lex("string")[0].kind == Tok::Ident);                   // only `String` is the type
}

void t_operators_longest_match() {
    for (const OpVocab &v : kOps) {
        const std::string src = std::string("a ") + v.src + " b";
        const std::vector<Token> t = lex(src);
        REG_REQUIRE(t.size() == 4);
        reg_note_check();
        if (t[1].kind != v.kind)
            reg_fail(__FILE__, __LINE__, "operator \"%s\" lexed as %s", v.src, sleela::tokName(t[1].kind));
        if (v.text_checked) REG_CHECK(t[1].text == v.src);
        REG_CHECK(std::string(sleela::tokName(v.kind)) == v.src);
    }
    // maximal munch on adjacent operators
    REG_CHECK(kinds("a>>>=b") == "identifier >>>= identifier");
    REG_CHECK(kinds("a>>>b") == "identifier >>> identifier");
    REG_CHECK(kinds("a>>=b") == "identifier >>= identifier");
    REG_CHECK(kinds("a>>b") == "identifier >> identifier");
    REG_CHECK(kinds("a<<=b") == "identifier <<= identifier");
    REG_CHECK(kinds("a+++b") == "identifier ++ + identifier");
    REG_CHECK(kinds("a---b") == "identifier -- - identifier");
    REG_CHECK(kinds("a&&&b") == "identifier && & identifier");
    REG_CHECK(kinds("a::::b") == "identifier ::: : identifier");
    REG_CHECK(kinds("a::::::b") == "identifier ::: ::: identifier");
    REG_CHECK(kinds("a->b") == "identifier -> identifier");
    REG_CHECK(kinds("a=>b") == "identifier = > identifier");
    REG_CHECK(kinds("a===b") == "identifier == = identifier");
    REG_CHECK(kinds("!!a") == "! ! identifier");
}

// Regression (was LEX-CMP-TEXT): the token text of !=, <= and >= is the full
// operator, so diagnostics that print Token::text show what was written.
void t_comparison_token_text_is_the_full_operator() {
    REG_CHECK(lex("a != b")[1].text == "!=");
    REG_CHECK(lex("a <= b")[1].text == "<=");
    REG_CHECK(lex("a >= b")[1].text == ">=");
    REG_CHECK(lex("a ! b")[1].text == "!");
    REG_CHECK(lex("a < b")[1].text == "<");
    REG_CHECK(lex("a > b")[1].text == ">");
    REG_CHECK(texts("a!=b<=c>=d") == "a|!=|b|<=|c|>=|d");
}

void t_identifiers_and_hyphen_rule() {
    // SLEELA.syntax 43.6 rule 12: '-' joins identifier characters only when an
    // identifier character follows it.
    REG_CHECK(kinds("jdk-25") == "identifier");
    REG_CHECK(texts("jdk-25 version-mual") == "jdk-25|version-mual");
    REG_CHECK(texts("a-b") == "a-b");                                  // pinned: one identifier
    REG_CHECK(texts("i-1") == "i-1");                                  // pinned: one identifier
    REG_CHECK(kinds("j--") == "identifier --");
    REG_CHECK(kinds("j-=1") == "identifier -= int-literal");
    REG_CHECK(kinds("x - 1") == "identifier - int-literal");
    REG_CHECK(kinds("x -1") == "identifier - int-literal");
    REG_CHECK(kinds("x- 1") == "identifier - int-literal");            // trailing '-' ends the name
    REG_CHECK(kinds("a->b") == "identifier -> identifier");            // '>' is not an identifier char
    REG_CHECK(texts("_under_score9") == "_under_score9");
    REG_CHECK(kinds("9lives") == "int-literal identifier");            // digits do not start identifiers
    // contextual builtins are ordinary identifiers to the lexer
    REG_CHECK(kinds("gc mem ran") == "identifier identifier identifier");
    REG_CHECK(sleela::isContextualBuiltin("gc") && sleela::isContextualBuiltin("mem") &&
              sleela::isContextualBuiltin("ran") && sleela::isContextualBuiltin("Munction") &&
              sleela::isContextualBuiltin("synchroOpen") && sleela::isContextualBuiltin("bestOfNew") &&
              sleela::isContextualBuiltin("ideaCanvasCreate"));
    REG_CHECK(!sleela::isContextualBuiltin("") && !sleela::isContextualBuiltin("foo") &&
              !sleela::isContextualBuiltin("GC") && !sleela::isContextualBuiltin("class"));
    REG_CHECK(sleela::isGcHintWord("gc") && sleela::isGcHintWord("mem"));
    REG_CHECK(!sleela::isGcHintWord("ran") && !sleela::isGcHintWord("") && !sleela::isGcHintWord("Mem"));
}

void t_numeric_literals() {
    struct N { const char *src; Tok kind; const char *text; };
    const N ok[] = {
        {"0", Tok::Int, "0"}, {"42", Tok::Int, "42"}, {"1_000_000", Tok::Int, "1000000"},
        {"10L", Tok::Int, "10"}, {"10l", Tok::Int, "10"}, {"0x1F", Tok::Int, "0x1F"}, {"0XFF", Tok::Int, "0XFF"},
        {"0xFF_FF", Tok::Int, "0xFFFF"}, {"0x1FL", Tok::Int, "0x1F"}, {"0b101_1", Tok::Int, "0b1011"},
        {"0B11", Tok::Int, "0B11"}, {"0o17", Tok::Int, "017"}, {"0O7", Tok::Int, "07"},
        {"3.14", Tok::Double, "3.14"}, {"1e5", Tok::Double, "1e5"}, {"1E5", Tok::Double, "1E5"},
        {"1.5e-3", Tok::Double, "1.5e-3"}, {"2e+10", Tok::Double, "2e+10"}, {"1_0.2_5", Tok::Double, "10.25"},
        {"2f", Tok::Double, "2.0"}, {"2F", Tok::Double, "2.0"}, {"2d", Tok::Double, "2.0"}, {"2.5d", Tok::Double, "2.5"},
        {"2.5f", Tok::Double, "2.5"},
    };
    for (const N &n : ok) {
        const std::vector<Token> t = lex(n.src);
        reg_note_check();
        if (t.size() != 2 || t[0].kind != n.kind || t[0].text != n.text)
            reg_fail(__FILE__, __LINE__, "literal %s: got %s{%s}", n.src,
                     sleela::tokName(t[0].kind), t[0].text.c_str());
    }
    // a trailing '.' is not part of the number; 1.e5 is not a float
    REG_CHECK(kinds("7.") == "int-literal .");
    REG_CHECK(kinds("1.e5") == "int-literal . identifier");
    REG_CHECK(kinds("1..2") == "int-literal . . int-literal");
    REG_CHECK(kinds("1_") == "int-literal identifier");                // '_' needs a digit after it
    REG_CHECK(kinds("10Lx") == "int-literal identifier");              // suffix must end the token
    // malformed radix literals
    REG_CHECK(contains(lex_error("0x"), "hexadecimal literal has no digits"));
    REG_CHECK(contains(lex_error("0xG"), "hexadecimal literal has no digits"));
    REG_CHECK(contains(lex_error("0b"), "binary literal has no digits"));
    REG_CHECK(contains(lex_error("0b2"), "binary literal has no digits"));
    REG_CHECK(contains(lex_error("0o"), "octal literal has no digits"));
    REG_CHECK(contains(lex_error("0o8"), "octal literal has no digits"));
}

void t_string_literals() {
    REG_CHECK(texts("\"hello\"") == "hello");
    REG_CHECK(texts("\"\"") == "");
    REG_CHECK(texts("\"a\\nb\\tc\\r\\b\\f\\\\\\\"\"") == std::string("a\nb\tc\r\b\f\\\""));
    REG_CHECK(texts("\"\\u00e9\"") == "\xC3\xA9");                     // U+00E9 -> UTF-8
    REG_CHECK(texts("\"\\u20AC\"") == "\xE2\x82\xAC");
    REG_CHECK(texts("\"\\u0041\"") == "A");
    REG_CHECK(texts("\"\\uE000\"") == "\xEE\x80\x80");
    REG_CHECK(texts("\"caf\xC3\xA9\"") == "caf\xC3\xA9");             // raw UTF-8 passes through
    {   // \0 yields an embedded NUL, not a terminator
        const std::vector<Token> t = lex("\"a\\0b\"");
        REG_CHECK(t[0].text.size() == 3 && t[0].text[1] == '\0');
    }
    REG_CHECK(texts("\"\\q\"") == "q");                                // CHARACTERIZATION: unknown escape keeps the char
    REG_CHECK(texts("\"line\nbreak\"") == "line\nbreak");               // CHARACTERIZATION: literal may span lines
    REG_CHECK(texts("\"// not a comment\"") == "// not a comment");
    REG_CHECK(texts("\"# not a comment\"") == "# not a comment");
    REG_CHECK(texts("\"/* x */\"") == "/* x */");
    // errors
    REG_CHECK(contains(lex_error("\"abc"), "unterminated string literal"));
    REG_CHECK(contains(lex_error("\"abc\\"), "unterminated escape sequence"));
    REG_CHECK(contains(lex_error("\"\\u12\""), "invalid unicode escape"));
    REG_CHECK(contains(lex_error("\"\\u12"), "unterminated unicode escape"));
    REG_CHECK(contains(lex_error("\"\\uZZZZ\""), "invalid unicode escape"));
    REG_CHECK(contains(lex_error("\"\\uD83D\""), "surrogate"));          // lone high
    REG_CHECK(contains(lex_error("\"\\uDE00\""), "surrogate"));          // lone low
    REG_CHECK(contains(lex_error("\"\\uD83D\\uDE00\""), "surrogate"));   // pairs are not recombined
    REG_CHECK(lex_error("\"\\uD7FF\"").empty() && lex_error("\"\\uE000\"").empty());
}

void t_char_literals() {
    struct C { const char *src; const char *code; };
    const C ok[] = {
        {"'a'", "97"}, {"'A'", "65"}, {"' '", "32"}, {"'\\n'", "10"}, {"'\\t'", "9"}, {"'\\r'", "13"},
        {"'\\b'", "8"}, {"'\\f'", "12"}, {"'\\s'", "32"}, {"'\\\\'", "92"}, {"'\\''", "39"}, {"'\\\"'", "34"},
        {"'\\0'", "0"}, {"'\\7'", "7"}, {"'\\101'", "65"}, {"'\\377'", "255"}, {"'\\u0041'", "65"},
        {"'\\uuu0041'", "65"}, {"'\\uFFFF'", "65535"}, {"'\xC3\xA9'", "233"}, {"'\xE2\x82\xAC'", "8364"},
        {"'\"'", "34"},
    };
    for (const C &c : ok) {
        const std::vector<Token> t = lex(c.src);
        reg_note_check();
        if (t.size() != 2 || t[0].kind != Tok::Char || t[0].text != c.code)
            reg_fail(__FILE__, __LINE__, "char literal %s: got %s", c.src, t.empty() ? "?" : t[0].text.c_str());
    }
    REG_CHECK(contains(lex_error("''"), "empty character literal"));
    REG_CHECK(contains(lex_error("'ab'"), "exactly one character"));
    REG_CHECK(contains(lex_error("'a"), "exactly one character"));
    REG_CHECK(contains(lex_error("'"), "unterminated character literal"));
    REG_CHECK(contains(lex_error("'\n'"), "unterminated character literal"));
    REG_CHECK(contains(lex_error("'\\q'"), "invalid escape sequence"));
    REG_CHECK(contains(lex_error("'\\u00G0'"), "invalid unicode escape"));
    REG_CHECK(contains(lex_error("'\\u12'"), "invalid unicode escape"));
    REG_CHECK(contains(lex_error("'\xF0\x9F\x98\x80'"), "basic multilingual plane"));
    REG_CHECK(contains(lex_error("'\x80'"), "invalid UTF-8"));
    REG_CHECK(contains(lex_error("'\\400'"), "exactly one character"));  // octal escapes stop at 255
}

void t_comments() {
    REG_CHECK(kinds("a # to end of line\nb") == "identifier identifier");
    REG_CHECK(kinds("a // to end of line\nb") == "identifier identifier");
    REG_CHECK(kinds("a // c") == "identifier");
    REG_CHECK(kinds("#sleela 3.1\nclass") == "class");
    REG_CHECK(kinds("a\n< single line comment >\nb") == "identifier identifier");
    REG_CHECK(kinds("a\n    < indented comment >\nb") == "identifier identifier");
    REG_CHECK(kinds("a\n\t< tab indented >\nb") == "identifier identifier");
    REG_CHECK(kinds("a\n<< block >>\nb") == "identifier identifier");
    REG_CHECK(kinds("a\n<< multi\nline\nblock >>\nb") == "identifier identifier");
    REG_CHECK(kinds("a\n  << x >> b") == "identifier identifier");     // code may follow a block on its line
    REG_CHECK(kinds("<< head >>\nclass") == "class");
    // a block comment accepts ANY characters, including the ones that are
    // lexical errors in code (apostrophe, backtick, dollar)
    REG_CHECK(kinds("a\n<< don't `use` $this 'q' \"unterminated >>\nb") == "identifier identifier");
    // ... and angle brackets that are not a close
    REG_CHECK(kinds("a\n<< a > b >> c") == "identifier identifier");
    REG_CHECK(contains(lex_error("a\n<< never closed"), "unterminated angle-bracket block comment"));
    REG_CHECK(contains(lex_error("<< x > y"), "unterminated angle-bracket block comment"));
    // C-style comments are NOT comments: the body is tokenised as code
    REG_CHECK(kinds("/* c */") == "/ * identifier * /");
    REG_CHECK(kinds("a /* c */ b") == "identifier / * identifier * / identifier");
    REG_CHECK(!lex_error("/* don't */").empty());                      // the apostrophe is a lexical error
    REG_CHECK(!lex_error("/* `x` */").empty());
    // `<` mid-line is an operator, never a comment opener
    REG_CHECK(kinds("a < b > c") == "identifier < identifier > identifier");
    REG_CHECK(kinds("x; < c >") == "identifier ; < identifier >");     // inline trailing "comment" is code
    REG_CHECK(kinds("a << b") == "identifier << identifier");
    // a '<' line with no closing '>' on that line is not a comment
    REG_CHECK(kinds("a\n< no close\nb") == "identifier < identifier identifier identifier");
    REG_CHECK(!lex_error("a\n< it's open\nb").empty());                // so its apostrophe is an error
    // '#' and '//' win even inside what would be a block opener's line
    REG_CHECK(kinds("a // << not a block\nb") == "identifier identifier");
}

void t_positions() {
    const std::vector<Token> t = lex("ab cd\n  ef\r\n\tgh");
    REG_REQUIRE(t.size() == 5);
    REG_CHECK(t[0].line == 1 && t[0].col == 1);
    REG_CHECK(t[1].line == 1 && t[1].col == 4);
    REG_CHECK(t[2].line == 2 && t[2].col == 3);
    REG_CHECK(t[3].line == 3 && t[3].col == 2);                        // a tab is one column
    REG_CHECK(t[4].kind == Tok::Eof && t[4].line == 3 && t[4].col == 4);
    const std::vector<Token> e = lex("");
    REG_CHECK(e.size() == 1 && e[0].kind == Tok::Eof && e[0].line == 1 && e[0].col == 1);
    const std::vector<Token> w = lex("  \n\n  ");
    REG_CHECK(w.size() == 1 && w[0].kind == Tok::Eof && w[0].line == 3 && w[0].col == 3);
    // multi-line string literal advances the line counter
    const std::vector<Token> s = lex("\"a\nb\" x");
    REG_REQUIRE(s.size() == 3);
    REG_CHECK(s[0].line == 1 && s[1].line == 2 && s[1].col == 4);
    // positions inside error messages
    REG_CHECK(contains(lex_error("a\n  $"), "line 2, col 4"));
    REG_CHECK(contains(lex_error("a $"), "unexpected character '$'"));
    REG_CHECK(contains(lex_error("`"), "unexpected character '`'"));
    REG_CHECK(contains(lex_error("\\"), "unexpected character"));
}

void t_non_ascii_outside_literals_is_rejected() {
    REG_CHECK(!lex_error("caf\xC3\xA9").empty());                      // identifiers are ASCII only
    REG_CHECK(!lex_error("\xE2\x82\xAC").empty());
    REG_CHECK(!lex_error(std::string("a\0b", 3)).empty());              // NUL byte in source
    REG_CHECK(lex_error("# caf\xC3\xA9 comment\nx").empty());           // fine inside comments
    REG_CHECK(lex_error("// \xFF\xFE\nx").empty());
}

// ----------------------------------------------------------- generated sources

struct Piece { std::string src; Tok kind; std::string text; bool text_checked; };

std::vector<Piece> build_vocabulary() {
    std::vector<Piece> v;
    for (const Vocab &k : kKeywords) v.push_back({k.src, k.kind, k.text, true});
    for (const OpVocab &o : kOps) v.push_back({o.src, o.kind, o.src, o.text_checked});
    const char *ids[] = {"x", "foo", "Foo9", "_t", "a_b", "jdk-25", "version-mual", "gc", "mem", "ran", "Munction"};
    for (const char *i : ids) v.push_back({i, Tok::Ident, i, true});
    v.push_back({"0", Tok::Int, "0", true}); v.push_back({"123", Tok::Int, "123", true});
    v.push_back({"0xBEEF", Tok::Int, "0xBEEF", true}); v.push_back({"1_000", Tok::Int, "1000", true});
    v.push_back({"2.5", Tok::Double, "2.5", true}); v.push_back({"1e9", Tok::Double, "1e9", true});
    v.push_back({"\"s\\n\"", Tok::Str, "s\n", true}); v.push_back({"\"\"", Tok::Str, "", true});
    v.push_back({"'z'", Tok::Char, "122", true}); v.push_back({"'\\n'", Tok::Char, "10", true});
    return v;
}

// Generate random token streams with random whitespace and check kinds, text,
// line and column of every token against positions tracked by the generator.
void t_generated_streams() {
    const std::vector<Piece> vocab = build_vocabulary();
    reg_seed(0x1E8E4ull);
    for (int it = 0; it < 3000; ++it) {
        std::string src; int line = 1, col = 1; bool after_newline = true;   // a '<' at a line start would open a comment
        struct Want { Tok kind; std::string text; bool text_checked; int line, col; };
        std::vector<Want> want;
        const unsigned count = 1 + reg_below(25);
        for (unsigned i = 0; i < count; ++i) {
            // the first token on every line is a plain identifier, so '<' / '<<' is never at a line start
            const Piece use = after_newline ? Piece{"q", Tok::Ident, "q", true}
                                            : vocab[reg_below((uint32_t)vocab.size())];
            want.push_back({use.kind, use.text, use.text_checked, line, col});
            src += use.src;
            // advance expected position over the token's source text (strings/chars are single-line here)
            col += (int)use.src.size();
            after_newline = false;
            switch (reg_below(5)) {
                case 0: src += ' '; col += 1; break;
                case 1: src += "  "; col += 2; break;
                case 2: src += '\t'; col += 1; break;
                case 3: src += '\n'; line += 1; col = 1; after_newline = true; break;
                default: src += "\r\n"; line += 1; col = 1; after_newline = true; break;
            }
        }
        std::vector<Token> got;
        try { got = lex(src); }
        catch (const std::exception &e) { REG_CHECK(!"generated stream failed to lex"); reg_fail(__FILE__, __LINE__, "%s", e.what()); return; }
        REG_REQUIRE(got.size() == want.size() + 1);
        for (size_t k = 0; k < want.size(); ++k) {
            reg_note_check();
            if (got[k].kind != want[k].kind || (want[k].text_checked && got[k].text != want[k].text) ||
                got[k].line != want[k].line || got[k].col != want[k].col) {
                reg_fail(__FILE__, __LINE__, "stream mismatch at token %zu in iteration %d: kind %s/%s line %d/%d col %d/%d",
                         k, it, sleela::tokName(got[k].kind), sleela::tokName(want[k].kind),
                         got[k].line, want[k].line, got[k].col, want[k].col);
                return;
            }
        }
        REG_CHECK(got.back().kind == Tok::Eof);
    }
}

// Robustness: arbitrary bytes either lex to a well-formed stream or throw
// std::runtime_error; never crash, hang, or leave positions non-monotonic.
void t_fuzz_never_crashes() {
    reg_seed(0xF022ull);
    const char *snippets[] = {"<<", ">>", "<", ">", "'", "\"", "\\", "#", "//", "/*", "0x", "0b", "0o", "1e", ".", "::", "-", "\n", "\r", "\t"};
    int accepted = 0, rejected = 0;
    for (int it = 0; it < 40000; ++it) {
        std::string src; const unsigned n = reg_below(40);
        for (unsigned i = 0; i < n; ++i) {
            if (reg_below(3) == 0) src += snippets[reg_below(sizeof snippets / sizeof snippets[0])];
            else src += (char)reg_below(256);
        }
        try {
            const std::vector<Token> t = lex(src);
            ++accepted;
            if (t.empty() || t.back().kind != Tok::Eof) { REG_CHECK(!"stream does not end with Eof"); return; }
            int pl = 1;
            for (const Token &k : t) {
                if (k.line < pl || k.line < 1 || k.col < 1) { REG_CHECK(!"line/col not monotonic"); return; }
                pl = k.line;
            }
            for (size_t i = 0; i + 1 < t.size(); ++i)
                if (t[i].kind == Tok::Eof) { REG_CHECK(!"Eof in the middle of the stream"); return; }
        } catch (const std::runtime_error &) {
            ++rejected;
        }
    }
    REG_CHECK(accepted > 0 && rejected > 0);                          // the generator reaches both outcomes
}

void t_large_input_is_linear() {
    std::string src;
    for (int i = 0; i < 200000; ++i) src += "foo = bar + 12345; ";
    const std::vector<Token> t = lex(src);
    REG_CHECK(t.size() == 200000u * 6u + 1u);
    std::string comment = "<<";
    comment.append(4u << 20, 'x');
    comment += ">>\nend";
    REG_CHECK(kinds(comment) == "identifier");
}

}  // namespace

int main() {
    REG_RUN(t_keywords);
    REG_RUN(t_operators_longest_match);
    REG_RUN(t_comparison_token_text_is_the_full_operator);
    REG_RUN(t_identifiers_and_hyphen_rule);
    REG_RUN(t_numeric_literals);
    REG_RUN(t_string_literals);
    REG_RUN(t_char_literals);
    REG_RUN(t_comments);
    REG_RUN(t_positions);
    REG_RUN(t_non_ascii_outside_literals_is_rejected);
    REG_RUN(t_generated_streams);
    REG_RUN(t_fuzz_never_crashes);
    REG_RUN(t_large_input_is_linear);
    return reg_finish("lexer");
}
