// java_to_sleela.cpp -- see java_to_sleela.h.
//
// Structure: lex (C, java_lex.c) -> two translation passes over the tokens.
// Pass 1 collects every type's field and method signatures; pass 2 translates
// with those signatures known (so forward references and cross-type calls are
// typed). Only pass 2 diagnostics are reported.
#include "java_to_sleela.h"

extern "C" {
#include "java_lex.h"
}

#include <cctype>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>

namespace javaconv {
namespace {

struct ConvErr {
    int line, col;
    std::string construct, msg;
};

// ---------------------------------------------------------------- types ----
Ty mk(const std::string& n, int d = 0) { Ty t; t.n = n; t.dims = d; return t; }

struct E {
    std::string code;
    Ty ty;
    bool pure = true;       // no calls / allocation / assignment inside
    bool isType = false;    // a type name used as a qualifier (Math, System.out)
    bool isNull = false;
    bool isLitInt = false;  // int literal (value in iv)
    long long iv = 0;
    size_t lambdaPos = (size_t)-1;   // a lambda argument whose target type is not known until the call is resolved
    int line = 0, col = 0;
};

const std::set<std::string> kPrim = {"boolean", "byte", "short", "char", "int", "long", "float", "double"};
const std::set<std::string> kModifiers = {"public", "private", "protected", "static", "final", "abstract",
                                          "synchronized", "volatile", "transient", "native", "strictfp",
                                          "default", "sealed"};
// SLeeLa words that are legal Java identifiers but would collide in SLeeLa.
const std::set<std::string> kSleelaReserved = {"struct", "material", "spark", "int32", "record", "close", "native",
                                               "Any", "print", "enum", "interface", "super", "yield", "throw",
                                               "instanceof", "switch", "synchronized", "assert",
                                               "join", "spawn", "sleep", "arrayLength", "arrayNew", "arraySet", "structPack", "structUnpack"};

struct Tok {
    jt_kind kind = JT_EOF;
    std::string text;
    long long ival = 0;
    int line = 0, col = 0;
};

bool isIdentTok(const Tok& t) { return t.kind == JT_IDENT; }

class Translator {
public:
    Translator(const cm_model& m, const Options& o, const std::vector<Tok>& t, SymbolTable& tab, bool skipCollect)
        : M(m), opt(o), T(t), classes(tab.classes), nestedMap_(tab.nested), skipPass1(skipCollect) {}

    // JDK functional interfaces: their abstract method, with generic parameters unknown. They are
    // only used when the interface itself was not collected from source.
    void seedFunctionalInterfaces() {
        struct Fi { const char* name; const char* method; const char* ret; int nparams; };
        static const Fi table[] = {
            {"Runnable", "run", "void", 0}, {"Callable", "call", "?", 0}, {"Supplier", "get", "?", 0},
            {"Consumer", "accept", "void", 1}, {"BiConsumer", "accept", "void", 2},
            {"Function", "apply", "?", 1}, {"BiFunction", "apply", "?", 2},
            {"UnaryOperator", "apply", "?", 1}, {"BinaryOperator", "apply", "?", 2},
            {"Predicate", "test", "boolean", 1}, {"BiPredicate", "test", "boolean", 2},
            {"Comparator", "compare", "int", 2},
        };
        for (const Fi& f : table) {
            if (classes.count(f.name)) continue;
            ClassInfo& ci = classes[f.name];
            ci.isInterface = true;
            MethodSig m; m.name = f.method; m.ret = f.ret[0] == '?' ? Ty() : mk(f.ret); m.isAbstract = true;
            for (int i = 0; i < f.nparams; i++) m.params.push_back(Ty());
            ci.methods.push_back(m);
        }
    }

    Result run(bool collectOnly = false) {
        Result r;
        seedFunctionalInterfaces();
        for (pass = skipPass1 ? 2 : 1; pass <= (collectOnly ? 1 : 2); pass++) {
            p = 0;
            diags.clear();
            files.clear();
            parseUnit();
        }
        r.diagnostics = diags;
        if (diags.empty()) r.files = files;
        return r;
    }

private:
    const cm_model& M;
    Options opt;
    const std::vector<Tok>& T;
    size_t p = 0;
    int pass = 1;
    std::vector<Diagnostic> diags;
    std::vector<OutFile> files;
    std::map<std::string, ClassInfo>& classes;
    std::map<std::string, std::string>& nestedMap_;
    bool skipPass1;

    // per-type / per-method state
    std::string curClass;
    std::set<std::string> typeVars;
    bool curIsInterface = false;
    std::vector<std::map<std::string, std::pair<std::string, Ty>>> scopes;
    std::set<std::string> usedNames;
    Ty curRet;
    std::vector<std::string> out;   // lines of the member being built
    int ind = 0;
    int tmpCounter = 0;
    std::vector<char> breakCtx;      // 'L' loop, 'S' lowered switch

    // ---- anonymous classes and lambdas ---------------------------------------------------------
    // Both are lowered to a generated top-level class (javac does the same): the captured local
    // variables become constructor-initialised fields (Java requires them effectively final, so
    // copying is exact), the enclosing instance becomes the field outer_, and a lambda's class
    // implements the one abstract method of its target interface.
    struct AnonCtx {
        std::string sub, outerClass;
        bool outerStatic = false;
        bool isLambda = false;   // in a lambda `this` is the enclosing object
        std::vector<std::map<std::string, std::pair<std::string, Ty>>> outerScopes;
        std::vector<std::pair<std::string, Ty>> captures;   // emit name -> type, in first-use order
    };
    std::vector<AnonCtx> anonStack_;
    std::vector<OutFile> pendingTypes_;
    int anonCounter_ = 0;
    Ty lambdaTarget_;
    bool curIsStatic_ = false;
    bool enumMode_ = false;
    bool innerMode_ = false;        // parsing members of an inner (non-static) class: constructors take the enclosing instance
    bool innerCtorSeen_ = false;
    // Instance field initializers of the class being parsed. Java runs them in the constructor, after
    // super(...) and before the rest of the body (JLS 12.5); SLeeLa would run them at allocation, before
    // any constructor, which reorders side effects and lets a superclass constructor see them early.
    // They are therefore collected here and assigned from a per-class method that each constructor calls.
    struct ClassInit { std::vector<std::string> inits; bool ctorSeen = false; };
    ClassInit* classInit_ = nullptr;   // null inside enums, records and anonymous classes (generated, not hoisted)
    struct InitScope {
        Translator* t; ClassInit* saved;
        InitScope(Translator* t_, ClassInit* n) : t(t_), saved(t_->classInit_) { t->classInit_ = n; }
        ~InitScope() { t->classInit_ = saved; }
    };
    static constexpr const char* kInitMarker = "@@FIELDINIT@@";
    std::string curSimple_;         // the source name of the class being parsed (constructors are written with it)
    std::string resolveTypeName(const std::string& n) const {
        auto it = nestedMap_.find(n);
        if (it == nestedMap_.end()) return n;
        if (it->second == "\x01") throw ConvErr{cur().line, cur().col, "nested-ambiguous", "the simple name '" + n + "' is used by several nested types"};
        return it->second;
    }
    // ---------------------------------------------------------- tokens ----
    const Tok& cur() const { return T[p]; }
    const Tok& at(size_t i) const { return T[std::min(i, T.size() - 1)]; }
    bool isOp(const char* s) const { return cur().kind == JT_OP && cur().text == s; }
    bool isOpAt(size_t i, const char* s) const { return at(i).kind == JT_OP && at(i).text == s; }
    bool isKw(const char* s) const { return cur().kind == JT_IDENT && cur().text == s; }
    bool isKwAt(size_t i, const char* s) const { return at(i).kind == JT_IDENT && at(i).text == s; }
    bool atEof() const { return cur().kind == JT_EOF; }

    [[noreturn]] void fail(const std::string& construct, const std::string& msg) const {
        throw ConvErr{cur().line, cur().col, construct, msg};
    }
    [[noreturn]] void unsup(const std::string& key) const {
        const cm_rec* r = cm_find(&M, "unsupported", key.c_str(), nullptr, nullptr);
        throw ConvErr{cur().line, cur().col, key, r ? r->f[1] : ("construct '" + key + "' is not supported")};
    }
    void expectOp(const char* s) {
        if (!isOp(s)) fail("syntax", std::string("expected '") + s + "' but found '" + descr() + "'");
        p++;
    }
    bool acceptOp(const char* s) { if (isOp(s)) { p++; return true; } return false; }
    bool acceptKw(const char* s) { if (isKw(s)) { p++; return true; } return false; }
    void expectKw(const char* s) {
        if (!isKw(s)) fail("syntax", std::string("expected '") + s + "' but found '" + descr() + "'");
        p++;
    }
    std::string descr() const {
        const Tok& t = cur();
        if (t.kind == JT_EOF) return "end of file";
        if (!t.text.empty()) return t.text;
        return "literal";
    }
    std::string ident() {
        if (!isIdentTok(cur())) fail("syntax", "expected an identifier but found '" + descr() + "'");
        return T[p++].text;
    }

    void line(const std::string& s) { out.push_back(std::string((size_t)ind * 2, ' ') + s); }

    // ----------------------------------------------------------- model ----
    const char* tplBinop(const std::string& cls, const std::string& op) const {
        const cm_rec* r = cm_find(&M, "binop", cls.c_str(), op.c_str(), nullptr);
        return r ? r->f[2] : nullptr;
    }
    const char* tplUnop(const std::string& cls, const std::string& op) const {
        const cm_rec* r = cm_find(&M, "unop", cls.c_str(), op.c_str(), nullptr);
        return r ? r->f[2] : nullptr;
    }
    static std::string subst(std::string tpl, const std::string& a, const std::string& b) {
        std::string o;
        for (size_t i = 0; i < tpl.size(); i++) {
            if (tpl[i] == '$' && i + 1 < tpl.size() && (tpl[i + 1] == 'a' || tpl[i + 1] == 'b')) {
                o += (tpl[i + 1] == 'a') ? a : b;
                i++;
            } else o += tpl[i];
        }
        return o;
    }
    bool modelBoxed(const std::string& n, std::string* prim = nullptr) const {
        const cm_rec* r = cm_find(&M, "box", n.c_str(), nullptr, nullptr);
        if (r && prim) *prim = r->f[1];
        return r != nullptr;
    }

    // Operand class used to select binop/unop rows.
    std::string classOf(const Ty& t) const {
        if (t.dims > 0) return "reference";
        if (t.n == "?") return "unknown";
        if (t.n == "null") return "reference";
        std::string n = t.n;
        std::string prim;
        if (modelBoxed(n, &prim)) n = prim;
        if (n == "float") return "float";
        if (n == "boolean") return "boolean";
        if (n == "byte" || n == "short" || n == "char" || n == "int") return "int";
        if (n == "long") return "long";
        if (n == "double") return "double";
        if (n == "String") return "text";
        return "reference";
    }
    static int rankOf(const std::string& c) { return c == "int" ? 1 : c == "long" ? 2 : c == "float" ? 3 : c == "double" ? 4 : 0; }
    bool isPrimTy(const Ty& t) const { return t.dims == 0 && kPrim.count(t.n) > 0; }
    bool isNumericCls(const std::string& c) const { return rankOf(c) > 0; }

    // ---------------------------------------------------------- idents ----
    static std::string safeName(const std::string& n) {
        return kSleelaReserved.count(n) ? n + "_" : n;
    }
    void pushScope() { scopes.emplace_back(); }
    void popScope() { scopes.pop_back(); }
    std::string declare(const std::string& jn, const Ty& ty) {
        std::string base = safeName(jn), e = base;
        int k = 2;
        while (usedNames.count(e)) e = base + "_" + std::to_string(k++);
        usedNames.insert(e);
        scopes.back()[jn] = {e, ty};
        return e;
    }
    const std::pair<std::string, Ty>* lookupLocal(const std::string& jn) const {
        for (size_t i = scopes.size(); i-- > 0;) {
            auto it = scopes[i].find(jn);
            if (it != scopes[i].end()) return &it->second;
        }
        return nullptr;
    }
    std::string freshTmp(const char* stem) { return std::string(stem) + "_" + std::to_string(++tmpCounter); }

    // ----------------------------------------------------------- types ----
    // Returns the token index just after a type starting at i, or npos.
    size_t scanType(size_t i) const {
        const size_t npos = (size_t)-1;
        while (at(i).kind == JT_OP && at(i).text == "@") {  // annotations on types
            i++;
            if (!isIdentTok(at(i))) return npos;
            i++;
            while (isOpAt(i, ".") && isIdentTok(at(i + 1))) i += 2;
            if (isOpAt(i, "(")) {
                int d = 0;
                do { if (isOpAt(i, "(")) d++; else if (isOpAt(i, ")")) d--; i++; } while (d > 0 && at(i).kind != JT_EOF);
            }
        }
        if (!isIdentTok(at(i))) return npos;
        i++;
        for (;;) {
            if (isOpAt(i, "<")) {
                int d = 0;
                do {
                    const Tok& t = at(i);
                    if (t.kind == JT_EOF) return npos;
                    if (t.kind == JT_OP) {
                        if (t.text == "<") d++;
                        else if (t.text == ">") d--;
                        else if (t.text == ">>") d -= 2;
                        else if (t.text == ">>>") d -= 3;
                        else if (t.text != "," && t.text != "?" && t.text != "." && t.text != "[" && t.text != "]" && t.text != "&" && t.text != "@" && t.text != "(" && t.text != ")") return npos;
                    } else if (t.kind != JT_IDENT) return npos;
                    i++;
                } while (d > 0);
                if (d < 0) return npos;
            }
            if (isOpAt(i, ".") && isIdentTok(at(i + 1))) { i += 2; continue; }
            break;
        }
        while (isOpAt(i, "[") && isOpAt(i + 1, "]")) i += 2;
        return i;
    }

    Ty parseType() {
        size_t end = scanType(p);
        if (end == (size_t)-1) fail("syntax", "expected a type but found '" + descr() + "'");
        while (isOp("@")) {  // skip type annotations
            p++; p++;
            while (isOp(".")) p += 2;
            if (isOp("(")) { int d = 0; do { if (isOp("(")) d++; else if (isOp(")")) d--; p++; } while (d > 0 && !atEof()); }
        }
        std::string name = ident();
        // A type-qualified name Outer.Inner names the hoisted type Outer__Inner (package segments, which
        // start in lower case, are dropped). Resolving only the last segment would bind Format.Field to
        // the nested Field of the current file: java.text.NumberFormat.Field then extended itself.
        std::string qual;
        int depth = 0;
        for (;;) {
            if (isOp("<")) {
                int d = 0;
                do {
                    if (isOp("<")) d++; else if (isOp(">")) d--; else if (isOp(">>")) d -= 2; else if (isOp(">>>")) d -= 3;
                    p++;
                } while (d > 0 && !atEof());
            }
            if (isOp(".") && isIdentTok(at(p + 1))) {
                if (!name.empty() && std::isupper((unsigned char)name[0])) qual = qual.empty() ? resolveTypeName(name) : qual + "__" + name;
                else qual.clear();
                p++;
                name = ident();
                continue;
            }
            break;
        }
        while (isOp("[") && isOpAt(p + 1, "]")) { p += 2; depth++; }
        Ty t = mk(name, depth);
        if (qual.empty() && typeVars.count(name)) t.n = "?";
        else if (qual.empty()) t.n = resolveTypeName(name);
        else t.n = qual + "__" + name;
        // A boxed type written as a type (Integer i, (Long) o, Double[] a). The converter holds
        // boxed values as primitives, but a declaration would name the lib/java class Integer, to
        // which a primitive is not assignable, so the output would not compile. Type arguments
        // (List<Integer>) are erased and do not reach this point.
        if (qual.empty() && modelBoxed(t.n)) fail("boxed", "the boxed type " + t.n + " used as a declared or cast type is not converted yet");
        return t;
    }

    std::string emitType(const Ty& t) const {
        std::string n = t.n;
        if (n == "?") n = "Any";
        if (n == "float") n = "double";   // a binary32 value is held in a double that is rounded to 24 bits after each operation (JavaFloat)
        for (int i = 0; i < t.dims; i++) n += "[]";
        return n;
    }

    // ------------------------------------------------------- literals ----
    static std::string quote(const std::string& s, const Tok& where, bool* bad) {
        std::string o = "\"";
        for (unsigned char c : s) {
            switch (c) {
                case '"': o += "\\\""; break;
                case '\\': o += "\\\\"; break;
                case '\n': o += "\\n"; break;
                case '\t': o += "\\t"; break;
                case '\r': o += "\\r"; break;
                default:
                    if (c < 0x20) { *bad = true; (void)where; }
                    o += (char)c;
            }
        }
        return o + "\"";
    }

    // ----------------------------------------------------- expressions ----
    E lit(const std::string& code, const Ty& ty) { E e; e.code = code; e.ty = ty; return e; }

    E coerce(const E& v, const Ty& target) {
        // Implicit widening on assignment/return: int/long -> double must become a
        // real double (otherwise a later '/' would divide as integers).
        if (target.dims == 0 && target.n == "float" && v.ty.dims == 0) {
            std::string c = classOf(v.ty);
            if (c == "int" || c == "long") {   // int/long -> float rounds to 24 bits (JLS 5.1.2)
                E r = v;
                r.code = "JavaFloat.f32((double) " + paren(v.code) + ")";
                r.ty = mk("float");
                r.isLitInt = false;
                return r;
            }
        }
        if (target.dims == 0 && target.n == "double" && v.ty.dims == 0) {
            std::string c = classOf(v.ty);
            if (c == "int" || c == "long") {
                E r = v;
                r.code = v.isLitInt ? std::to_string(v.iv) + ".0" : "(double) " + paren(v.code);
                r.ty = mk("double");
                r.isLitInt = false;
                return r;
            }
        }
        return v;
    }
    static std::string paren(const std::string& c) {
        bool simple = true;
        for (char ch : c) if (!(isalnum((unsigned char)ch) || ch == '_' || ch == '.' || ch == '$')) { simple = false; break; }
        if (simple) return c;
        if (!c.empty() && c.front() == '(' && c.back() == ')') {
            int d = 0; bool whole = true;
            for (size_t i = 0; i < c.size(); i++) { if (c[i] == '(') d++; else if (c[i] == ')') { d--; if (d == 0 && i + 1 < c.size()) { whole = false; break; } } }
            if (whole) return c;
        }
        return "(" + c + ")";
    }
    E toClass(const E& v, const std::string& cls) {
        // bring an operand to the operation class (int/long -> double only)
        std::string c = classOf(v.ty);
        if (cls == "double" && (c == "int" || c == "long")) return coerce(v, mk("double"));
        if (cls == "float" && (c == "int" || c == "long")) return coerce(v, mk("float"));
        return v;
    }

    // Java string conversion of an operand (JLS 5.1.11) when SLeeLa renders it differently.
    // Does the class (or a superclass / interface of it) declare a no-argument toString()?
    // The class that declares toString() for cls (itself or a superclass), or "".
    std::string toStringOwner(const std::string& cls, int depth = 0) const {
        auto it = classes.find(cls);
        if (it == classes.end() || depth > 32) return "";
        if (it->second.isInterface) return "";
        if (it->second.isEnum) return cls;   // generated
        for (const auto& m : it->second.methods) if (m.name == "toString" && m.params.empty()) return cls;
        if (!it->second.super.empty()) return toStringOwner(it->second.super, depth + 1);
        return "";
    }
    bool hasToString(const std::string& cls) const { return !toStringOwner(cls).empty(); }

    std::string strOf(const E& x) const {
        // JLS 5.1.11: an object becomes text by its toString(); null becomes "null". SLeeLa would show
        // the handle (Name#3), so the call is made explicitly for classes whose toString is known.
        if (x.ty.dims == 0 && !kPrim.count(x.ty.n) && x.ty.n != "String" && x.ty.n != "?" && x.ty.n != "null" && !x.isType && hasToString(x.ty.n)) {
            // the owner class carries a static helper javaText(x): null is "null", else x.toString()
            return toStringOwner(x.ty.n) + ".javaText(" + x.code + ")";
        }
        if (x.ty.dims == 0 && kPrim.count(x.ty.n)) {
            if (x.ty.n == "float") return "JavaFloat.text(" + x.code + ")";
            const cm_rec* r = cm_find(&M, "tostr", x.ty.n.c_str(), nullptr, nullptr);
            if (r) return subst(r->f[1], x.code, "");
        }
        // Any other single object of a KNOWN reference type (a library type, Object, an interface):
        // its toString() at run time, through the model row. Boxed values are held as primitives and
        // already show as Java does, so they keep their own text. A value of unknown type ("?", an
        // erased generic or an untyped call result such as e.getSuppressed().length) may be a
        // primitive at run time, so it keeps the plain SLeeLa text.
        if (x.ty.dims == 0 && !kPrim.count(x.ty.n) && x.ty.n != "String" && x.ty.n != "null" && x.ty.n != "?" && !x.isType && !modelBoxed(x.ty.n)) {
            auto ci = classes.find(x.ty.n);
            if (ci != classes.end() && !ci->second.isInterface && !ci->second.isEnum) {
                // a class of this tree without toString(): Java shows Name@hash unless a library
                // ancestor supplies toString()
                bool libraryAncestor = false;
                int hops = 0;
                for (std::string c = ci->second.super; !c.empty() && ++hops <= 32;) {
                    auto up = classes.find(c);
                    if (up == classes.end()) { libraryAncestor = true; break; }
                    if (up->second.super == c) break;
                    c = up->second.super;
                }
                if (!libraryAncestor) fail("object-text", "an object of " + x.ty.n + ", which has no toString(), shown as text: Java prints an identity hash");
            }
            if (const cm_rec* r = cm_find(&M, "tostr", "reference", nullptr, nullptr)) return subst(r->f[1], x.code, "");
        }
        return x.code;
    }

    E emitBinary(const std::string& op, const E& a, const E& b) {
        E r;
        r.pure = a.pure && b.pure;
        bool isShift = (op == "<<" || op == ">>" || op == ">>>");
        bool isCmp = (op == "<" || op == "<=" || op == ">" || op == ">=" || op == "==" || op == "!=");
        bool isLogic = (op == "&&" || op == "||");

        // string concatenation
        auto isStr = [&](const E& x) { return x.ty.dims == 0 && x.ty.n == "String"; };
        if (op == "+" && (isStr(a) || isStr(b))) {
            r.code = "(" + strOf(a) + " + " + strOf(b) + ")";
            r.ty = mk("String");
            return r;
        }

        // reference / text identity comparison
        if (op == "==" || op == "!=") {
            if (a.isNull || b.isNull) {
                r.code = "(" + a.code + " " + op + " " + b.code + ")";
                r.ty = mk("boolean");
                return r;
            }
        }
        std::string ca = classOf(a.ty), cb = classOf(b.ty);
        if (ca == "unknown" || cb == "unknown") {
            // both sides unknown is fine only for logical ops on already-boolean subexpressions
            if (!isLogic) unsup("untyped");
        }
        if ((op == "==" || op == "!=") && (ca == "text" || cb == "text")) {
            fail("string-identity", "'==' on Strings compares identity in Java but value in SLeeLa; compare with equals() or against null");
        }
        if ((op == "==" || op == "!=") && modelBoxed(a.ty.n) && modelBoxed(b.ty.n) && a.ty.dims == 0 && b.ty.dims == 0)
            fail("boxed-identity", "'==' between two boxed values compares identity in Java (value cache behaviour); unbox one side");

        std::string cls;
        if (isLogic) cls = "boolean";
        else if (isShift) {
            if (!isNumericCls(ca) || !isNumericCls(cb) || ca == "double" || cb == "double") unsup("untyped");
            cls = ca;
        } else if (isNumericCls(ca) && isNumericCls(cb)) {
            cls = rankOf(ca) >= rankOf(cb) ? ca : cb;
        } else if (ca == cb) {
            cls = ca;
        } else if (ca == "reference" || cb == "reference") {
            cls = "reference";
        } else {
            fail("model", "no operand class for " + ca + " " + op + " " + cb);
        }
        if (cls == "float") {
            // binary32 arithmetic (JLS 15.17-15.20): do the operation in double, then round to 24 bits.
            // For + - * / and %, rounding the double result equals the correctly rounded binary32
            // result because a double has more than 2*24+2 significant bits.
            E fx = toClass(a, "float"), fy = toClass(b, "float");
            if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
                r.code = "JavaFloat.f32((" + fx.code + " " + op + " " + fy.code + "))";
                r.ty = mk("float");
                return r;
            }
            if (isCmp) { r.code = "(" + fx.code + " " + op + " " + fy.code + ")"; r.ty = mk("boolean"); return r; }
            fail("model", "no binary32 rule for operator '" + op + "'");
        }
        const char* tpl = tplBinop(cls, op);
        if (!tpl) fail("model", "the conversion model has no rule for '" + op + "' on operand class '" + cls + "'");
        E x = isShift ? a : toClass(a, cls);
        E y = isShift ? b : toClass(b, cls);
        r.code = subst(tpl, x.code, y.code);
        if (isCmp || isLogic) r.ty = mk("boolean");
        else if (cls == "int") r.ty = mk("int");
        else if (cls == "long" || cls == "double" || cls == "boolean") r.ty = mk(cls);
        else r.ty = mk("?");
        return r;
    }

    E emitUnary(const std::string& op, const E& a) {
        E r;
        r.pure = a.pure;
        std::string c = classOf(a.ty);
        const bool isF = (c == "float");
        if (isF) c = "double";
        if (c == "unknown") unsup("untyped");
        const char* tpl = tplUnop(c, op);
        if (!tpl) fail("model", "the conversion model has no rule for unary '" + op + "' on operand class '" + c + "'");
        r.code = subst(tpl, a.code, "");
        r.ty = isF ? mk("float") : (c == "boolean") ? mk("boolean") : mk(c);
        return r;
    }

    E emitCast(const Ty& to, const E& a) {
        E r;
        r.pure = a.pure;
        r.ty = to;
        if (to.dims == 0 && kPrim.count(to.n)) {
            if (to.n == "boolean") { r.code = a.code; return r; }
            std::string from = classOf(a.ty);
            if (from == "unknown") unsup("untyped");
            if (to.n == "float") {   // (float): round to binary32
                if (from == "float") { r.code = a.code; return r; }
                r.code = (from == "double") ? "JavaFloat.f32(" + a.code + ")" : "JavaFloat.f32((double) " + paren(a.code) + ")";
                return r;
            }
            if (from == "float") from = "double";   // a float is held as a double: the double conversion rows apply
            const cm_rec* c = cm_find(&M, "cast", from.c_str(), to.n.c_str(), nullptr);
            if (!c) fail("model", "the conversion model has no cast from '" + from + "' to '" + to.n + "'");
            if (std::string(c->f[2]) == "=") {
                r.code = a.code;
                // Syntax 3.1 shows a value of static type char as text in a String context; a Java
                // (int)/(long) cast of a char is a number, so it must drop the char type explicitly.
                if (a.ty.dims == 0 && a.ty.n == "char" && (to.n == "int" || to.n == "long")) {
                    r.code = "(" + to.n + ") " + paren(a.code);
                    r.isLitInt = false;
                    return r;
                }
                // a plain int->int/long cast of a literal stays a literal
                r.isLitInt = a.isLitInt && to.n == "int";
                r.iv = a.iv;
            } else r.code = subst(c->f[2], paren(a.code), "");
            return r;
        }
        // reference cast; boxing casts like (Integer) x are not modelled
        std::string prim;
        if (a.ty.dims == 0 && isPrimTy(a.ty) && to.dims == 0) fail("model", "boxing cast (" + to.n + ") of a primitive is not modelled");
        r.code = "(" + emitType(to) + ") " + paren(a.code);
        return r;
    }

    // Are the tokens at p a cast "(Type) operand" ?
    bool looksLikeCast() const {
        if (!isOp("(")) return false;
        size_t e = scanType(p + 1);
        if (e == (size_t)-1 || !isOpAt(e, ")")) {
            // intersection casts and the like are not recognised
            return false;
        }
        const Tok& first = at(p + 1);
        bool prim = first.kind == JT_IDENT && kPrim.count(first.text) && e == p + 2 + 0 + 0 ? true : (first.kind == JT_IDENT && kPrim.count(first.text));
        const Tok& nx = at(e + 1);
        if (prim) return true;  // (int) -x is a cast
        // reference cast: next token must start an operand that is not binary +/-
        if (nx.kind == JT_IDENT) {
            return !(nx.text == "instanceof");
        }
        if (nx.kind == JT_STRING || nx.kind == JT_INT || nx.kind == JT_LONG || nx.kind == JT_DOUBLE || nx.kind == JT_CHAR || nx.kind == JT_TEXTBLOCK || nx.kind == JT_FLOAT) return true;
        if (nx.kind == JT_OP && (nx.text == "(" || nx.text == "!" || nx.text == "~")) return true;
        return false;
    }

    // an argument that is a method reference: Name::name  expr.Name::name  Type::new
    bool isMethodRefAhead() const {
        int depth = 0;
        for (size_t i = p; at(i).kind != JT_EOF; i++) {
            if (isOpAt(i, "(") || isOpAt(i, "[") || isOpAt(i, "{")) depth++;
            else if (isOpAt(i, ")") || isOpAt(i, "]") || isOpAt(i, "}")) { if (depth == 0) return false; depth--; }
            else if (depth == 0 && (isOpAt(i, ",") || isOpAt(i, ";") || isOpAt(i, "?") || isOpAt(i, "->"))) return false;
            else if (depth == 0 && isOpAt(i, "::")) return true;
            else if (depth == 0 && at(i).kind == JT_OP && at(i).text != "." && at(i).text != "<" && at(i).text != ">") return false;
        }
        return false;
    }
    bool isLambdaAhead() const {
        if (isOp("(")) {
            int d = 0; size_t i = p;
            do { if (isOpAt(i, "(")) d++; else if (isOpAt(i, ")")) d--; i++; } while (d > 0 && at(i).kind != JT_EOF);
            return isOpAt(i, "->");
        }
        return isIdentTok(cur()) && isOpAt(p + 1, "->");
    }

    // Syntax 3.1 array initializer {a, b, c} for a one-dimensional array type.
    std::string parseArrayInit(const Ty& arrayTy) {
        if (arrayTy.dims < 1) fail("model", "array initializer needs an array type");
        expectOp("{");
        std::string code = "{";
        bool first = true;
        while (!isOp("}")) {
            if (atEof()) fail("syntax", "unterminated array initializer");
            std::string el;
            if (isOp("{")) {
                if (arrayTy.dims < 2) fail("syntax", "nested array initializer inside a one-dimensional array");
                el = parseArrayInit(mk(arrayTy.n, arrayTy.dims - 1));
            } else {
                if (arrayTy.dims > 1) fail("syntax", "array of arrays needs a nested initializer for each element");
                
                el = coerce(parseExpr(), mk(arrayTy.n)).code;
            }
            code += (first ? "" : ", ") + el;
            first = false;
            if (!acceptOp(",")) break;
        }
        expectOp("}");
        return code + "}";
    }

    // The outermost expression of a statement may start with a switch expression, which is hoisted
    // into statements before the statement that uses it (see parseSwitchExpr).
    size_t hoistPos_ = (size_t)-1;
    int exprDepth_ = 0;
    bool allowHoist_ = false;
    Ty hoistTy_;

    E parseExpr() {
        if (exprDepth_ == 0) hoistPos_ = p;
        struct Depth { int& d; Depth(int& x) : d(x) { ++d; } ~Depth() { --d; } } depth(exprDepth_);
        E e = parseTernary();
        if (isAssignOp(cur())) {
            // assignment used as a value (JLS 15.26): the value is the assigned value
            std::string op = cur().text;
            p++;
            
            if (!isLvalueCode(e)) fail("syntax", "the left side of an assignment must be a variable, field or array element");
            E rhs = parseExpr();
            std::string text = assignText(e, op, rhs);
            E r;
            r.code = "(" + text + ")";
            r.ty = e.ty;
            r.pure = false;
            return r;
        }
        return e;
    }
    bool isLvalueCode(const E& e) const {
        if (e.isType || e.isNull || e.code.empty()) return false;
        char c = e.code.back();
        return !(c == ')' && e.code.find('(') != std::string::npos && e.code.rfind(").") == std::string::npos && false) && (isalnum((unsigned char)c) || c == '_' || c == ']' );
    }

    E parseTernary() {
        E c = parseBinary(1);
        if (!isOp("?")) return c;
        p++;
        
        E a = parseTernary();
        expectOp(":");
        E b = parseTernary();
        E r;
        r.pure = c.pure && a.pure && b.pure;
        std::string ca = classOf(a.ty), cb = classOf(b.ty);
        if (a.ty == b.ty) r.ty = a.ty;
        else if (a.isNull) r.ty = b.ty;
        else if (b.isNull) r.ty = a.ty;
        else if (isNumericCls(ca) && isNumericCls(cb)) {
            std::string cls = rankOf(ca) >= rankOf(cb) ? ca : cb;
            a = toClass(a, cls); b = toClass(b, cls);
            r.ty = mk(cls);
        } else r.ty = mk("?");
        r.code = "(" + c.code + " ? " + a.code + " : " + b.code + ")";
        return r;
    }

    static int precOf(const std::string& op) {
        if (op == "||") return 1;
        if (op == "&&") return 2;
        if (op == "|") return 3;
        if (op == "^") return 4;
        if (op == "&") return 5;
        if (op == "==" || op == "!=") return 6;
        if (op == "<" || op == ">" || op == "<=" || op == ">=") return 7;
        if (op == "<<" || op == ">>" || op == ">>>") return 8;
        if (op == "+" || op == "-") return 9;
        if (op == "*" || op == "/" || op == "%") return 10;
        return -1;
    }

    E parseBinary(int minPrec) {
        E left = parseUnary();
        for (;;) {
            if (isKw("instanceof")) {
                if (7 < minPrec) break;
                p++;
                acceptKw("final");
                Ty t = parseType();
                if (isIdentTok(cur()) && cur().text != "instanceof" && !isKw("when")) {
                    // pattern binding `x instanceof T v` (JLS 14.30.1): v is declared before the statement and
                    // assigned when the test succeeds; a non-null match makes the assignment test true
                    std::string vname = ident();
                    std::string en = declare(vname, t);
                    line(emitType(t) + " " + en + ";");
                    std::string subject = left.code;
                    std::string test;
                    if (left.pure) {
                        test = "(" + left.code + " instanceof " + emitType(t) + ")";
                    } else {
                        std::string tmp = freshTmp("pat");
                        usedNames.insert(tmp);
                        line("Any " + tmp + ";");
                        test = "((" + tmp + " = " + left.code + ") instanceof " + emitType(t) + ")";
                        subject = tmp;
                    }
                    E r;
                    r.pure = false;
                    r.code = "(" + test + " && ((" + en + " = (" + emitType(t) + ") " + subject + ") != null))";
                    r.ty = mk("boolean");
                    left = r;
                    continue;
                }
                E r;
                r.pure = left.pure;
                r.code = "(" + left.code + " instanceof " + emitType(t) + ")";
                r.ty = mk("boolean");
                left = r;
                continue;
            }
            if (cur().kind != JT_OP) break;
            int pr = precOf(cur().text);
            if (pr < 0 || pr < minPrec) break;
            std::string op = cur().text;
            p++;
            
            E right = parseBinary(pr + 1);
            left = emitBinary(op, left, right);
        }
        return left;
    }

    E parseUnary() {
        if (cur().kind == JT_OP) {
            const std::string& o = cur().text;
            if (o == "++" || o == "--") {   // prefix ++/-- used as a value: the new value
                bool inc = (o == "++");
                p++;
                E lv = parseUnary();
                std::string text = incrText(lv, inc);
                E r;
                r.code = "(" + text + ")";
                r.ty = lv.ty;
                r.pure = false;
                return r;
            }
            if (o == "-" || o == "+") {
                std::string op = o;
                // negative literals: keep -2147483648 / -9223372036854775808 exact
                const Tok& nx = at(p + 1);
                if (op == "-" && nx.kind == JT_INT) {
                    p += 2;
                    if (isOp(".") || isOp("[")) { /* not reachable for literals */ }
                    E r = lit(std::to_string(-nx.ival), mk("int"));
                    r.isLitInt = true; r.iv = -nx.ival;
                    return r;
                }
                if (op == "-" && nx.kind == JT_LONG) {
                    p += 2;
                    long long v = nx.ival;
                    if ((unsigned long long)v == 9223372036854775808ULL) return lit("(-9223372036854775807 - 1)", mk("long"));
                    return lit(std::to_string(-v), mk("long"));
                }
                if (op == "-" && nx.kind == JT_DOUBLE) {
                    p += 2;
                    return lit("-" + std::string(nx.text), mk("double"));
                }
                p++;
                E a = parseUnary();
                return emitUnary(op, a);
            }
            if (o == "!" || o == "~") {
                std::string op = o;
                p++;
                E a = parseUnary();
                return emitUnary(op, a);
            }
            if (o == "(" ) {
                if (isLambdaAhead()) return parseLambda();
                
                if (looksLikeCast()) {
                    p++;
                    Ty t = parseType();
                    expectOp(")");
                    E a = parseUnary();
                    return emitCast(t, a);
                }
            }
        }
        return parsePostfix();
    }

    E parsePostfix() {
        E e = parseChain();
        if (isOp("++") || isOp("--")) {   // postfix ++/-- used as a value: the OLD value
            bool inc = isOp("++");
            p++;
            std::string c = classOf(e.ty);
            if (c == "double" || c == "float") fail("model", "postfix ++/-- of a floating point value as a value cannot recover the old value exactly");
            std::string newText = incrText(e, inc);
            // old value = inverse step applied to the stored new value, narrowed back to the variable type
            const cm_rec* r = cm_find(&M, "incr", c.c_str(), nullptr, nullptr);
            std::string both = r->f[1];
            size_t sep = both.find(" ;; ");
            std::string up = both.substr(0, sep), down = sep == std::string::npos ? both : both.substr(sep + 4);
            std::string oldCode = subst(inc ? down : up, "(" + newText + ")", "");
            if (e.ty.dims == 0 && (e.ty.n == "byte" || e.ty.n == "short" || e.ty.n == "char")) {
                E tmp; tmp.code = oldCode; tmp.ty = mk("int");
                oldCode = emitCast(e.ty, tmp).code;
            }
            E res;
            res.code = oldCode;
            res.ty = e.ty;
            res.pure = false;
            return res;
        }
        return e;
    }

    std::vector<E> parseArgs() {
        std::vector<E> a;
        expectOp("(");
        if (!isOp(")")) {
            do {
                if (isLambdaAhead() || isMethodRefAhead()) {
                    // the target type comes from the callee's parameter: remember where the lambda is
                    E d; d.lambdaPos = p; d.line = cur().line; d.col = cur().col; d.pure = false;
                    int depth = 0;
                    while (!atEof()) {
                        if (isOp("(") || isOp("[") || isOp("{")) depth++;
                        else if (isOp(")") || isOp("]") || isOp("}")) { if (depth == 0) break; depth--; }
                        else if (isOp(",") && depth == 0) break;
                        p++;
                    }
                    a.push_back(d);
                    continue;
                }
                a.push_back(parseExpr());
            } while (acceptOp(","));
        }
        expectOp(")");
        return a;
    }
    static std::string joinArgs(const std::vector<E>& a) {
        std::string s;
        for (size_t i = 0; i < a.size(); i++) {
            if (a[i].lambdaPos != (size_t)-1) throw ConvErr{a[i].line, a[i].col, "lambda-target", "the parameter type a lambda argument converts to is not known (the called method's signature was not collected)"};
            if (i) s += ", ";
            s += a[i].code;
        }
        return s;
    }
    static bool anyImpure(const std::vector<E>& a) {
        for (auto& x : a) if (!x.pure) return true;
        return false;
    }

    const MethodSig* pickMethod(const std::string& cls, const std::string& name, const std::vector<E>& args, int depth = 0) const {
        auto it = classes.find(cls);
        if (it == classes.end() || depth > 32) return nullptr;
        const MethodSig* best = nullptr;
        int bestScore = -1;
        for (const auto& m : it->second.methods) {
            if (m.name != name) continue;
            const bool exact = m.params.size() == args.size();
            if (!exact && !(m.varargs && args.size() + 1 >= m.params.size())) continue;
            int score = exact && !m.varargs ? 100 : 0;
            for (size_t i = 0; i < args.size() && i < m.params.size(); i++) if (m.params[i] == args[i].ty) score++;
            if (score > bestScore) { bestScore = score; best = &m; }
        }
        if (best) return best;
        if (name == "<init>") return nullptr;   // constructors are not inherited
        if (!it->second.super.empty()) if (const MethodSig* s = pickMethod(it->second.super, name, args, depth + 1)) return s;
        for (const auto& i : it->second.ifaces) if (const MethodSig* s = pickMethod(i, name, args, depth + 1)) return s;
        return nullptr;
    }

    // Java method invocation conversion (JLS 5.3, 15.12.4.2) for a resolved target: pack the trailing
    // arguments of a variable-arity call into an array, and widen int/long arguments to double
    // parameters (an int left in a double parameter would later divide as integers).
    std::vector<E> adaptArgs(const MethodSig* m, std::vector<E> args) {
        if (!m) return args;
        for (size_t i = 0; i < args.size() && i < m->params.size(); i++) {
            if (args[i].lambdaPos == (size_t)-1) continue;
            size_t save = p;
            p = args[i].lambdaPos;
            lambdaTarget_ = m->params[i];
            E lam = isLambdaAhead() ? parseLambda() : parseExpr();
            p = save;
            args[i] = lam;
        }
        const size_t n = m->params.size();
        size_t fixed = n;
        if (m->varargs) {
            fixed = n - 1;
            const Ty& vt = m->params.back();
            const bool direct = args.size() == n && (args.back().isNull || (args.back().ty.dims == vt.dims && args.back().ty.n == vt.n));
            if (!direct) {
                Ty et = mk(vt.n, vt.dims - 1);
                std::string body;
                bool imp = false;
                for (size_t i = fixed; i < args.size(); i++) {
                    E a = vt.dims - 1 == 0 ? coerce(args[i], et) : args[i];
                    body += (i > fixed ? ", " : "") + a.code;
                    imp = imp || !a.pure;
                }
                std::string dimsText;
                for (int i = 0; i < vt.dims; i++) dimsText += "[]";
                E packed;
                std::string tail;
                for (int i = 1; i < vt.dims; i++) tail += "[]";
                if (args.size() == fixed) packed.code = "new " + vt.n + "[0]" + tail;
                else packed.code = "new " + vt.n + dimsText + "{" + body + "}";
                packed.ty = vt;
                packed.pure = false;
                (void)imp;
                args.resize(fixed);
                args.push_back(packed);
            }
        }
        for (size_t i = 0; i < fixed && i < args.size(); i++) args[i] = coerce(args[i], m->params[i]);
        return args;
    }

    const FieldInfo* findField(const std::string& cls, const std::string& name, int depth = 0) const {
        auto it = classes.find(cls);
        if (it == classes.end() || depth > 32) return nullptr;
        auto f = it->second.fields.find(name);
        if (f != it->second.fields.end()) return &f->second;
        if (!it->second.super.empty()) if (const FieldInfo* s = findField(it->second.super, name, depth + 1)) return s;
        for (const auto& i : it->second.ifaces) if (const FieldInfo* s = findField(i, name, depth + 1)) return s;
        return nullptr;
    }

    // A method of java.lang.Object (equals, hashCode, getClass ...) called on a type of this tree that
    // neither declares it nor has a library ancestor: SLeeLa has no such method, so the converted call
    // would only fail later in the checker. Refuse it here, located, instead. Used for receiver calls
    // (a.equals(b)) and for unqualified calls inside the class (getClass()).
    void refuseObjectMethod(const std::string& recvType, const std::string& name) const {
        static const std::set<std::string> kObjectMethods = {"equals", "hashCode", "getClass", "clone", "notify", "notifyAll", "wait", "finalize"};
        if (!kObjectMethods.count(name) || !classes.count(recvType)) return;
        int hops = 0;
        for (std::string c = recvType; !c.empty();) {
            auto ci = classes.find(c);
            // an unknown type, or a cyclic / over-deep chain, may be a library type: do not refuse
            if (ci == classes.end() || ++hops > 32 || ci->second.super == c) return;
            c = ci->second.super;
        }
        fail("object-method", "java.lang.Object." + name + " on " + recvType + ", which does not declare it, has no SLeeLa equivalent yet");
    }

    E callResult(const std::string& recvType, const std::string& name, const std::vector<E>& args, std::string code, bool staticCall) {
        E r;
        r.code = code;
        r.pure = false;
        (void)staticCall;
        if (const MethodSig* m = pickMethod(recvType, name, args)) {
            r.ty = m->ret;
            return r;
        }
        refuseObjectMethod(recvType, name);
        std::string owner = recvType;
        std::string prim;
        if (modelBoxed(owner, &prim) && owner != "Integer" && owner != "Long" && owner != "Double") { /* keep */ }
        const cm_rec* rec = cm_find(&M, "ret", owner.c_str(), name.c_str(), nullptr);
        // A user type that extends a library type (class MyEx extends Exception) inherits that
        // type's members: walk the declared superclass chain through the model rows.
        int depth = 0;
        // the depth bound guards a cyclic chain (a nested Field extends Format.Field resolves to itself)
        for (std::string up = owner; !rec; ) {
            if (++depth > 32) break;
            auto ci = classes.find(up);
            if (ci == classes.end() || ci->second.super.empty() || ci->second.super == up) break;
            up = ci->second.super;
            if (const MethodSig* m = pickMethod(up, name, args)) { r.ty = m->ret; return r; }
            rec = cm_find(&M, "ret", up.c_str(), name.c_str(), nullptr);
        }
        if (rec) {
            std::string t = rec->f[2];
            if (t == "@arg0") {
                if (args.empty()) { r.ty = mk("?"); return r; }
                std::string c = classOf(args[0].ty);
                r.ty = (c == "int" || c == "long" || c == "double" || c == "float") ? mk(c) : mk("?");
            } else r.ty = mk(t);
        } else r.ty = mk("?");
        return r;
    }

    E parseChain() {
        E e = parsePrimary();
        for (;;) {
            if (isOp(".")) {
                p++;
                if (isOp("<")) {  // explicit generic method call: skip type args
                    int d = 0;
                    do { if (isOp("<")) d++; else if (isOp(">")) d--; else if (isOp(">>")) d -= 2; p++; } while (d > 0 && !atEof());
                }
                if (isKw("new")) unsup("nested");
                if (isKw("this") && e.isType && !anonStack_.empty() && e.code == anonStack_.back().outerClass) {
                    p++;
                    if (anonStack_.back().outerStatic) fail("outer-instance", "Outer.this from a static context");
                    E r; r.code = "outer_"; r.ty = mk(e.code); e = r;
                    continue;
                }
                if (isKw("this") || isKw("class")) unsup("nested");
                std::string name = ident();
                if (isOp("(")) {
                    std::vector<E> args = parseArgs();
                    bool imp = anyImpure(args) || !e.pure;
                    (void)imp;
                    if (e.isType) {
                        std::string full = e.code + "." + name;
                        const cm_rec* c = cm_find(&M, "call", full.c_str(), nullptr, nullptr);
                        if (c) {
                            std::string a;
                            if (full == "System.out.println") {
                                if (args.size() > 1) fail("model", "println takes at most one argument");
                                a = args.empty() ? "\"\"" : strOf(args[0]);
                            } else a = joinArgs(args);
                            std::string code = c->f[1];
                            size_t k = code.find("$args");
                            if (k != std::string::npos) code.replace(k, 5, a);
                            E r;
                            r.code = code; r.pure = false; r.ty = mk("void");
                            e = r;
                            continue;
                        }
                        std::string owner = e.code;
                        size_t dot = owner.rfind('.');
                        if (dot != std::string::npos) owner = owner.substr(dot + 1);
                        args = adaptArgs(pickMethod(owner, name, args), args);
                        E r = callResult(owner, name, args, e.code + "." + mapMember(name) + "(" + joinArgs(args) + ")", true);
                        e = r;
                        continue;
                    }
                    std::string recvType = e.ty.dims == 0 ? e.ty.n : std::string("array");
                    args = adaptArgs(pickMethod(recvType, name, args), args);
                    // model 9c: a library parameter that takes a char as text
                    for (size_t ai = 0; ai < args.size(); ai++) {
                        const std::string idx = std::to_string(ai);
                        if (args[ai].ty.dims == 0 && args[ai].ty.n == "char" && cm_find(&M, "chartext", recvType.c_str(), name.c_str(), idx.c_str())) {
                            args[ai].code = strOf(args[ai]);
                            args[ai].ty = mk("String");
                        }
                    }
                    E r = callResult(recvType, name, args, paren(e.code) + "." + mapMember(name) + "(" + joinArgs(args) + ")", false);
                    e = r;
                    continue;
                }
                // field access
                E r;
                if (e.isType && nestedMap_.count(name)) {   // Outer.Inner used as a qualifier
                    std::string tn = resolveTypeName(name);
                    r.code = tn; r.isType = true; r.ty = mk("type:" + tn);
                    e = r;
                    continue;
                }
                if (e.isType) {
                    const cm_rec* f = cm_find(&M, "field", e.code.c_str(), name.c_str(), nullptr);
                    r.pure = true;
                    if (f) {
                        r.code = e.code + "." + name;
                        r.ty = mk(f->f[2]);
                    } else {
                        // a nested qualifier (System.out) or a static field of an unmodelled type
                        r.isType = !name.empty() && (isupper((unsigned char)name[0]) == 0) && e.code.find('.') == std::string::npos && e.code == "System";
                        r.code = e.code + "." + name;
                        r.ty = mk("?");
                        if (const FieldInfo* fi = findField(e.code, name)) r.ty = fi->ty;
                    }
                } else if (e.ty.dims > 0 && name == "length") {
                    const cm_rec* al = cm_find(&M, "call", "array.length", nullptr, nullptr);
                    if (!al) fail("model", "the conversion model has no array.length rule");
                    std::string code = al->f[1];
                    size_t k = code.find("$recv");
                    if (k != std::string::npos) code.replace(k, 5, e.code);
                    r.code = code;
                    r.ty = mk("int");
                    r.pure = e.pure;
                } else {
                    r.code = paren(e.code) + "." + name;
                    r.pure = e.pure;
                    r.ty = mk("?");
                    if (e.ty.dims == 0) if (const FieldInfo* fi = findField(e.ty.n, name)) r.ty = fi->ty;
                }
                e = r;
                continue;
            }
            if (isOp("[")) {
                p++;
                E idx = parseExpr();
                expectOp("]");
                std::string ic = classOf(idx.ty);
                if (ic == "long" || ic == "double") fail("model", "array index must be int");
                E r;
                r.pure = e.pure && idx.pure;
                r.code = paren(e.code) + "[" + idx.code + "]";
                r.ty = e.ty.dims > 0 ? mk(e.ty.n, e.ty.dims - 1) : mk("?");
                e = r;
                continue;
            }
            if (isOp("::")) { e = buildMethodRef(e); continue; }
            break;
        }
        return e;
    }

    E parsePrimary() {
        if (isLambdaAhead()) return parseLambda();
        const Tok& t = cur();
        switch (t.kind) {
            case JT_INT: {
                if (t.ival > 2147483647LL) fail("syntax", "int literal 2147483648 is only valid under unary minus");
                E r = lit(std::to_string(t.ival), mk("int"));
                r.isLitInt = true; r.iv = t.ival;
                p++;
                return r;
            }
            case JT_LONG: {
                E r = lit(std::to_string(t.ival), mk("long"));
                p++;
                return r;
            }
            case JT_DOUBLE: { E r = lit(t.text, mk("double")); p++; return r; }
            case JT_FLOAT: {
                // the float nearest the decimal literal, printed with enough digits to read back exactly
                char buf[64];
                float fv = std::strtof(t.text.c_str(), nullptr);
                std::snprintf(buf, sizeof buf, "%.17g", (double)fv);
                std::string txt = buf;
                if (txt.find('.') == std::string::npos && txt.find('e') == std::string::npos && txt.find("inf") == std::string::npos) txt += ".0";
                E r = lit(txt, mk("float")); p++; return r;
            }
            case JT_CHAR: { E r = lit(std::to_string(t.ival), mk("char")); r.isLitInt = false; p++; return r; }
            case JT_STRING:
            case JT_TEXTBLOCK: {
                bool bad = false;
                std::string q = quote(t.text, t, &bad);
                if (bad) fail("model", "string contains a control character with no SLeeLa escape");
                p++;
                return lit(q, mk("String"));
            }
            case JT_OP:
                if (t.text == "(") {
                    p++;
                    E e = parseExpr();
                    expectOp(")");
                    e.isLitInt = false;
                    return e;
                }
                fail("syntax", "unexpected '" + descr() + "' in expression");
            case JT_IDENT: break;
            default: fail("syntax", "unexpected end of expression");
        }
        const std::string& w = t.text;
        if (w == "true" || w == "false") { E r = lit(w, mk("boolean")); p++; return r; }
        if (w == "null") { E r = lit("null", mk("null")); r.isNull = true; p++; return r; }
        if (w == "this") {
            p++;
            if (isOp("(")) {
                std::vector<E> a = parseArgs();
                E r; r.code = "this(" + joinArgs(a) + ")"; r.pure = false; r.ty = mk("void");
                return r;
            }
            if (!anonStack_.empty() && anonStack_.back().isLambda) {
                if (anonStack_.back().outerStatic) fail("lambda-this", "this in a lambda of a static context");
                E r; r.code = "outer_"; r.ty = mk(anonStack_.back().outerClass);
                return r;
            }
            E r; r.code = "this"; r.ty = mk(curClass);
            return r;
        }
        if (w == "super") {
            p++;
            if (isOp("(")) {
                std::vector<E> a = parseArgs();
                E r; r.code = "super(" + joinArgs(a) + ")"; r.pure = false; r.ty = mk("void");
                return r;
            }
            expectOp(".");
            std::string name = ident();
            if (isOp("(")) {
                std::vector<E> a = parseArgs();
                E r; r.code = "super." + name + "(" + joinArgs(a) + ")"; r.pure = false; r.ty = mk("?");
                return r;
            }
            unsup("untyped");
        }
        if (w == "switch") return parseSwitchExpr();
        if (w == "new") return parseNew();
        if (kPrim.count(w)) {
            // int.class, int[].class -- not supported
            unsup("untyped");
        }
        // identifier: local, field, or a type name used as a qualifier
        if (w == "String" && isOpAt(p + 1, ".")) return parseStringStatic();
        std::string name = ident();
        if (isOp("(")) {
            std::vector<E> a = parseArgs();
            const MethodSig* pm = pickMethod(curClass, name, a);
            std::string qualifier;
            if (!pm && !anonStack_.empty()) {
                const AnonCtx& ac = anonStack_.back();
                if (const MethodSig* om = pickMethod(ac.outerClass, name, a)) {
                    pm = om;
                    if (om->isStatic) qualifier = ac.outerClass + ".";
                    else { if (ac.outerStatic) fail("outer-instance", "call of an instance method of the enclosing class from a static context"); qualifier = "outer_."; }
                }
            }
            if (!pm) refuseObjectMethod(curClass, name);
            a = adaptArgs(pm, a);
            E r;
            r.code = qualifier + mapMember(safeNameForCall(name)) + "(" + joinArgs(a) + ")";
            r.pure = false;
            if (pm) r.ty = pm->ret;
            else r.ty = mk("?");
            return r;
        }
        if (const auto* loc = lookupLocal(name)) {
            E r; r.code = loc->first; r.ty = loc->second; return r;
        }
        if (const FieldInfo* fi = findField(curClass, name)) {
            E r; r.code = safeName(name); r.ty = fi->ty; return r;
        }
        { E o; if (resolveOuter(name, &o)) return o; }
        if (isupper((unsigned char)name[0]) || classes.count(name) || nestedMap_.count(name)) {
            std::string tn = resolveTypeName(name);
            E r; r.code = tn; r.isType = true; r.ty = mk("type:" + tn); return r;
        }
        // lower-case unknown name: inherited field or a package qualifier (java.util.X)
        if (isOp(".") && at(p + 1).kind == JT_IDENT && name.find('_') == std::string::npos) {
            // package-qualified name: fold to its simple type name
            size_t q = p;
            std::string last = name;
            while (isOpAt(q, ".") && isIdentTok(at(q + 1)) && islower((unsigned char)last[0])) { last = at(q + 1).text; q += 2; }
            if (isupper((unsigned char)last[0])) { p = q; E r; r.code = last; r.isType = true; r.ty = mk("type:" + last); return r; }
        }
        E r; r.code = safeName(name); r.ty = mk("?"); return r;
    }
    static std::string safeNameForCall(const std::string& n) { return safeName(n); }

    E parseNew() {
        expectKw("new");
        size_t e0 = scanType(p);
        if (e0 == (size_t)-1) fail("syntax", "expected a type after 'new'");
        // element type without dims
        std::string name;
        struct Resolve { };
        {
            if (!isIdentTok(cur())) fail("syntax", "expected a type after 'new'");
            name = ident();
            for (;;) {
                if (isOp("<")) {
                    int d = 0;
                    do { if (isOp("<")) d++; else if (isOp(">")) d--; else if (isOp(">>")) d -= 2; else if (isOp(">>>")) d -= 3; p++; } while (d > 0 && !atEof());
                }
                if (isOp(".") && isIdentTok(at(p + 1))) { p++; name = ident(); continue; }
                break;
            }
        }
        if (!isOp("[") && !typeVars.count(name)) name = resolveTypeName(name);
        if (isOp("[")) {
            // new T[a][b]..[] (syntax 3.1 multi-dimensional creation) or new T[]..{...} (initializer)
            std::vector<std::string> sized;
            int rank = 0;
            bool sawUnsized = false;
            while (isOp("[")) {
                p++;
                if (isOp("]")) { p++; rank++; sawUnsized = true; continue; }
                if (sawUnsized) fail("syntax", "array dimension expression after an empty [] in 'new " + name + "'");
                E n = parseExpr();
                expectOp("]");
                std::string c = classOf(n.ty);
                if (c == "long" || c == "double") fail("model", "array size must be int");
                sized.push_back(n.code);
                rank++;
            }
            E r;
            r.pure = false;
            r.ty = mk(name, rank);
            if (sized.empty()) {                       // new T[]...{ ... }
                if (!isOp("{")) fail("syntax", "expected '{' after 'new " + name + "[]'");
                std::string body = parseArrayInit(r.ty);
                std::string dimsText;
                for (int i = 0; i < rank; i++) dimsText += "[]";
                r.code = "new " + name + dimsText + body;
                return r;
            }
            if (sized.size() > 8) fail("model", "array creation with more than 8 sized dimensions");
            std::string dimsText;
            for (const auto& z : sized) dimsText += "[" + z + "]";
            for (int i = (int)sized.size(); i < rank; i++) dimsText += "[]";
            r.code = "new " + (name == "float" ? std::string("double") : name) + dimsText;
            return r;
        }
        std::vector<E> args = parseArgs();
        if (isOp("{")) return parseAnonymous(name, args);   // anonymous class
        args = adaptArgs(pickMethod(name, "<init>", args), args);
        std::string outerArg;
        {
            auto ci = classes.find(name);
            if (ci != classes.end() && !ci->second.innerOf.empty()) outerArg = outerInstanceCode(ci->second.innerOf) + (args.empty() ? "" : ", ");
        }
        E r;
        r.code = "new " + name + "(" + outerArg + joinArgs(args) + ")";
        r.pure = false;
        r.ty = mk(typeVars.count(name) ? "?" : name);
        return r;
    }

    // -------------------------------------------------------- statements ----
    bool isLvalueChainEnd(size_t i) const {  // after an lvalue chain: ++/-- ; or ) ?
        return true && i < T.size();
    }

    // skip an lvalue chain starting at i (IDENT (. IDENT | [ ... ])*) -> index after, or npos
    size_t scanChain(size_t i) const {
        const size_t npos = (size_t)-1;
        if (!(isIdentTok(at(i)))) return npos;
        i++;
        for (;;) {
            if (isOpAt(i, ".") && isIdentTok(at(i + 1))) { i += 2; continue; }
            if (isOpAt(i, "[")) {
                int d = 0;
                do { if (isOpAt(i, "[")) d++; else if (isOpAt(i, "]")) d--; i++; } while (d > 0 && at(i).kind != JT_EOF);
                continue;
            }
            break;
        }
        return i;
    }

    bool isAssignOp(const Tok& t) const {
        if (t.kind != JT_OP) return false;
        const std::string& s = t.text;
        return s == "=" || s == "+=" || s == "-=" || s == "*=" || s == "/=" || s == "%=" || s == "&=" || s == "|=" || s == "^=" || s == "<<=" || s == ">>=" || s == ">>>=";
    }

    // Parse an expression used as a statement; returns SLeeLa statement text (no ';').
    std::string parseStatementExpr() {
        // prefix ++/--
        if (isOp("++") || isOp("--")) {
            bool inc = isOp("++");
            p++;
            E lv = parseChain();
            return incrText(lv, inc);
        }
        // postfix ++/-- on a simple chain
        size_t after = scanChain(p);
        if (after != (size_t)-1 && (isOpAt(after, "++") || isOpAt(after, "--"))) {
            bool inc = isOpAt(after, "++");
            E lv = parseChain();
            if (!(isOp("++") || isOp("--"))) fail("syntax", "internal: expected ++/--");
            p++;
            return incrText(lv, inc);
        }
        E lhs = parseTernaryNoAssign();
        if (isAssignOp(cur())) {
            std::string op = cur().text;
            p++;
            
            hoistTy_ = lhs.ty; lambdaTarget_ = lhs.ty;
            E rhs = parseRhs(lhs.ty);
            return assignText(lhs, op, rhs);
        }
        if (isOp("++") || isOp("--")) unsup("incr-expr");
        // a plain call / creation used for its effect
        return lhs.code;
    }
    E parseTernaryNoAssign() { return parseTernary(); }

    E parseRhs(const Ty& /*target*/) {
        return parseExpr();
    }

    std::string incrText(const E& lv, bool inc) {
        if (!lv.pure) fail("side-effecting-lvalue", "++/-- on an operand with side effects would evaluate it twice");
        std::string c = classOf(lv.ty);
        if (c == "unknown") unsup("untyped");
        const bool isF = (c == "float");
        if (isF) c = "double";
        const cm_rec* r = cm_find(&M, "incr", c.c_str(), nullptr, nullptr);
        if (!r) fail("model", "the conversion model has no ++/-- rule for operand class '" + c + "'");
        std::string both = r->f[1];
        size_t sep = both.find(" ;; ");
        std::string up = both.substr(0, sep), down = sep == std::string::npos ? both : both.substr(sep + 4);
        std::string val = subst(inc ? up : down, lv.code, "");
        if (isF) val = "JavaFloat.f32(" + val + ")";
        // narrow back to byte/short/char as Java does
        if (lv.ty.dims == 0 && (lv.ty.n == "byte" || lv.ty.n == "short" || lv.ty.n == "char")) {
            E tmp; tmp.code = val; tmp.ty = mk("int");
            val = emitCast(lv.ty, tmp).code;
        }
        return lv.code + " = " + val;
    }

    std::string assignText(const E& lhs, const std::string& op, E rhs) {
        if (op == "=") {
            rhs = coerce(rhs, lhs.ty);
            return lhs.code + " = " + rhs.code;
        }
        if (!lhs.pure) fail("side-effecting-lvalue", "compound assignment on an operand with side effects would evaluate it twice");
        std::string bop = op.substr(0, op.size() - 1);
        std::string cls = classOf(lhs.ty);
        if (cls == "unknown") unsup("untyped");
        if (!cm_find(&M, "compound", (cls == "float" ? "double" : cls).c_str(), bop.c_str(), nullptr))
            fail("model", "the conversion model has no compound-assignment rule for '" + op + "' on '" + cls + "'");
        E combined = emitBinary(bop, lhs, rhs);
        E store = combined;
        if (lhs.ty.dims == 0 && kPrim.count(lhs.ty.n) && lhs.ty.n != "boolean") {
            std::string rc = classOf(combined.ty);
            // narrowing back to the variable type (JLS 15.26.2 implicit cast)
            if (!(rc == "int" && (lhs.ty.n == "int")) && !(rc == "long" && lhs.ty.n == "long") && !(rc == "double" && lhs.ty.n == "double") && !(rc == "float" && lhs.ty.n == "float"))
                store = emitCast(lhs.ty, combined);
        }
        return lhs.code + " = " + store.code;
    }

    void parseBlockInto() {  // '{' statements '}' -> lines at current indent
        expectOp("{");
        pushScope();
        while (!isOp("}")) {
            if (atEof()) fail("syntax", "unexpected end of file in block");
            parseStatement();
        }
        p++;
        popScope();
    }

    // body of a control statement: always rendered as a braced block
    void parseBody() {
        ind++;
        if (isOp("{")) {
            expectOp("{");
            pushScope();
            while (!isOp("}")) {
                if (atEof()) fail("syntax", "unexpected end of file in block");
                parseStatement();
            }
            p++;
            popScope();
        } else {
            pushScope();
            parseStatement();
            popScope();
        }
        ind--;
    }

    bool looksLikeDecl() const {
        size_t i = p;
        while (isKwAt(i, "final") || isOpAt(i, "@")) {
            if (isOpAt(i, "@")) { i += 2; while (isOpAt(i, ".")) i += 2; if (isOpAt(i, "(")) { int d = 0; do { if (isOpAt(i, "(")) d++; else if (isOpAt(i, ")")) d--; i++; } while (d > 0 && at(i).kind != JT_EOF); } }
            else i++;
        }
        const Tok& f = at(i);
        if (f.kind != JT_IDENT) return false;
        static const std::set<std::string> stmtKw = {"return", "if", "while", "for", "do", "switch", "try", "throw", "break", "continue", "new", "this", "super", "assert", "synchronized", "else", "case", "default", "yield"};
        if (stmtKw.count(f.text)) return false;
        size_t e = scanType(i);
        if (e == (size_t)-1) return false;
        if (!isIdentTok(at(e))) return false;
        const Tok& n = at(e + 1);
        return n.kind == JT_OP && (n.text == "=" || n.text == ";" || n.text == "," || n.text == "[" || n.text == ":");
    }

    void parseLocalDecl(bool inFor, std::string* forInit) {
        while (isKw("final") || isOp("@")) {
            if (isOp("@")) { p += 2; while (isOp(".")) p += 2; if (isOp("(")) { int d = 0; do { if (isOp("(")) d++; else if (isOp(")")) d--; p++; } while (d > 0 && !atEof()); } }
            else p++;
        }
        bool isVar = isKw("var") && isIdentTok(at(p + 1));
        Ty base = isVar ? mk("var") : parseType();
        if (isVar) p++;
        std::vector<std::string> pieces;
        do {
            std::string name = ident();
            Ty ty = base;
            while (isOp("[") && isOpAt(p + 1, "]")) { p += 2; ty.dims++; }
            std::string initCode;
            bool haveInit = false;
            if (acceptOp("=")) {
                haveInit = true;
                if (isOp("{")) {
                    initCode = parseArrayInit(ty);
                } else {
                    
                    if (!isVar) { hoistTy_ = ty; lambdaTarget_ = ty; }
                    E v = parseExpr();
                    if (isVar) {
                        if (!v.ty.known() || v.ty.n == "null" || v.ty.n.rfind("type:", 0) == 0) unsup("untyped");
                        ty = v.ty;
                    }
                    v = coerce(v, ty);
                    initCode = v.code;
                }
            } else if (isVar) unsup("untyped");
            if (inFor) {
                std::string en = declare(name, ty);
                pieces.push_back(emitType(ty) + " " + en + (haveInit ? " = " + initCode : ""));
                if (forInit) { /* single declaration expected */ }
                continue;
            }
            std::string en = declare(name, ty);
            if (haveInit) line(emitType(ty) + " " + en + " = " + initCode + ";");
            else line(emitType(ty) + " " + en + ";");
        } while (acceptOp(","));
        if (inFor) {
            if (pieces.size() != 1) fail("model", "for-loop initializer declaring several variables has no single SLeeLa form");
            *forInit = pieces[0];
        }
    }

    void parseStatement() {
        allowHoist_ = true; hoistTy_ = mk("?");
        const Tok& t = cur();
        if (t.kind == JT_OP) {
            if (t.text == "{") {
                line("{");
                ind++;
                expectOp("{");
                pushScope();
                while (!isOp("}")) { if (atEof()) fail("syntax", "unexpected end of file in block"); parseStatement(); }
                p++;
                popScope();
                ind--;
                line("}");
                return;
            }
            if (t.text == ";") { p++; return; }
        }
        if (t.kind == JT_IDENT) {
            const std::string& w = t.text;
            // labeled statement
            if (isOpAt(p + 1, ":") && !(w == "default" || w == "case")) {   // syntax 3.1 labeled statement
                std::string lab = safeName(ident());
                expectOp(":");
                line(lab + ":");
                parseStatement();
                return;
            }
            if (w == "if") { parseIf(); return; }
            if (w == "while") {
                p++; expectOp("(");
                allowHoist_ = false;
                E c = parseExpr(); expectOp(")");
                line("while (" + c.code + ") {");
                breakCtx.push_back('L');
                parseBody();
                breakCtx.pop_back();
                line("}");
                return;
            }
            if (w == "do") {
                p++;
                line("do {");
                breakCtx.push_back('L');
                parseBody();
                breakCtx.pop_back();
                expectKw("while"); expectOp("(");
                allowHoist_ = false;
                E c = parseExpr(); expectOp(")"); expectOp(";");
                line("} while (" + c.code + ");");
                return;
            }
            if (w == "for") { parseFor(); return; }
            if (w == "switch") { parseSwitch(); return; }
            if (w == "return") {
                p++;
                if (acceptOp(";")) { line("return;"); return; }
                
                hoistTy_ = curRet; lambdaTarget_ = curRet;
                E v = parseExpr();
                expectOp(";");
                v = coerce(v, curRet);
                line("return " + v.code + ";");
                return;
            }
            if (w == "yield" && !yieldVar_.empty() && !isOpAt(p + 1, "=") && !isOpAt(p + 1, ".") && !isOpAt(p + 1, "(")) {
                if (breakCtx.size() != yieldBase_) fail("switch-expr", "yield inside a nested loop or switch cannot be expressed with a single break");
                p++;
                E v = parseExpr();
                expectOp(";");
                noteYieldType(v.ty);
                v = coerce(v, yieldTyKnown_ ? yieldTy_ : v.ty);
                line(yieldVar_ + " = " + v.code + ";");
                line("break;");
                return;
            }
            if (w == "break") {
                p++;
                if (isIdentTok(cur())) { std::string lab = safeName(ident()); expectOp(";"); line("break " + lab + ";"); return; }
                expectOp(";");
                if (!breakCtx.empty() && breakCtx.back() == 'S')
                    fail("switch-break", "a break inside a lowered switch that is not its final statement of a case group cannot be expressed without a jump");
                line("break;");
                return;
            }
            if (w == "continue") {
                p++;
                if (isIdentTok(cur())) { std::string lab = safeName(ident()); expectOp(";"); line("continue " + lab + ";"); return; }
                expectOp(";");
                line("continue;");
                return;
            }
            if (w == "throw") {
                p++;
                E v = parseExpr();
                expectOp(";");
                line("throw " + v.code + ";");
                return;
            }
            if (w == "try") { parseTry(); return; }
            if (w == "synchronized") {
                if (!opt.singleThread) unsup("synchronized");
                p++; expectOp("(");
                allowHoist_ = false;
                E mon = parseExpr();
                expectOp(")");
                line("< synchronized block, converted for single-thread execution >");
                line("{");
                (void)mon;
                parseBody();
                line("}");
                return;
            }
            if (w == "assert") {
                // assertions are disabled unless the JVM runs with -ea; as a default run they have no effect
                p++;
                int d = 0;
                while (!(isOp(";") && d == 0)) { if (isOp("(")) d++; else if (isOp(")")) d--; if (atEof()) fail("syntax", "unterminated assert"); p++; }
                p++;
                line("< assert statement omitted: Java assertions are disabled by default >");
                return;
            }
            if (w == "class" || w == "interface" || w == "enum" || (w == "record" && isIdentTok(at(p + 1)) && isOpAt(p + 2, "("))) unsup("nested");
            if (w == "abstract" || w == "static") unsup("nested");
            if (looksLikeDecl()) {
                parseLocalDecl(false, nullptr);
                expectOp(";");
                return;
            }
        }
        // expression statement
        std::string s = parseStatementExpr();
        expectOp(";");
        line(s + ";");
    }

    void parseIf() {
        expectKw("if"); expectOp("(");
        allowHoist_ = false;
        E c = parseExpr(); expectOp(")");
        line("if (" + c.code + ") {");
        parseBody();
        while (isKw("else")) {
            p++;
            if (isKw("if")) {
                p++; expectOp("(");
                allowHoist_ = false;
                E c2 = parseExpr(); expectOp(")");
                line("} else if (" + c2.code + ") {");
                parseBody();
                continue;
            }
            line("} else {");
            parseBody();
            break;
        }
        line("}");
    }

    void parseFor() {
        expectKw("for"); expectOp("(");
        allowHoist_ = false;
        pushScope();
        // enhanced for?
        {
            size_t i = p;
            while (isKwAt(i, "final")) i++;
            size_t e = scanType(i);
            if (e != (size_t)-1 && isIdentTok(at(e)) && isOpAt(e + 1, ":")) {
                while (isKw("final")) p++;
                bool isVar = isKw("var") && isIdentTok(at(p + 1));
                Ty ty = isVar ? mk("?") : parseType();
                if (isVar) p++;
                std::string name = ident();
                expectOp(":");
                E coll = parseExpr();
                expectOp(")");
                if (isVar && coll.ty.dims > 0) ty = mk(coll.ty.n, coll.ty.dims - 1);
                if (isVar && !ty.known()) unsup("untyped");
                std::string en = declare(name, ty);
                line("for (" + emitType(ty) + " " + en + " : " + coll.code + ") {");
                breakCtx.push_back('L');
                parseBody();
                breakCtx.pop_back();
                line("}");
                popScope();
                return;
            }
        }
        std::string init, cond, upd;
        if (!isOp(";")) {
            if (looksLikeDecl()) parseLocalDecl(true, &init);
            else {
                init = parseStatementExpr();
                if (isOp(",")) fail("model", "for-loop with several initializer expressions has no single SLeeLa form");
            }
        }
        expectOp(";");
        if (!isOp(";")) cond = parseExpr().code;
        expectOp(";");
        if (!isOp(")")) {
            upd = parseStatementExpr();
            if (isOp(",")) fail("model", "for-loop with several update expressions has no single SLeeLa form");
        }
        expectOp(")");
        line("for (" + init + "; " + cond + "; " + upd + ") {");
        breakCtx.push_back('L');
        parseBody();
        breakCtx.pop_back();
        line("}");
        popScope();
    }

    size_t skipBraces(size_t i) const {
        int d = 0;
        do { if (isOpAt(i, "{")) d++; else if (isOpAt(i, "}")) d--; i++; } while (d > 0 && at(i).kind != JT_EOF);
        return i;
    }

    // try (R r = init; ...) body [catch...] [finally...]  (JLS 14.20.3). Each resource is closed in reverse
    // order after the body; an exception from the body is kept as primary and one from close() is added
    // to it as suppressed; the catch and finally clauses of the statement wrap all of this.
    void emitResources(bool* outerOpened) {
        expectOp("(");
        size_t afterHeader = p;
        { int d = 1; size_t i = p; while (d > 0 && at(i).kind != JT_EOF) { if (isOpAt(i, "(")) d++; else if (isOpAt(i, ")")) d--; i++; } afterHeader = i; }
        size_t afterBody = skipBraces(afterHeader);
        bool outer = isKwAt(afterBody, "catch") || isKwAt(afterBody, "finally");
        *outerOpened = outer;
        if (outer) { line("try {"); ind++; } else { line("{"); ind++; }
        pushScope();
        struct Res { std::string var; int id; };
        std::vector<Res> res;
        while (!isOp(")")) {
            if (atEof()) fail("syntax", "unterminated resource specification");
            Res r; r.id = ++tmpCounter;
            if (looksLikeDecl()) {
                while (isKw("final")) p++;
                bool isVar = isKw("var") && isIdentTok(at(p + 1));
                Ty ty = isVar ? mk("?") : parseType();
                if (isVar) p++;
                std::string nm = ident();
                expectOp("=");
                E init = parseExpr();
                if (isVar) { if (!init.ty.known()) unsup("untyped"); ty = init.ty; }
                if (!ty.known()) unsup("untyped");
                r.var = declare(nm, ty);
                line(emitType(ty) + " " + r.var + " = " + init.code + ";");
            } else {
                E v = parseExpr();
                if (!v.ty.known()) unsup("untyped");
                r.var = "res_" + std::to_string(r.id);
                usedNames.insert(r.var);
                line(emitType(v.ty) + " " + r.var + " = " + v.code + ";");
            }
            line("Any primary_" + std::to_string(r.id) + " = null;");
            line("try {");
            ind++;
            res.push_back(r);
            if (!acceptOp(";")) break;
        }
        expectOp(")");
        // the body, indented inside the innermost try
        {
            expectOp("{");
            pushScope();
            while (!isOp("}")) { if (atEof()) fail("syntax", "unexpected end of file in block"); parseStatement(); }
            p++;
            popScope();
        }
        for (size_t k = res.size(); k-- > 0;) {
            std::string id = std::to_string(res[k].id);
            ind--;
            line("} catch (Any thrown_" + id + ") {");
            line("  primary_" + id + " = thrown_" + id + ";");
            line("  throw thrown_" + id + ";");
            line("} finally {");
            line("  if (" + res[k].var + " != null) {");
            line("    if (primary_" + id + " != null) {");
            line("      try { " + res[k].var + ".closeResource(); } catch (Any suppressed_" + id + ") { ((Throwable) primary_" + id + ").addSuppressed((Throwable) suppressed_" + id + "); }");
            line("    } else { " + res[k].var + ".closeResource(); }");
            line("  }");
            line("}");
        }
        popScope();
        if (!outer) { ind--; line("}"); }
        else { ind--; }
    }

    void parseTry() {
        expectKw("try");
        if (isOp("(")) {
            bool outer = false;
            emitResources(&outer);
            if (!outer) return;
            // the statement's own catch / finally clauses follow the resource block
        } else {
            line("try {");
            parseBody();
        }
        while (isKw("catch")) {
            p++; expectOp("(");
            while (isKw("final")) p++;
            std::vector<Ty> types;
            types.push_back(parseType());
            while (acceptOp("|")) types.push_back(parseType());
            std::string name = ident();
            expectOp(")");
            // multi-catch: replicate the handler body once per alternative (identical behaviour)
            size_t bodyStart = p;
            size_t bodyEnd = bodyStart;
            for (size_t k = 0; k < types.size(); k++) {
                p = bodyStart;
                pushScope();
                std::string en = declare(name, types[k]);
                line("} catch (" + emitType(types[k]) + " " + en + ") {");
                parseBody();
                popScope();
                bodyEnd = p;
            }
            p = bodyEnd;
        }
        if (isKw("finally")) {
            p++;
            line("} finally {");
            parseBody();
        }
        line("}");
    }

    // ---- static members of String ------------------------------------------------------------
    // String is a keyword type in SLeeLa, so String.valueOf / join / format cannot be called; the
    // Java tier lowers them. valueOf(x) is the string conversion of x (JLS 5.1.11). join(sep, ...)
    // goes to JavaFormat.joinArray. format(literal, args...) is expanded at conversion time into a
    // concatenation of JavaFormat calls, one per conversion, so each argument keeps its Java type.
    E parseStringStatic() {
        p++;                       // String
        expectOp(".");
        std::string name = ident();
        std::vector<E> args = parseArgs();
        E r;
        r.pure = false;
        r.ty = mk("String");
        if (name == "valueOf" && args.size() == 1) {
            const E& a = args[0];
            if (!a.ty.known() && !a.isNull) unsup("untyped");
            if (a.ty.dims == 1 && a.ty.n == "char") fail("model", "String.valueOf(char[]) is not converted");
            E empty = lit("\"\"", mk("String"));
            E v = emitBinary("+", empty, a);
            v.pure = a.pure;
            return v;
        }
        if (name == "join" && args.size() >= 1) {
            std::string sep = strOfText(args[0]);
            if (args.size() == 2 && args[1].ty.dims == 1 && args[1].ty.n == "String") {
                r.code = "JavaFormat.joinArray(" + sep + ", " + args[1].code + ")";
                return r;
            }
            std::string body;
            for (size_t i = 1; i < args.size(); i++) {
                if (!(args[i].ty.dims == 0 && args[i].ty.n == "String")) fail("model", "String.join over something other than Strings or a String[] is not converted");
                body += (i > 1 ? ", " : "") + args[i].code;
            }
            r.code = args.size() == 1 ? "JavaFormat.joinArray(" + sep + ", new String[0])" : "JavaFormat.joinArray(" + sep + ", new String[]{" + body + "})";
            return r;
        }
        if (name == "format" && !args.empty()) return lowerFormat(args);
        fail("string-static", "String." + name + " with " + std::to_string(args.size()) + " argument(s) is not converted");
    }
    std::string strOfText(const E& a) { return "(\"\" + " + strOf(a) + ")"; }

    E lowerFormat(const std::vector<E>& args) {
        // the format must be a string literal: its text is recovered from the generated code
        const E& f = args[0];
        if (!(f.ty.dims == 0 && f.ty.n == "String") || f.code.size() < 2 || f.code.front() != '"' || f.code.back() != '"')
            fail("format-nonliteral", "String.format is converted only with a string literal format");
        // unquote our own quote() output
        std::string fmt;
        for (size_t i = 1; i + 1 < f.code.size(); i++) {
            char c = f.code[i];
            if (c == '\\' && i + 2 < f.code.size()) {
                char n = f.code[++i];
                fmt += (n == 'n') ? '\n' : (n == 't') ? '\t' : (n == 'r') ? '\r' : n;
            } else fmt += c;
        }
        std::vector<std::string> parts;
        std::string lit;
        size_t ai = 1;
        auto flushLit = [&]() {
            if (lit.empty()) return;
            bool bad = false;
            Tok none;
            parts.push_back(quote(lit, none, &bad));
            lit.clear();
        };
        for (size_t i = 0; i < fmt.size(); i++) {
            if (fmt[i] != '%') { lit += fmt[i]; continue; }
            i++;
            if (i >= fmt.size()) fail("format-conv", "format string ends with %");
            int flags = 0;
            for (; i < fmt.size(); i++) {
                char c = fmt[i];
                if (c == '-') flags |= 1; else if (c == '0') flags |= 2; else if (c == '+') flags |= 4;
                else if (c == ',') flags |= 8; else if (c == ' ') flags |= 16; else break;
            }
            int width = 0, prec = -1;
            while (i < fmt.size() && isdigit((unsigned char)fmt[i])) width = width * 10 + (fmt[i++] - '0');
            if (i < fmt.size() && fmt[i] == '.') { i++; prec = 0; while (i < fmt.size() && isdigit((unsigned char)fmt[i])) prec = prec * 10 + (fmt[i++] - '0'); }
            if (i >= fmt.size()) fail("format-conv", "incomplete format specifier");
            char conv = fmt[i];
            if (conv == '%') { lit += '%'; continue; }
            if (conv == 'n') { lit += '\n'; continue; }
            if (ai >= args.size()) fail("format-arg", "the format has more conversions than arguments");
            const E& a = args[ai++];
            std::string w = std::to_string(width), fl = std::to_string(flags);
            std::string cls = classOf(a.ty);
            flushLit();
            if (conv == 'd') {
                if (cls != "int" && cls != "long") fail("format-arg", "%d needs an integral argument");
                parts.push_back("JavaFormat.dec(" + a.code + ", " + w + ", " + fl + ")");
            } else if (conv == 'x' || conv == 'X' || conv == 'o') {
                if (cls != "int" && cls != "long") fail("format-arg", std::string("%") + conv + " needs an integral argument");
                int bits = (a.ty.n == "long" || a.ty.n == "Long") ? 64 : (a.ty.n == "byte" || a.ty.n == "Byte") ? 8 : (a.ty.n == "short" || a.ty.n == "Short") ? 16 : 32;
                parts.push_back("JavaFormat.radix(" + a.code + ", " + std::to_string(bits) + ", " + (conv == 'o' ? "3" : "4") + ", " + (conv == 'X' ? "true" : "false") + ", " + w + ", " + fl + ")");
            } else if (conv == 'f') {
                if (cls != "double" && cls != "float") fail("format-arg", "%f needs a floating point argument");
                parts.push_back("JavaFormat.fixed(" + a.code + ", " + std::to_string(prec < 0 ? 6 : prec) + ", " + w + ", " + fl + ")");
            } else if (conv == 's') {
                parts.push_back("JavaFormat.text(" + strOfText(a) + ", " + std::to_string(prec) + ", " + w + ", " + fl + ")");
            } else if (conv == 'c') {
                std::string txt;
                if (a.ty.dims == 0 && a.ty.n == "char") txt = strOf(a);
                else if (cls == "int") { const cm_rec* tr = cm_find(&M, "tostr", "char", nullptr, nullptr); txt = tr ? subst(tr->f[1], a.code, "") : a.code; }
                else fail("format-arg", "%c needs a char or int argument");
                parts.push_back("JavaFormat.text(" + txt + ", -1, " + w + ", " + fl + ")");
            } else if (conv == 'b') {
                if (cls != "boolean") fail("format-arg", "%b is converted for boolean arguments only");
                parts.push_back("JavaFormat.text((\"\" + " + a.code + "), " + std::to_string(prec) + ", " + w + ", " + fl + ")");
            } else {
                fail("format-conv", std::string("the conversion %") + conv + " is not converted (supported: d x X o f s c b n %)");
            }
        }
        if (ai != args.size()) fail("format-arg", "the format has fewer conversions than arguments");
        flushLit();
        E r;
        r.pure = false;
        r.ty = mk("String");
        if (parts.empty()) { r.code = "\"\""; return r; }
        std::string code = "(" + parts[0];
        for (size_t i = 1; i < parts.size(); i++) code += " + " + parts[i];
        r.code = code + ")";
        return r;
    }

    struct Saved {
        std::string curClass; std::set<std::string> typeVars; bool curIsInterface;
        std::vector<std::map<std::string, std::pair<std::string, Ty>>> scopes; std::set<std::string> usedNames;
        Ty curRet; std::vector<std::string> out; int ind; int tmpCounter; std::vector<char> breakCtx;
        std::string yieldVar; Ty yieldTy; bool yieldTyKnown; size_t yieldBase, yieldDeclIdx;
        bool allowHoist; Ty hoistTy; size_t hoistPos; int exprDepth; bool enumMode, lastVarargs; Ty lambdaTarget; bool curIsStatic;
    };
    Saved saveState() const {
        return Saved{curClass, typeVars, curIsInterface, scopes, usedNames, curRet, out, ind, tmpCounter, breakCtx,
                     yieldVar_, yieldTy_, yieldTyKnown_, yieldBase_, yieldDeclIdx_, allowHoist_, hoistTy_, hoistPos_, exprDepth_,
                     enumMode_, lastVarargs_, lambdaTarget_, curIsStatic_};
    }
    void restoreState(const Saved& v) {
        curClass = v.curClass; typeVars = v.typeVars; curIsInterface = v.curIsInterface; scopes = v.scopes; usedNames = v.usedNames;
        curRet = v.curRet; out = v.out; ind = v.ind; tmpCounter = v.tmpCounter; breakCtx = v.breakCtx;
        yieldVar_ = v.yieldVar; yieldTy_ = v.yieldTy; yieldTyKnown_ = v.yieldTyKnown; yieldBase_ = v.yieldBase; yieldDeclIdx_ = v.yieldDeclIdx;
        allowHoist_ = v.allowHoist; hoistTy_ = v.hoistTy; hoistPos_ = v.hoistPos; exprDepth_ = v.exprDepth;
        enumMode_ = v.enumMode; lastVarargs_ = v.lastVarargs; lambdaTarget_ = v.lambdaTarget; curIsStatic_ = v.curIsStatic;
    }

    // the expression that is the enclosing instance of kind `want` at this point (for new Inner())
    std::string outerInstanceCode(const std::string& want) {
        if (curClass == want && !curIsStatic_ && anonStack_.empty()) return "this";
        if (!anonStack_.empty() && anonStack_.back().outerClass == want && !anonStack_.back().outerStatic) return "outer_";
        auto it = classes.find(curClass);
        if (it != classes.end() && it->second.innerOf == want) return "outer_";
        if (curClass == want && !curIsStatic_) return "this";
        fail("outer-instance", "creating an inner class of " + want + " needs an enclosing instance that is not available here");
    }

    // the generated class text: header, metadata, outer_/capture fields, constructor, member lines
    void finishGenerated(const AnonCtx& a, const std::string& base, bool baseIsInterface, const std::vector<std::string>& memberLines,
                         const std::vector<std::pair<std::string, Ty>>& superParams) {
        std::ostringstream o;
        o << "class " << a.sub;
        if (!base.empty()) o << (baseIsInterface ? " implements " : " extends ") << base;
        o << " {\n" << metaBlock(a.sub) << "\n";
        std::string params, assigns;
        bool needOuter = !a.outerStatic;
        if (needOuter) { o << "  " << a.outerClass << " outer_;\n"; params += a.outerClass + " outerArg_"; assigns += "    this.outer_ = outerArg_;\n"; }
        for (size_t i = 0; i < a.captures.size(); i++) {
            const auto& c = a.captures[i];
            o << "  " << emitType(c.second) << " cap_" << c.first << ";\n";
            params += (params.empty() ? "" : ", ") + emitType(c.second) + " capArg" + std::to_string(i);
            assigns += "    this.cap_" + c.first + " = capArg" + std::to_string(i) + ";\n";
        }
        std::string supers;
        for (size_t i = 0; i < superParams.size(); i++) {
            params += (params.empty() ? "" : ", ") + emitType(superParams[i].second) + " superArg" + std::to_string(i);
            supers += (i ? ", " : "") + std::string("superArg") + std::to_string(i);
        }
        o << "  " << a.sub << "(" << params << ") {\n";
        if (!baseIsInterface && !superParams.empty()) o << "    super(" << supers << ");\n";
        o << assigns << "  }\n";
        for (auto& l : memberLines) o << l << "\n";
        o << "}\n";
        OutFile f; f.typeName = a.sub; f.text = o.str();
        pendingTypes_.push_back(f);
    }

    // the call that creates the generated object, evaluated in the enclosing method
    std::string creationCode(const AnonCtx& a, const std::vector<E>& superArgs) {
        std::string args;
        if (!a.outerStatic) args += "this";
        for (const auto& c : a.captures) args += (args.empty() ? "" : ", ") + c.first;
        for (const auto& e : superArgs) args += (args.empty() ? "" : ", ") + e.code;
        return "new " + a.sub + "(" + args + ")";
    }

    bool inGenerated() const { for (const auto& c : anonStack_) if (c.sub.rfind("#nest", 0) != 0) return true; return false; }
    std::string nextAnonName() { return topLevelName_ + "__" + std::to_string(++anonCounter_); }
    std::string topLevelName_;

    // new T(args) { members }
    E parseAnonymous(const std::string& base, std::vector<E> superArgs) {
        InitScope noHoist(this, nullptr);
        if (inGenerated()) fail("anonymous-nested", "an anonymous class inside another anonymous class or lambda is not converted");
        AnonCtx a;
        a.sub = nextAnonName();
        a.outerClass = curClass;
        a.outerStatic = curIsStatic_ || curIsInterface;
        a.outerScopes = scopes;
        ClassInfo& sci = classes[a.sub];
        auto bi = classes.find(base);
        const bool baseKnown = bi != classes.end();
        const bool baseIsInterface = baseKnown ? bi->second.isInterface : true;   // an unlisted JDK type is taken to be an interface
        if (pass == 1) { sci.fields.clear(); sci.methods.clear(); sci.super = baseIsInterface ? "" : base; sci.ifaces.clear(); if (baseIsInterface) sci.ifaces.push_back(base); }
        std::vector<std::pair<std::string, Ty>> superParams;
        if (!baseIsInterface && !superArgs.empty()) {
            const MethodSig* sc = baseKnown ? pickMethod(base, "<init>", superArgs) : nullptr;
            if (!sc) fail("anonymous-super", "the constructor of " + base + " called by the anonymous class is not known");
            superArgs = adaptArgs(sc, superArgs);
            for (size_t i = 0; i < sc->params.size(); i++) superParams.push_back({"", sc->params[i]});
        }
        Saved saved = saveState();
        anonStack_.push_back(a);
        curClass = a.sub;
        curIsStatic_ = false;
        curIsInterface = false;
        std::vector<std::string> memberLines;
        int instanceFields = 0;
        expectOp("{");
        try {
            while (!isOp("}")) {
                if (atEof()) fail("syntax", "unexpected end of file in anonymous class body");
                if (acceptOp(";")) continue;
                out.clear(); ind = 1;
                parseMember(a.sub, false, sci, &instanceFields);
                for (auto& l : out) memberLines.push_back(l);
            }
            p++;
        } catch (...) { anonStack_.pop_back(); restoreState(saved); throw; }
        AnonCtx done = anonStack_.back();
        anonStack_.pop_back();
        restoreState(saved);
        if (instanceFields + 13 + 1 + (int)done.captures.size() > 64) fail("model", "the anonymous class needs more than 64 fields");
        finishGenerated(done, base, baseIsInterface, memberLines, superParams);
        E r;
        r.code = creationCode(done, superArgs);
        r.pure = false;
        r.ty = mk(a.sub);
        return r;
    }

    // the one abstract method of an interface (looked up through its super-interfaces)
    const MethodSig* functionalMethod(const std::string& iface, int depth = 0) const {
        auto it = classes.find(iface);
        if (it == classes.end() || depth > 16) return nullptr;
        const MethodSig* found = nullptr;
        for (const auto& m : it->second.methods) if (m.isAbstract && !m.isStatic) { if (found) return nullptr; found = &m; }
        if (found) return found;
        for (const auto& i : it->second.ifaces) if (const MethodSig* f = functionalMethod(i, depth + 1)) return f;
        return nullptr;
    }

    bool looksLikeLambdaTypedParam() const {
        size_t i = p;
        while (isKwAt(i, "final")) i++;
        size_t e = scanType(i);
        return e != (size_t)-1 && isIdentTok(at(e)) && (isOpAt(e + 1, ",") || isOpAt(e + 1, ")"));
    }

    // (a, b) -> expr   (int a, int b) -> { ... }   x -> expr   where the target is a functional interface
    E parseLambda() {
        Ty target = lambdaTarget_;
        lambdaTarget_ = Ty();
        if (!target.known() || target.dims != 0 || target.n.rfind("type:", 0) == 0) fail("lambda-target", "the type a lambda is converted to is not known here");
        const MethodSig* fm = functionalMethod(target.n);
        if (!fm) fail("lambda-target", "'" + target.n + "' is not a known functional interface (one abstract method)");
        std::vector<std::string> names;
        std::vector<Ty> types;
        if (isOp("(")) {
            p++;
            while (!isOp(")")) {
                if (looksLikeLambdaTypedParam()) { while (isKw("final")) p++; Ty t = parseType(); names.push_back(ident()); types.push_back(t); }
                else { names.push_back(ident()); types.push_back(Ty()); }
                if (!acceptOp(",")) break;
            }
            expectOp(")");
        } else { names.push_back(ident()); types.push_back(Ty()); }
        expectOp("->");
        if (names.size() != fm->params.size()) fail("lambda-target", "the lambda has " + std::to_string(names.size()) + " parameter(s) but " + target.n + " declares " + std::to_string(fm->params.size()));
        for (size_t i = 0; i < types.size(); i++) if (!types[i].known()) types[i] = fm->params[i];
        if (inGenerated()) fail("anonymous-nested", "a lambda inside an anonymous class or another lambda is not converted");

        AnonCtx a;
        a.sub = nextAnonName();
        a.outerClass = curClass;
        a.outerStatic = curIsStatic_ || curIsInterface;
        a.outerScopes = scopes;
        a.isLambda = true;
        ClassInfo& sci = classes[a.sub];
        if (pass == 1) { sci.fields.clear(); sci.methods.clear(); sci.super = ""; sci.ifaces.clear(); sci.ifaces.push_back(target.n); }
        const MethodSig fmCopy = *fm;
        Saved saved = saveState();
        anonStack_.push_back(a);
        curClass = a.sub;
        curIsStatic_ = false;
        curIsInterface = false;
        std::vector<std::string> memberLines;
        try {
            scopes.clear(); usedNames.clear(); pushScope(); tmpCounter = 0; breakCtx.clear();
            curRet = fmCopy.ret;
            std::string plist;
            for (size_t i = 0; i < names.size(); i++) {
                std::string en = declare(names[i], types[i]);
                plist += (i ? ", " : "") + emitType(types[i]) + " " + en;
            }
            out.clear(); ind = 2;
            const bool voidRet = fmCopy.ret.dims == 0 && fmCopy.ret.n == "void";
            if (isOp("{")) {
                expectOp("{");
                pushScope();
                while (!isOp("}")) { if (atEof()) fail("syntax", "unexpected end of file in lambda body"); parseStatement(); }
                p++;
                popScope();
            } else if (voidRet) {
                allowHoist_ = false;
                std::string st = parseStatementExpr();
                line(st + ";");
            } else {
                allowHoist_ = true; hoistTy_ = fmCopy.ret;
                E v = parseExpr();
                v = coerce(v, fmCopy.ret);
                line("return " + v.code + ";");
            }
            std::vector<std::string> body = out;
            out.clear(); ind = 1;
            memberLines.push_back("  " + emitType(fmCopy.ret) + " " + mapMember(fmCopy.name) + "(" + plist + ") {");
            for (auto& l : body) memberLines.push_back(l);
            memberLines.push_back("  }");
        } catch (...) { anonStack_.pop_back(); restoreState(saved); throw; }
        AnonCtx done = anonStack_.back();
        anonStack_.pop_back();
        restoreState(saved);
        if (pass == 1) { MethodSig ms; ms.name = fmCopy.name; ms.ret = fmCopy.ret; ms.params = fmCopy.params; classes[a.sub].methods.push_back(ms); }
        finishGenerated(done, target.n, true, memberLines, {});
        E r;
        r.code = creationCode(done, {});
        r.pure = false;
        r.ty = target;
        return r;
    }

    // Type::staticMethod  Type::instanceMethod  Type::new  this::method  local::method  (JLS 15.13) become
    // the lambda they stand for, in a generated class that implements the target interface.
    E buildMethodRef(const E& recv) {
        expectOp("::");
        Ty target = lambdaTarget_;
        lambdaTarget_ = Ty();
        std::string mname = isKw("new") ? (p++, std::string("new")) : ident();
        if (!target.known() || target.dims != 0) fail("lambda-target", "the type a method reference is converted to is not known here");
        const MethodSig* fm = functionalMethod(target.n);
        if (!fm) fail("lambda-target", "'" + target.n + "' is not a known functional interface (one abstract method)");
        if (inGenerated()) fail("anonymous-nested", "a method reference inside an anonymous class or lambda is not converted");
        const size_t n = fm->params.size();
        std::vector<std::string> pn;
        for (size_t i = 0; i < n; i++) pn.push_back("a" + std::to_string(i));
        auto args = [&](size_t from) { std::string r; for (size_t i = from; i < n; i++) r += (r.empty() ? "" : ", ") + pn[i]; return r; };
        std::string call;
        std::string capture;   // a captured receiver, by its local name
        if (mname == "new") {
            if (!recv.isType) fail("methodref", "Type::new needs a type");
            call = "new " + recv.code + "(" + args(0) + ")";
        } else if (recv.isType) {
            const std::string tn = recv.code;
            bool isStatic = false, found = false;
            std::vector<E> probe;
            auto it = classes.find(tn);
            if (it != classes.end()) for (const auto& m : it->second.methods) if (m.name == mname && !m.isAbstract) { found = true; isStatic = m.isStatic; }
            if (it != classes.end()) for (const auto& m : it->second.methods) if (m.name == mname) { found = true; isStatic = m.isStatic; }
            if (!found) fail("methodref", "cannot tell whether " + tn + "::" + mname + " is static or an instance method (its source was not collected)");
            if (isStatic) call = tn + "." + mapMember(mname) + "(" + args(0) + ")";
            else { if (n < 1) fail("methodref", "an unbound instance method reference needs a receiver parameter"); call = pn[0] + "." + mapMember(mname) + "(" + args(1) + ")"; }
        } else if (recv.code == "this") {
            call = "outer_." + mapMember(mname) + "(" + args(0) + ")";
        } else {
            bool isLocal = false;
            for (size_t i = scopes.size(); i-- > 0;) for (auto& kv : scopes[i]) if (kv.second.first == recv.code) isLocal = true;
            if (!isLocal) fail("methodref", "a method reference on a receiver that is not a local variable is not converted");
            capture = recv.code;
            call = "cap_" + recv.code + "." + mapMember(mname) + "(" + args(0) + ")";
        }
        AnonCtx a;
        a.sub = nextAnonName();
        a.outerClass = curClass;
        a.outerStatic = curIsStatic_ || curIsInterface;
        if (recv.code == "this" && a.outerStatic) fail("methodref", "this:: in a static context");
        a.isLambda = true;
        ClassInfo& sci = classes[a.sub];
        if (pass == 1) { sci.fields.clear(); sci.methods.clear(); sci.super = ""; sci.ifaces.clear(); sci.ifaces.push_back(target.n); }
        if (!capture.empty()) {
            Ty ct;
            for (size_t i = scopes.size(); i-- > 0;) for (auto& kv : scopes[i]) if (kv.second.first == capture) ct = kv.second.second;
            a.captures.push_back({capture, ct});
        }
        std::string plist;
        for (size_t i = 0; i < n; i++) plist += (i ? ", " : "") + emitType(fm->params[i]) + " " + pn[i];
        const bool voidRet = fm->ret.dims == 0 && fm->ret.n == "void";
        std::vector<std::string> lines;
        lines.push_back("  " + emitType(fm->ret) + " " + mapMember(fm->name) + "(" + plist + ") {");
        lines.push_back(voidRet ? "    " + call + ";" : "    return " + call + ";");
        lines.push_back("  }");
        if (pass == 1) { MethodSig ms; ms.name = fm->name; ms.ret = fm->ret; ms.params = fm->params; classes[a.sub].methods.push_back(ms); }
        finishGenerated(a, target.n, true, lines, {});
        E r;
        r.code = creationCode(a, {});
        r.pure = false;
        r.ty = target;
        return r;
    }

    // a name that is not a local, a field of the current class or a type: inside a generated class it
    // may be a captured local variable of the enclosing method or a member of the enclosing class
    bool resolveOuter(const std::string& name, E* res) {
        if (anonStack_.empty()) return false;
        AnonCtx& a = anonStack_.back();
        for (size_t i = a.outerScopes.size(); i-- > 0;) {
            auto it = a.outerScopes[i].find(name);
            if (it == a.outerScopes[i].end()) continue;
            bool have = false;
            for (auto& c : a.captures) if (c.first == it->second.first) have = true;
            if (!have) a.captures.push_back({it->second.first, it->second.second});
            res->code = "cap_" + it->second.first;
            res->ty = it->second.second;
            return true;
        }
        if (const FieldInfo* fi = findField(a.outerClass, name)) {
            if (fi->isStatic) res->code = a.outerClass + "." + safeName(name);
            else { if (a.outerStatic) return false; res->code = "outer_." + safeName(name); }
            res->ty = fi->ty;
            return true;
        }
        return false;
    }

    // `(` at i opens a record component list `(T a, U b) [implements ...] {` (and not a method called record)?
    bool scanRecordHeader(size_t i) const {
        int d = 0; size_t j = i;
        do { if (isOpAt(j, "(")) d++; else if (isOpAt(j, ")")) d--; j++; } while (d > 0 && at(j).kind != JT_EOF);
        return isOpAt(j, "{") || isKwAt(j, "implements");
    }

    // ---- record (JLS 8.10) -------------------------------------------------------------------
    // A record is lowered to a class with one field per component, the canonical constructor (a
    // compact constructor runs its body first), one accessor per component, and the generated
    // toString() "Name[a=1, b=2]" and, when every component is a primitive or a String,
    // equals(Object) and, for int-like, long, boolean and String components, hashCode().
    void parseRecord(const std::string& nestPrefix, const std::string& outerName, bool nestedStatic) {
        InitScope noHoist(this, nullptr);
        expectKw("record");
        const bool nested = !nestPrefix.empty();
        Saved sv = saveState();
        const std::string simpleName = ident();
        std::string name = nestPrefix + simpleName;
        typeVars.clear();
        std::vector<std::string> tparams;
        skipTypeParams(&tparams);
        curClass = name;
        curIsInterface = false;
        curSimple_ = nested ? simpleName : "";
        if (nested) {
            auto it = nestedMap_.find(simpleName);
            if (pass == 1) { if (it == nestedMap_.end()) nestedMap_[simpleName] = name; else if (it->second != name) it->second = "\x01"; }
        } else { topLevelName_ = name; anonCounter_ = 0; pendingTypes_.clear(); }
        (void)nestedStatic; (void)outerName;
        std::vector<std::pair<std::string, Ty>> comps = parseParams();
        const bool compVarargs = lastVarargs_;
        std::string header = "class " + name;
        if (!tparams.empty()) { header += "<"; for (size_t i = 0; i < tparams.size(); i++) header += (i ? ", " : "") + tparams[i]; header += ">"; }
        std::vector<std::string> ifaceNames;
        if (acceptKw("implements")) {
            std::vector<std::string> ifs;
            auto one = [&]() { Ty t = parseType(); ifs.push_back(t.n == "?" ? std::string("Any") : t.n); };
            one();
            while (acceptOp(",")) one();
            ifaceNames = ifs;
            header += " implements " + ifs[0];
            for (size_t i = 1; i < ifs.size(); i++) header += ", " + ifs[i];
        }
        expectOp("{");
        ClassInfo& ci = classes[name];
        if (pass == 1) {
            ci.fields.clear(); ci.methods.clear(); ci.super = ""; ci.ifaces = ifaceNames; ci.isInterface = false;
            for (auto& c : comps) { FieldInfo fi; fi.ty = c.second; ci.fields[c.first] = fi; MethodSig ms; ms.name = c.first; ms.ret = c.second; ci.methods.push_back(ms); }
            MethodSig canon; canon.name = "<init>"; canon.ret = mk("void"); canon.varargs = compVarargs; for (auto& c : comps) canon.params.push_back(c.second); ci.methods.push_back(canon);
            MethodSig ts; ts.name = "toString"; ts.ret = mk("String"); ci.methods.push_back(ts);
            // equals / hashCode exist exactly when the record lowering generates them (see below)
            bool simpleC = true, hashC = true;
            for (auto& c : comps) {
                const Ty& t = c.second;
                const bool pr = t.dims == 0 && (kPrim.count(t.n) > 0) && t.n != "float" && t.n != "double";
                const bool st = t.dims == 0 && t.n == "String";
                if (!pr && !st) simpleC = false;
                if (!(t.dims == 0 && (t.n == "int" || t.n == "short" || t.n == "byte" || t.n == "char" || t.n == "boolean" || t.n == "long" || st))) hashC = false;
            }
            if (simpleC) { MethodSig es; es.name = "equals"; es.ret = mk("boolean"); es.params.push_back(mk("Object")); ci.methods.push_back(es); }
            if (hashC) { MethodSig hs; hs.name = "hashCode"; hs.ret = mk("int"); ci.methods.push_back(hs); }
        }
        size_t beforeDiags = diags.size();
        std::vector<std::string> memberLines;
        int instanceFields = (int)comps.size();
        bool compact = false, canonicalSeen = false;
        std::string compactBody;
        std::set<std::string> declared;
        while (!isOp("}")) {
            if (atEof()) { restoreState(sv); fail("syntax", "unexpected end of file in record body"); }
            if (acceptOp(";")) continue;
            size_t start = p;
            out.clear(); ind = 1;
            try {
                // compact canonical constructor: Name { body }
                Mods mods = parseModifiers();
                if (isIdentTok(cur()) && (cur().text == simpleName || cur().text == name) && isOpAt(p + 1, "{")) {
                    p++;
                    scopes.clear(); usedNames.clear(); pushScope(); tmpCounter = 0; curRet = mk("void"); curIsStatic_ = false;
                    std::string plist = paramList(comps);
                    std::vector<std::string> saved; saved.swap(out);
                    ind = 2;
                    expectOp("{");
                    pushScope();
                    while (!isOp("}")) { if (atEof()) fail("syntax", "unexpected end of file in constructor"); parseStatement(); }
                    p++;
                    popScope();
                    std::vector<std::string> body; body.swap(out); out.swap(saved);
                    line(name + "(" + plist + ") {");
                    for (auto& l : body) out.push_back(l);
                    for (auto& c : comps) { std::string en = scopes[0].count(c.first) ? scopes[0][c.first].first : safeName(c.first); line("  this." + c.first + " = " + en + ";"); }
                    line("}");
                    compact = true; canonicalSeen = true;
                } else {
                    p = start;
                    if (isIdentTok(cur()) && false) {}
                    // an explicit constructor with the component types is the canonical one
                    size_t before = p;
                    (void)before;
                    parseMember(name, false, ci, &instanceFields);
                }
                for (auto& l : out) memberLines.push_back(l);
                (void)mods;
            } catch (const ConvErr& e) { diag(e); skipMember(start); }
        }
        expectOp("}");
        out.clear();
        for (auto& m : ci.methods) if (pass == 2 || true) declared.insert(m.name + "/" + std::to_string(m.params.size()));
        // an explicit canonical constructor is one declared with the same number of parameters as components
        int ctorCount = 0;
        for (auto& m : ci.methods) if (m.name == "<init>" && m.params.size() == comps.size()) ctorCount++;
        if (ctorCount > 1) canonicalSeen = true;   // the generated signature plus a declared one
        const bool userCanon = canonicalSeen;
        restoreState(sv);
        if (diags.size() != beforeDiags || pass != 2) { if (!nested) pendingTypes_.clear(); return; }
        if (instanceFields + 13 > 64) fail("model", "the record exceeds the 64-field class limit");
        // members that the user did not write
        auto declares = [&](const std::string& n, size_t nparams) {
            for (auto& l : memberLines) {
                std::string probe = " " + n + "(";
                size_t k = l.find(probe);
                if (k != std::string::npos && k < 14 && l.find('{') != std::string::npos) {
                    // count parameters in the signature line
                    size_t a = l.find('(', k), b = l.find(')', a);
                    std::string inside = l.substr(a + 1, b - a - 1);
                    size_t cnt = inside.empty() ? 0 : 1; for (char ch : inside) if (ch == ',') cnt++;
                    if (cnt == nparams) return true;
                }
            }
            return false;
        };
        std::ostringstream o;
        o << header << " {\n" << metaBlock(name) << "\n";
        for (auto& c : comps) o << "  " << emitType(c.second) << " " << c.first << ";\n";
        if (!userCanon) {
            std::string pl, as;
            for (size_t i = 0; i < comps.size(); i++) { pl += (i ? ", " : "") + emitType(comps[i].second) + " " + safeName(comps[i].first); as += "    this." + comps[i].first + " = " + safeName(comps[i].first) + ";\n"; }
            o << "  " << name << "(" << pl << ") {\n" << as << "  }\n";
        }
        for (auto& c : comps) if (!declares(c.first, 0)) o << "  " << emitType(c.second) << " " << c.first << "() { return " << c.first << "; }\n";
        if (!declares("toString", 0)) {
            std::string expr = "\"" + simpleName + "[\"";
            for (size_t i = 0; i < comps.size(); i++) {
                E fe; fe.code = comps[i].first; fe.ty = comps[i].second; fe.pure = true;
                expr += std::string(" + \"") + (i ? ", " : "") + comps[i].first + "=\" + " + strOf(fe);
            }
            expr += " + \"]\"";
            o << "  String toString() { return " << expr << "; }\n";
        }
        o << "  static String javaText(" << name << " x) {\n    if (x == null) { return \"null\"; }\n    return x.toString();\n  }\n";
        bool simpleComps = true, hashable = true;
        for (auto& c : comps) {
            const Ty& t = c.second;
            const bool prim = t.dims == 0 && (kPrim.count(t.n) > 0) && t.n != "float" && t.n != "double";
            const bool str = t.dims == 0 && t.n == "String";
            if (!prim && !str) simpleComps = false;
            if (!(t.dims == 0 && (t.n == "int" || t.n == "short" || t.n == "byte" || t.n == "char" || t.n == "boolean" || t.n == "long" || str))) hashable = false;
        }
        if (simpleComps && !declares("equals", 1)) {
            o << "  boolean equals(Any o) {\n    if (!(o instanceof " << name << ")) { return false; }\n    " << name << " q = (" << name << ") o;\n    return ";
            if (comps.empty()) o << "true"; 
            for (size_t i = 0; i < comps.size(); i++) o << (i ? " && " : "") << "(" << comps[i].first << " == q." << comps[i].first << ")";
            o << ";\n  }\n";
        }
        if (hashable && !declares("hashCode", 0)) {
            o << "  int hashCode() {\n    int h = 0;\n";
            for (auto& c : comps) {
                std::string hv;
                const Ty& t = c.second;
                if (t.n == "String") hv = "(" + c.first + " == null ? 0 : " + c.first + ".hashCode())";
                else if (t.n == "boolean") hv = "(" + c.first + " ? 1231 : 1237)";
                else if (t.n == "long") hv = "JavaOperands.toInt(" + c.first + " ^ (" + c.first + " >>> 32))";
                else hv = c.first;
                o << "    h = JavaOperands.addInt(JavaOperands.mulInt(31, h), " << hv << ");\n";
            }
            o << "    return h;\n  }\n";
        }
        for (auto& l : memberLines) o << l << "\n";
        o << "}\n";
        (void)compact;
        OutFile f; f.typeName = name; f.text = o.str();
        if (nested) { pendingTypes_.push_back(f); return; }
        files.push_back(f);
        for (auto& g : pendingTypes_) files.push_back(g);
        pendingTypes_.clear();
    }

    // ---- enum --------------------------------------------------------------------------------
    // An enum is lowered, as javac does, to a class: two hidden fields (name, ordinal), one static
    // instance per constant (a constant with a body is an instance of a generated subclass), a
    // static values array, and values()/valueOf()/name()/ordinal()/toString()/compareTo().
    // A bare constant name in a case label is resolved by parseCaseLabel.
    void parseEnum(const std::string& nestPrefix = "") {
        InitScope noHoist(this, nullptr);
        expectKw("enum");
        typeVars.clear();
        curIsInterface = false;
        const std::string simpleName = ident();
        std::string name = nestPrefix + simpleName;
        curClass = name;
        if (!nestPrefix.empty()) {
            auto it = nestedMap_.find(simpleName);
            if (pass == 1) { if (it == nestedMap_.end()) nestedMap_[simpleName] = name; else if (it->second != name) it->second = "\x01"; }
        } else { topLevelName_ = name; anonCounter_ = 0; pendingTypes_.clear(); }
        std::vector<std::string> ifaceNames;
        std::string header = "class " + name;
        if (acceptKw("implements")) {
            std::vector<std::string> ifs;
            auto one = [&]() { Ty t = parseType(); ifs.push_back(t.n == "?" ? std::string("Any") : t.n); };
            one();
            while (acceptOp(",")) one();
            ifaceNames = ifs;
            header += " implements " + ifs[0];
            for (size_t i = 1; i < ifs.size(); i++) header += ", " + ifs[i];
        }
        expectOp("{");
        ClassInfo& ci = classes[name];
        if (pass == 1) { ci.fields.clear(); ci.methods.clear(); ci.isEnum = true; ci.super = ""; ci.ifaces = ifaceNames; }
        size_t beforeDiags = diags.size();

        struct Const { std::string name; std::vector<E> args; std::string sub; std::vector<std::string> subLines; int subFields = 0; };
        std::vector<Const> consts;
        scopes.clear(); usedNames.clear(); pushScope(); curRet = mk("void"); tmpCounter = 0;
        while (isIdentTok(cur()) || isOp("@")) {
            while (isOp("@")) { p++; ident(); while (acceptOp(".")) ident(); if (isOp("(")) { int d = 0; do { if (isOp("(")) d++; else if (isOp(")")) d--; p++; } while (d > 0 && !atEof()); } }
            Const c;
            c.name = ident();
            if (isOp("(")) c.args = parseArgs();
            if (isOp("{")) {                    // constant-specific class body
                c.sub = name + "__" + c.name;
                ClassInfo& sci = classes[c.sub];
                if (pass == 1) { sci.fields.clear(); sci.methods.clear(); sci.super = name; sci.ifaces.clear(); }
                const std::string savedClass = curClass;
                curClass = c.sub;
                p++;
                while (!isOp("}")) {
                    if (atEof()) fail("syntax", "unexpected end of file in enum constant body");
                    size_t start = p;
                    out.clear(); ind = 1;
                    try { parseMember(c.sub, false, sci, &c.subFields); for (auto& l : out) c.subLines.push_back(l); }
                    catch (const ConvErr& e) { diag(e); skipMember(start); }
                }
                p++;
                curClass = savedClass;
            }
            consts.push_back(std::move(c));
            if (!acceptOp(",")) break;
        }
        bool hadMembers = acceptOp(";");
        std::vector<std::string> memberLines;
        int instanceFields = 0;
        if (pass == 1) {
            for (auto& c : consts) { FieldInfo fi; fi.ty = mk(name); fi.isStatic = true; ci.fields[c.name] = fi; }
            FieldInfo vf; vf.ty = mk(name, 1); vf.isStatic = true; ci.fields["enumValues"] = vf;
            auto addM = [&](const char* n, Ty r, std::vector<Ty> ps, bool st) { MethodSig m; m.name = n; m.ret = r; m.params = ps; m.isStatic = st; ci.methods.push_back(m); };
            addM("values", mk(name, 1), {}, true);
            addM("valueOf", mk(name), {mk("String")}, true);
            addM("name", mk("String"), {}, false);
            addM("ordinal", mk("int"), {}, false);
            addM("compareTo", mk("int"), {mk(name)}, false);
        }
        bool userToString = false, ctorSeen = false;
        enumMode_ = true;
        while (hadMembers || isOp("}") == false) {
            if (isOp("}")) break;
            if (atEof()) { enumMode_ = false; fail("syntax", "unexpected end of file in enum body"); }
            if (acceptOp(";")) continue;
            size_t start = p;
            out.clear(); ind = 1;
            if (isIdentTok(cur()) && cur().text == name && isOpAt(p + 1, "(")) ctorSeen = true;
            try { parseMember(name, false, ci, &instanceFields); for (auto& l : out) memberLines.push_back(l); }
            catch (const ConvErr& e) { diag(e); skipMember(start); }
        }
        enumMode_ = false;
        expectOp("}");
        out.clear();
        for (auto& m : ci.methods) if (m.name == "toString" && m.params.empty()) userToString = true;
        if (diags.size() != beforeDiags || pass != 2) return;
        if (instanceFields + 13 + 2 > 64)
            fail("model", "the enum has " + std::to_string(instanceFields) + " instance fields; with the metadata and name/ordinal fields it exceeds the 64-field SLeeLa class limit");

        auto ctorFor = [&](const std::vector<E>& args) -> const MethodSig* {
            const MethodSig* best = nullptr; int bestScore = -1;
            for (const auto& m : ci.methods) {
                if (m.name != "<init>" || m.params.size() != args.size()) continue;
                int sc = 0; for (size_t i = 0; i < args.size(); i++) if (m.params[i] == args[i].ty) sc++;
                if (sc > bestScore) { bestScore = sc; best = &m; }
            }
            return best;
        };
        std::ostringstream o;
        o << header << " {\n" << metaBlock(name) << "\n";
        o << "  String enumName;\n  int enumOrdinal;\n";
        for (auto& l : memberLines) o << l << "\n";
        if (!ctorSeen) o << "  " << name << "(String enumName_, int enumOrd_) {\n    this.enumName = enumName_;\n    this.enumOrdinal = enumOrd_;\n  }\n";
        int ord = 0;
        for (auto& c : consts) {
            const MethodSig* m = ctorFor(c.args);
            if (!c.args.empty() && !m) fail("model", "no constructor of enum " + name + " takes " + std::to_string(c.args.size()) + " argument(s)");
            std::string argText;
            for (size_t i = 0; i < c.args.size(); i++) {
                E a = m ? coerce(c.args[i], m->params[i]) : c.args[i];
                argText += ", " + a.code;
            }
            o << "  static " << name << " " << c.name << " = new " << (c.sub.empty() ? name : c.sub) << "(\"" << c.name << "\", " << ord << argText << ");\n";
            ord++;
        }
        o << "  static " << name << "[] enumValues = {";
        for (size_t i = 0; i < consts.size(); i++) o << (i ? ", " : "") << consts[i].name;
        o << "};\n";
        o << "  static " << name << "[] values() {\n    " << name << "[] r = new " << name << "[arrayLength(enumValues)];\n"
          << "    for (int i = 0; i < arrayLength(enumValues); i++) { r[i] = enumValues[i]; }\n    return r;\n  }\n";
        o << "  static " << name << " valueOf(String n) {\n    for (int i = 0; i < arrayLength(enumValues); i++) { " << name << " e = enumValues[i]; if (e.name() == n) { return e; } }\n"
          << "    throw new IllegalArgumentException(\"No enum constant " << name << ".\" + n);\n  }\n";
        o << "  static String javaText(" << name << " x) {\n    if (x == null) { return \"null\"; }\n    return x.toString();\n  }\n";
        o << "  String name() { return enumName; }\n  int ordinal() { return enumOrdinal; }\n";
        if (!userToString) o << "  String toString() { return enumName; }\n";
        o << "  int compareTo(" << name << " other) { return enumOrdinal - other.ordinal(); }\n";
        o << "}\n";
        OutFile f; f.typeName = name; f.text = o.str();
        if (nestPrefix.empty()) files.push_back(f); else pendingTypes_.push_back(f);
        for (auto& c : consts) {
            if (c.sub.empty()) continue;
            const MethodSig* m = ctorFor(c.args);
            std::string params, supers;
            if (m) for (size_t i = 0; i < m->params.size(); i++) { params += ", " + emitType(m->params[i]) + " a" + std::to_string(i); supers += ", a" + std::to_string(i); }
            std::ostringstream so;
            so << "class " << c.sub << " extends " << name << " {\n" << metaBlock(c.sub) << "\n";
            so << "  " << c.sub << "(String enumName_, int enumOrd_" << params << ") {\n    super(enumName_, enumOrd_" << supers << ");\n  }\n";
            for (auto& l : c.subLines) so << l << "\n";
            so << "}\n";
            OutFile sf; sf.typeName = c.sub; sf.text = so.str();
            files.push_back(sf);
        }
    }

    // ---- switch (statement and expression) -------------------------------------------------
    // A switch statement is emitted as a native SLeeLa switch: the compiler lowers it in source order
    // with Java fallthrough, a default anywhere, and break/continue (SLEELA.syntax 44.4). Arrow cases
    // become a case followed by an implicit break. A switch expression is hoisted: a temporary is
    // declared, a native switch assigns it in each arm, and the temporary is the value.
    // Java member names that collide with SLeeLa built-ins are renamed consistently in declarations and calls.
    // close() is the Munction reach-handle built-in, so the Java tier calls it closeResource().
    static std::string mapMember(const std::string& n) { return n == "close" ? "closeResource" : n; }
    bool lastVarargs_ = false;
    std::string yieldVar_;
    Ty yieldTy_;
    bool yieldTyKnown_ = false;
    size_t yieldBase_ = 0;
    size_t yieldDeclIdx_ = 0;

    bool isEnumType(const std::string& n) const {
        auto it = classes.find(n);
        return it != classes.end() && it->second.isEnum;
    }

    std::string parseCaseLabel(const Ty& selTy) {
        if (isKw("null")) unsup("switch-expr");
        // enum constant: a bare name resolved in the selector's enum type
        if (isIdentTok(cur()) && (isOpAt(p + 1, ",") || isOpAt(p + 1, ":") || isOpAt(p + 1, "->")) && selTy.dims == 0 && isEnumType(selTy.n)) {
            std::string c = ident();
            return selTy.n + "." + c;
        }
        // type patterns (case Circle c ->) are not converted
        if (isIdentTok(cur()) && isIdentTok(at(p + 1))) unsup("switch-expr");
        E lab = parseTernary();
        std::string cls = classOf(selTy);
        if (cls == "text" && !(lab.ty.dims == 0 && lab.ty.n == "String")) unsup("untyped");
        return lab.code;
    }

    // Parses '{' cases '}' emitting native switch lines. resultVar non-empty => switch expression.
    void parseSwitchBody(const Ty& selTy, const std::string& resultVar) {
        expectOp("{");
        pushScope();
        breakCtx.push_back('W');
        while (!isOp("}")) {
            if (atEof()) fail("syntax", "unexpected end of file in switch");
            std::vector<std::string> labels;
            bool arrow = false;
            if (acceptKw("default")) {
                labels.push_back("default:");
            } else {
                expectKw("case");
                do { labels.push_back("case " + parseCaseLabel(selTy) + ":"); } while (acceptOp(","));
            }
            if (acceptOp("->")) arrow = true; else expectOp(":");
            // old-style stacked labels: case 1: case 2: body
            while (!arrow && (isKw("case") || isKw("default"))) {
                if (acceptKw("default")) labels.push_back("default:");
                else { p++; do { labels.push_back("case " + parseCaseLabel(selTy) + ":"); } while (acceptOp(",")); }
                if (isOp("->")) fail("syntax", "cannot mix ':' and '->' case labels");
                expectOp(":");
            }
            for (auto& l : labels) line(l);
            ind++;
            if (arrow) {
                if (isOp("{")) {
                    parseBlockInto();
                } else if (isKw("throw")) {
                    parseStatement();
                } else {
                    if (!resultVar.empty()) {
                        E v = parseExpr();
                        noteYieldType(v.ty);
                        v = coerce(v, yieldTyKnown_ ? yieldTy_ : v.ty);
                        line(resultVar + " = " + v.code + ";");
                    } else {
                        std::string st = parseStatementExpr();
                        line(st + ";");
                    }
                    expectOp(";");
                }
                if (!(isKwAt(p - 1, "throw"))) line("break;");
            } else {
                while (!isKw("case") && !isKw("default") && !isOp("}")) {
                    if (atEof()) fail("syntax", "unexpected end of file in switch");
                    parseStatement();
                }
            }
            ind--;
        }
        expectOp("}");
        breakCtx.pop_back();
        popScope();
    }

    void noteYieldType(const Ty& t) {
        if (!yieldTyKnown_ && t.known() && t.n != "null") { yieldTy_ = t; yieldTyKnown_ = true; }
    }

    void parseSwitch() {
        expectKw("switch"); expectOp("(");
        allowHoist_ = false;
        E sel = parseExpr();
        expectOp(")");
        std::string cls = classOf(sel.ty);
        if (cls == "unknown" || cls == "double" || cls == "long" || cls == "boolean") unsup("untyped");
        line("switch (" + sel.code + ") {");
        std::string savedYield = yieldVar_; yieldVar_.clear();
        parseSwitchBody(sel.ty, "");
        yieldVar_ = savedYield;
        line("}");
    }

    E parseSwitchExpr() {
        if (!allowHoist_ || p != hoistPos_) unsup("switch-expr");
        expectKw("switch"); expectOp("(");
        allowHoist_ = false;
        E sel = parseExpr();
        expectOp(")");
        std::string cls = classOf(sel.ty);
        if (cls == "unknown" || cls == "double" || cls == "long" || cls == "boolean") unsup("untyped");
        std::string tmp = freshTmp("swv");
        // result type: the declared target when the statement gives one, else the first arm
        const std::string savedVar = yieldVar_; const Ty savedTy = yieldTy_; const bool savedKnown = yieldTyKnown_; const size_t savedBase = yieldBase_; const size_t savedIdx = yieldDeclIdx_;
        yieldVar_ = tmp;
        yieldTy_ = hoistTy_; yieldTyKnown_ = hoistTy_.known() && hoistTy_.n != "void";
        yieldDeclIdx_ = out.size();
        line("<TYPE> " + tmp + ";");
        line("switch (" + sel.code + ") {");
        yieldBase_ = breakCtx.size() + 1;   // parseSwitchBody pushes one 'W'
        parseSwitchBody(sel.ty, tmp);
        line("}");
        Ty resTy = yieldTyKnown_ ? yieldTy_ : mk("?");
        std::string decl = emitType(resTy) + " " + tmp + ";";
        out[yieldDeclIdx_] = std::string((size_t)ind * 2, ' ') + decl;
        yieldVar_ = savedVar; yieldTy_ = savedTy; yieldTyKnown_ = savedKnown; yieldBase_ = savedBase; yieldDeclIdx_ = savedIdx;
        E r;
        r.code = tmp;
        r.ty = resTy;
        r.pure = true;
        return r;
    }

    // ---------------------------------------------------------- members ----
    struct Mods { std::set<std::string> set; bool has(const char* s) const { return set.count(s) > 0; } };

    Mods parseModifiers() {
        Mods m;
        for (;;) {
            if (isOp("@") && !isKwAt(p + 1, "interface")) {
                p++; ident();
                while (isOp(".")) { p++; ident(); }
                if (isOp("(")) { int d = 0; do { if (isOp("(")) d++; else if (isOp(")")) d--; p++; } while (d > 0 && !atEof()); }
                continue;
            }
            if (isIdentTok(cur()) && kModifiers.count(cur().text)) {
                // 'default' as a modifier only inside interfaces and only when followed by a type
                if (cur().text == "default" && !curIsInterface) break;
                m.set.insert(cur().text);
                p++;
                continue;
            }
            if (isKw("non") && isOpAt(p + 1, "-") && isKwAt(p + 2, "sealed")) { p += 3; continue; }
            break;
        }
        return m;
    }

    void skipTypeParams(std::vector<std::string>* names) {
        if (!isOp("<")) return;
        int d = 0;
        bool expectName = true;
        do {
            if (isOp("<")) { d++; expectName = (d == 1); }
            else if (isOp(">")) d--;
            else if (isOp(">>")) d -= 2;
            else if (isOp(",")) { if (d == 1) expectName = true; }
            else if (isIdentTok(cur()) && expectName && d == 1) { if (names) names->push_back(cur().text); typeVars.insert(cur().text); expectName = false; }
            p++;
        } while (d > 0 && !atEof());
    }

    void skipMember(size_t start) {
        size_t i = start;
        int braces = 0;
        while (at(i).kind != JT_EOF) {
            const Tok& t = at(i);
            if (t.kind == JT_OP) {
                if (t.text == "{") braces++;
                else if (t.text == "}") {
                    if (braces == 0) break;           // class end, do not consume
                    braces--;
                    if (braces == 0) {
                        i++;
                        if (isOpAt(i, ";") || isOpAt(i, ",")) { while (at(i).kind != JT_EOF && !isOpAt(i, ";")) i++; if (isOpAt(i, ";")) i++; }
                        break;
                    }
                } else if (t.text == ";" && braces == 0) { i++; break; }
            }
            i++;
        }
        p = i;
    }

    void diag(const ConvErr& e) {
        if (pass != 2) return;
        Diagnostic d; d.line = e.line; d.col = e.col; d.construct = e.construct; d.message = e.msg;
        diags.push_back(d);
    }

    void parseUnit() {
        // package / imports
        size_t guard = 0;
        try {
            if (isKw("package")) { while (!isOp(";") && !atEof()) p++; p++; }
            while (isKw("import")) { while (!isOp(";") && !atEof()) p++; p++; }
        } catch (const ConvErr& e) { diag(e); return; }
        while (!atEof()) {
            if (acceptOp(";")) continue;
            size_t before = p;
            try {
                parseTypeDecl();
            } catch (const ConvErr& e) {
                diag(e);
                // recover to the end of this type
                p = before;
                int braces = 0;
                bool seen = false;
                while (!atEof()) {
                    if (isOp("{")) { braces++; seen = true; }
                    else if (isOp("}")) { braces--; if (seen && braces <= 0) { p++; break; } }
                    p++;
                }
            }
            if (++guard > 100000) break;
        }
    }

    std::string metaBlock(const std::string& typeName) const {
        std::ostringstream o;
        o << "  < ---- Descriptive metadata (version 1.11 nested-friendly accessors) ---- >\n"
          << "  << Values describe the CONVERTED artifact. Author and legal fields are\n"
          << "     unknown to the converter and are stated as unknown, not invented. >>\n"
          << "  String metaVersion = \"1.0.0\";\n"
          << "  int metaId = 0;\n"
          << "  String metaAuthor = \"" << opt.author << "\";\n"
          << "  String metaDate = \"unknown\";\n"
          << "  String metaCopyright = \"unknown\";\n"
          << "  String metaTrademark = \"none\";\n"
          << "  String metaCorporation = \"unknown\";\n"
          << "  String metaOrigin = \"unknown\";\n"
          << "  String metaBirthDate = \"unknown\";\n"
          << "  int metaScienceRating = 5;\n"
          << "  String metaNationalSchedule = \"none\";\n\n"
          << "  String getMetaVersion() { return metaVersion; }\n"
          << "  void setMetaVersion(String v) { metaVersion = v; }\n"
          << "  int getMetaId() { return metaId; }\n"
          << "  void setMetaId(int v) { metaId = v; }\n"
          << "  String getAuthor() { return metaAuthor; }\n"
          << "  void setAuthor(String v) { metaAuthor = v; }\n"
          << "  String getDate() { return metaDate; }\n"
          << "  void setDate(String v) { metaDate = v; }\n"
          << "  String getCopyright() { return metaCopyright; }\n"
          << "  void setCopyright(String v) { metaCopyright = v; }\n"
          << "  String getTrademark() { return metaTrademark; }\n"
          << "  void setTrademark(String v) { metaTrademark = v; }\n"
          << "  String getCorporation() { return metaCorporation; }\n"
          << "  void setCorporation(String v) { metaCorporation = v; }\n"
          << "  String getOrigin() { return metaOrigin; }\n"
          << "  void setOrigin(String v) { metaOrigin = v; }\n"
          << "  String getBirthDate() { return metaBirthDate; }\n"
          << "  void setBirthDate(String v) { metaBirthDate = v; }\n"
          << "  int getScienceRating() { return metaScienceRating; }\n"
          << "  void setScienceRating(int v) { metaScienceRating = v; }\n"
          << "  String getNationalSchedule() { return metaNationalSchedule; }\n"
          << "  void setNationalSchedule(String v) { metaNationalSchedule = v; }\n\n"
          << "  String javaType = \"" << typeName << "\";\n"
          << "  String javaSource = \"" << opt.sourceName << "\";\n";
        return o.str();
    }

    void parseTypeDecl(const std::string& nestPrefix = "", bool nestedStatic = true, bool modsConsumed = false) {
        const bool nested = !nestPrefix.empty();
        Saved nestedSaved = saveState();
        const std::string outerName = curClass;
        const bool savedInner = innerMode_, savedCtorSeen = innerCtorSeen_;
        const std::string savedSimple = curSimple_;
        struct Restorer { Translator* t; const Saved& sv; bool nested; bool inner, ctor; std::string simple;
            ~Restorer() { if (nested) { t->restoreState(sv); t->innerMode_ = inner; t->innerCtorSeen_ = ctor; t->curSimple_ = simple; if (!t->anonStack_.empty() && t->anonStack_.back().sub.rfind("#nest", 0) == 0) t->anonStack_.pop_back(); } } }
            restorer{this, nestedSaved, nested, savedInner, savedCtorSeen, savedSimple};
        typeVars.clear();
        curIsInterface = false;
        Mods mods = modsConsumed ? Mods() : parseModifiers();
        bool isIface = false;
        if (isKw("class")) p++;
        else if (isKw("interface")) { p++; isIface = true; }
        else if (isKw("enum")) { parseEnum(nestPrefix); return; }
        else if (isKw("record") && isIdentTok(at(p + 1))) { parseRecord(nestPrefix, outerName, nestedStatic); return; }
        else if (isKw("record")) unsup("nested");
        else if (isOp("@")) unsup("nested");
        else fail("syntax", "expected a class or interface declaration but found '" + descr() + "'");
        curIsInterface = isIface;
        const std::string simpleName = ident();
        std::string name = nestPrefix + simpleName;
        curClass = name;
        curSimple_ = nested ? simpleName : "";
        if (nested) {
            auto it = nestedMap_.find(simpleName);
            if (pass == 1) { if (it == nestedMap_.end()) nestedMap_[simpleName] = name; else if (it->second != name) it->second = "\x01"; }
            AnonCtx ac; ac.sub = "#nest" + name; ac.outerClass = outerName; ac.outerStatic = nestedStatic; 
            anonStack_.push_back(ac);
            innerMode_ = !nestedStatic && !isIface;
            innerCtorSeen_ = false;
        } else {
            topLevelName_ = name; anonCounter_ = 0; pendingTypes_.clear(); innerMode_ = false;
        }
        std::vector<std::string> tparams;
        skipTypeParams(&tparams);
        std::string header = std::string(isIface ? "interface " : "class ") + name;
        if (!tparams.empty()) {
            header += "<";
            for (size_t i = 0; i < tparams.size(); i++) { if (i) header += ", "; header += tparams[i]; }
            header += ">";
        }
        auto simple = [&](const Ty& t) { return t.n == "?" ? std::string("Any") : t.n; };
        std::string superName;
        std::vector<std::string> ifaceNames;
        if (acceptKw("extends")) {
            std::vector<std::string> bases;
            bases.push_back(simple(parseType()));
            while (acceptOp(",")) bases.push_back(simple(parseType()));
            if (isIface) {
                header += " extends " + bases[0];
                for (size_t i = 1; i < bases.size(); i++) header += ", " + bases[i];
                ifaceNames = bases;
            } else { header += " extends " + bases[0]; superName = bases[0]; }
        }
        if (acceptKw("implements")) {
            std::vector<std::string> ifs;
            ifs.push_back(simple(parseType()));
            while (acceptOp(",")) ifs.push_back(simple(parseType()));
            for (auto& x : ifs) ifaceNames.push_back(x);
            header += " implements " + ifs[0];
            for (size_t i = 1; i < ifs.size(); i++) header += ", " + ifs[i];
        }
        if (acceptKw("permits")) {   // sealed is a compile-time restriction only: no runtime behaviour to convert
            parseType();
            while (acceptOp(",")) parseType();
        }
        expectOp("{");
        ClassInit cinit;
        InitScope hoistScope(this, isIface ? nullptr : &cinit);
        ClassInfo& ci = classes[name];
        if (pass == 1) { ci.fields.clear(); ci.methods.clear(); ci.super = superName; ci.ifaces = ifaceNames; ci.isInterface = isIface; ci.innerOf = innerMode_ ? outerName : ""; }
        std::vector<std::string> memberLines;
        int instanceFields = 0;
        size_t beforeDiags = diags.size();
        while (!isOp("}")) {
            if (atEof()) fail("syntax", "unexpected end of file in class body");
            if (acceptOp(";")) continue;
            size_t start = p;
            out.clear();
            ind = 1;
            try {
                parseMember(name, isIface, ci, &instanceFields);
                for (auto& l : out) memberLines.push_back(l);
            } catch (const ConvErr& e) {
                diag(e);
                skipMember(start);
            }
        }
        p++;
        out.clear();
        if (diags.size() != beforeDiags || pass != 2) { if (!nested) pendingTypes_.clear(); return; }
        if (instanceFields + 13 > 64)
            fail("model", "the type has " + std::to_string(instanceFields) + " instance fields; with the 13 metadata fields it exceeds the 64-field SLeeLa class limit");
        std::ostringstream o;
        o << header << " {\n";
        if (!isIface) o << metaBlock(name) << "\n";
        else o << "  < interface: no metadata fields (an interface holds no instance state) >\n\n";
        if (!isIface) {
            bool declares = false;
            for (const auto& m : ci.methods) if (m.name == "toString" && m.params.empty()) declares = true;
            if (declares) o << "  static String javaText(" << name << " x) {\n    if (x == null) { return \"null\"; }\n    return x.toString();\n  }\n";
        }
        const std::string initCall = "javaFieldInit_" + name + "();";
        const bool hasInits = !cinit.inits.empty();
        if (innerMode_) {
            o << "  " << outerName << " outer_;\n";
            if (!innerCtorSeen_) o << "  " << name << "(" << outerName << " outerArg_) {\n    this.outer_ = outerArg_;\n" << (hasInits ? "    " + initCall + "\n" : "") << "  }\n";
        } else if (hasInits && !cinit.ctorSeen) {
            o << "  " << name << "() {\n    " << initCall << "\n  }\n";   // the implicit default constructor
        }
        if (hasInits) {
            o << "  void javaFieldInit_" << name << "() {\n";
            for (auto& a : cinit.inits) o << a << "\n";
            o << "  }\n";
        }
        for (auto& l : memberLines) {
            if (l.find(kInitMarker) != std::string::npos) { if (hasInits) o << "    " << initCall << "\n"; continue; }
            o << l << "\n";
        }
        o << "}\n";
        OutFile f;
        f.typeName = name;
        f.text = o.str();   // body only; the prologue is added by assemble()
        if (nested) { pendingTypes_.push_back(f); return; }
        files.push_back(f);
        for (auto& g : pendingTypes_) files.push_back(g);
        pendingTypes_.clear();
    }

    std::string prologue() const {
        std::ostringstream o;
        o << "<< Converted from Java by sleelactl java-convert (model java-to-sleela " << (M.rev ? M.rev : "?") << ").\n"
          << "   Java source: " << opt.sourceName << "\n"
          << "   Presumed JDK version: " << opt.jdkVersion << "\n"
          << "   Functional equivalence is the goal: operators follow lib/java/JavaOperands and the\n"
          << "   conversion subject model; library members resolve through the lib/java tier. >>\n"
          << "#sleela 3.1\n"
          << "import java.jdk-" << opt.jdkVersion << ";\n"
          << "material;\n\n";
        return o.str();
    }

public:
    void assemble(std::vector<OutFile>& v) const {
        if (opt.bundle && !v.empty()) {
            // superclasses and interfaces first, so every reference points backwards
            OutFile b;
            b.typeName = opt.bundleName.empty() ? v.back().typeName : opt.bundleName;
            b.text = prologue();
            for (const auto& f : v) b.text += f.text + "\n";
            v.assign(1, b);
            return;
        }
        for (auto& f : v) f.text = prologue() + f.text;
    }
private:

    void parseMember(const std::string& cls, bool isIface, ClassInfo& ci, int* instanceFields) {
        Mods mods = parseModifiers();
        if (isOp("{")) {                                      // initializer block
            if (!classInit_ || mods.has("static")) unsup("init-block");
            // An instance initializer runs with the field initializers, in source order (JLS 8.6): it
            // joins the per-class initializer method instead of being placed in the class body.
            scopes.clear(); usedNames.clear(); pushScope(); tmpCounter = 0; curRet = mk("void"); curIsStatic_ = false;
            const size_t outBefore = out.size();
            ind = 2;
            line("{");
            ind = 3;
            expectOp("{");
            pushScope();
            while (!isOp("}")) { if (atEof()) fail("syntax", "unexpected end of file in initializer block"); parseStatement(); }
            p++;
            popScope();
            ind = 2;
            line("}");
            ind = 1;
            for (size_t i = outBefore; i < out.size(); i++) classInit_->inits.push_back(out[i]);
            out.resize(outBefore);
            return;
        }
        if (isKw("class") || isKw("interface") || isKw("enum")) {   // a nested type is hoisted to Outer__Name
            if (!anonStack_.empty() && anonStack_.back().sub == cls && !innerMode_ && false) unsup("nested");
            bool st = mods.has("static") || isKw("interface") || isKw("enum") || curIsInterface;
            parseTypeDecl(cls + "__", st, true);
            return;
        }
        if (isKw("record") && isIdentTok(at(p + 1)) && (isOpAt(p + 2, "(") || isOpAt(p + 2, "<")) && (isOpAt(p + 2, "<") || scanRecordHeader(p + 2))) {
            parseTypeDecl(cls + "__", true, true);
            return;
        }
        if (isOp("@")) unsup("member-annotation");
        if (mods.has("synchronized") && !opt.singleThread) unsup("synchronized");
        if (mods.has("volatile") && !opt.singleThread) fail("volatile", "volatile requires the Java memory model; convert with --single-thread to treat it as a plain field");
        if (mods.has("native")) fail("native", "native methods have no Java body to convert");

        std::vector<std::string> mtp;
        std::set<std::string> savedVars = typeVars;
        skipTypeParams(&mtp);

        bool isStatic = mods.has("static");
        bool isAbstract = mods.has("abstract");
        curIsStatic_ = isStatic;

        // constructor
        if (isIdentTok(cur()) && (cur().text == cls || (!curSimple_.empty() && cur().text == curSimple_)) && isOpAt(p + 1, "(")) {
            p++;
            innerCtorSeen_ = true;
            scopes.clear(); usedNames.clear(); pushScope(); tmpCounter = 0;
            std::vector<std::pair<std::string, Ty>> params = parseParams();
            const bool ctorVarargs = lastVarargs_;
            if (isKw("throws")) skipThrows();
            curRet = mk("void");
            if (pass == 1) {
                MethodSig ms; ms.name = "<init>"; ms.ret = mk("void"); ms.varargs = ctorVarargs;
                for (auto& pr : params) ms.params.push_back(pr.second);
                ci.methods.push_back(ms);
            }
            std::string plist = paramList(params);
            std::string sig = enumMode_ ? cls + "(String enumName_, int enumOrd_" + (plist.empty() ? "" : ", " + plist) + ")" : cls + "(" + plist + ")";
            if (innerMode_) sig = cls + "(" + anonStack_.back().outerClass + " outerArg_" + (plist.empty() ? "" : ", " + plist) + ")";
            line(sig + " {");
            ind = 2;
            if (innerMode_) line("this.outer_ = outerArg_;");
            if (enumMode_) {
                if (isOp("{") && isKwAt(p + 1, "this") && isOpAt(p + 2, "(")) fail("enum-this-ctor", "an enum constructor that delegates with this(...) is not converted");
                line("this.enumName = enumName_;");
                line("this.enumOrdinal = enumOrd_;");
            }
            expectOp("{");
            pushScope();
            if (classInit_) {
                classInit_->ctorSeen = true;
                // this(...) delegates: the delegate runs the initializers. Otherwise they run right after
                // an explicit super(...), or first when the call is implicit.
                if (isKw("this") && isOpAt(p + 1, "(")) { /* none */ }
                else if (isKw("super") && isOpAt(p + 1, "(")) { parseStatement(); line(kInitMarker); }
                else line(kInitMarker);
            }
            while (!isOp("}")) { if (atEof()) fail("syntax", "unexpected end of file in constructor"); parseStatement(); }
            p++;
            popScope();
            ind = 1;
            line("}");
            typeVars = savedVars;
            return;
        }

        Ty ty = isKw("void") ? (p++, mk("void")) : parseType();
        std::string name = ident();

        if (isOp("(")) {
            if (kSleelaReserved.count(name) && name != "close")
                fail("reserved-member", "a method named '" + name + "' collides with a SLeeLa reserved word or built-in");
            scopes.clear(); usedNames.clear(); pushScope(); tmpCounter = 0;
            std::vector<std::pair<std::string, Ty>> params = parseParams();
            const bool methodVarargs = lastVarargs_;
            while (isOp("[") && isOpAt(p + 1, "]")) { p += 2; ty.dims++; }
            if (isKw("throws")) skipThrows();
            curRet = ty;
            if (pass == 1) {
                MethodSig ms; ms.name = name; ms.ret = ty; ms.isStatic = isStatic; ms.varargs = methodVarargs; ms.isAbstract = isOp(";");
                for (auto& pr : params) ms.params.push_back(pr.second);
                ci.methods.push_back(ms);
            }
            bool isMain = isStatic && name == "main" && params.size() == 1 && params[0].second.n == "String" && params[0].second.dims == 1;
            std::string head;
            if (isStatic && !isMain) head += "static ";
            if (isAbstract) head += "abstract ";
            std::string rt = emitType(ty);
            if (isMain) {
                head = "";
                line("static void main() {");
                ind = 2;
                expectOp("{");
                pushScope();
                std::string an = declare(params[0].first, params[0].second);
                line("String[] " + an + " = new String[0];");
                while (!isOp("}")) { if (atEof()) fail("syntax", "unexpected end of file in method"); parseStatement(); }
                p++;
                popScope();
                ind = 1;
                line("}");
                typeVars = savedVars;
                return;
            }
            std::string sig = head + rt + " " + mapMember(name) + "(" + paramList(params) + ")";
            if (isOp(";")) {
                p++;
                if (!isIface && !isAbstract) fail("native", "method without a body that is not abstract or an interface member");
                line(sig + ";");
                typeVars = savedVars;
                return;
            }
            line(sig + " {");
            ind = 2;
            expectOp("{");
            pushScope();
            while (!isOp("}")) { if (atEof()) fail("syntax", "unexpected end of file in method"); parseStatement(); }
            p++;
            popScope();
            ind = 1;
            line("}");
            typeVars = savedVars;
            return;
        }

        // field(s)
        for (;;) {
            Ty ft = ty;
            while (isOp("[") && isOpAt(p + 1, "]")) { p += 2; ft.dims++; }
            if (kSleelaReserved.count(name))
                fail("reserved-member", "a field named '" + name + "' collides with a SLeeLa reserved word");
            FieldInfo fi; fi.ty = ft; fi.isStatic = isStatic || isIface;
            if (pass == 1) ci.fields[name] = fi;
            std::string decl = std::string((isStatic || isIface) ? "static " : "") + emitType(ft) + " " + name;
            // JLS 4.12.5: a field starts at the default value of its type. SLeeLa leaves it null, which
            // prints as "null", breaks the integer laws, and turns an unset instance String into "".
            std::string dflt = " = null";
            if (ft.dims == 0) {
                const std::string& tn = ft.n;
                if (tn == "int" || tn == "long" || tn == "short" || tn == "byte" || tn == "char") dflt = " = 0";
                else if (tn == "double" || tn == "float") dflt = " = 0.0";
                else if (tn == "boolean") dflt = " = false";
            }
            const bool hoistable = classInit_ && !(isStatic || isIface);
            bool hoisted = false;
            if (acceptOp("=")) {
                scopes.clear(); usedNames.clear(); pushScope(); tmpCounter = 0; curRet = mk("void");
                const size_t outBefore = out.size();
                std::string init;
                if (isOp("{")) {
                    init = parseArrayInit(ft);
                } else {
                    lambdaTarget_ = ft;
                    E v = parseExpr();
                    v = coerce(v, ft);
                    init = v.code;
                }
                // an initializer that needed extra statements (temporaries) stays where it is
                if (hoistable && out.size() == outBefore) { classInit_->inits.push_back("    this." + name + " = " + init + ";"); decl += dflt; hoisted = true; }
                else decl += " = " + init;
            } else {
                decl += dflt;
            }
            (void)hoisted;
            line(decl + ";");
            if (!(isStatic || isIface)) (*instanceFields)++;
            if (acceptOp(",")) { name = ident(); continue; }
            break;
        }
        expectOp(";");
        typeVars = savedVars;
    }

    std::vector<std::pair<std::string, Ty>> parseParams() {
        std::vector<std::pair<std::string, Ty>> ps;
        lastVarargs_ = false;
        expectOp("(");
        if (!isOp(")")) {
            do {
                while (isKw("final") || isOp("@")) {
                    if (isOp("@")) { p++; ident(); if (isOp("(")) { int d = 0; do { if (isOp("(")) d++; else if (isOp(")")) d--; p++; } while (d > 0 && !atEof()); } }
                    else p++;
                }
                Ty t = parseType();
                if (isOp("...")) {   // variable arity: the parameter is an array, call sites pack the extra arguments
                    p++;
                    t.dims++;
                    lastVarargs_ = true;
                }
                std::string n = ident();
                while (isOp("[") && isOpAt(p + 1, "]")) { p += 2; t.dims++; }
                ps.push_back({n, t});
            } while (acceptOp(","));
        }
        expectOp(")");
        return ps;
    }
    std::string paramList(const std::vector<std::pair<std::string, Ty>>& ps) {
        std::string s;
        for (size_t i = 0; i < ps.size(); i++) {
            if (i) s += ", ";
            std::string en = declare(ps[i].first, ps[i].second);
            s += emitType(ps[i].second) + " " + en;
        }
        return s;
    }
    void skipThrows() {
        expectKw("throws");
        parseType();
        while (acceptOp(",")) parseType();
    }
};

}  // namespace

namespace {
bool lexInto(const std::string& src, std::vector<Tok>& v, Diagnostic* d) {
    jtokens toks;
    char err[256];
    if (jlex(src.c_str(), src.size(), &toks, err, sizeof err) != 0) {
        d->construct = "syntax";
        d->message = err;
        sscanf(err, "%d:%d", &d->line, &d->col);
        jtokens_free(&toks);
        return false;
    }
    for (size_t i = 0; i < toks.n; i++) {
        Tok t;
        t.kind = toks.v[i].kind; t.text = toks.v[i].text ? toks.v[i].text : "";
        t.ival = toks.v[i].ival; t.line = toks.v[i].line; t.col = toks.v[i].col;
        v.push_back(t);
    }
    jtokens_free(&toks);
    return true;
}
}  // namespace

Result convert(const std::string& src, const cm_model& model, const Options& opt, SymbolTable* shared) {
    Result r;
    std::vector<Tok> v;
    Diagnostic d;
    if (!lexInto(src, v, &d)) { r.diagnostics.push_back(d); return r; }
    SymbolTable local;
    SymbolTable& tab = shared ? *shared : local;
    Translator tr(model, opt, v, tab, shared != nullptr);
    r = tr.run();
    tr.assemble(r.files);
    return r;
}

void collect(const std::string& src, const cm_model& model, const Options& opt, SymbolTable& table) {
    std::vector<Tok> v;
    Diagnostic d;
    if (!lexInto(src, v, &d)) return;
    Translator tr(model, opt, v, table, false);
    tr.run(true);
}

}  // namespace javaconv
