// ===========================================================================
// xclass_loader.cpp -- .xclass (SecureJDK 28) ingestion into a sleela::Program.
// ===========================================================================
#include "xclass_loader.h"

#include <cctype>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>

namespace sleela {
namespace xclass {

// ---------------------------------------------------------------------------
// A tiny, dependency-free XML scanner sufficient for the well-formed .xclass
// schema (elements, attributes, text, self-closing tags; comments + the XML
// declaration are skipped). It is NOT a general XML parser: it rejects DOCTYPE
// so entity-expansion tricks cannot appear (matching the SecureJDK config's
// reject-dtd posture).
// ---------------------------------------------------------------------------
namespace {

struct Node {
    std::string tag;
    std::map<std::string, std::string> attrs;
    std::string text;                 // concatenated direct text
    std::vector<Node> children;

    const Node* child(const std::string& t) const {
        for (auto& c : children) if (c.tag == t) return &c;
        return nullptr;
    }
    std::string childText(const std::string& t, const std::string& def = "") const {
        const Node* c = child(t);
        return c ? c->text : def;
    }
    std::string attr(const std::string& a, const std::string& def = "") const {
        auto it = attrs.find(a);
        return it == attrs.end() ? def : it->second;
    }
};

struct XmlParser {
    const std::string& s;
    size_t p = 0;
    explicit XmlParser(const std::string& src) : s(src) {}

    [[noreturn]] void fail(const std::string& why) {
        throw std::runtime_error("xclass: malformed XML: " + why);
    }

    void skipWs() { while (p < s.size() && std::isspace((unsigned char) s[p])) p++; }

    // Skip <?...?>, <!-- ... -->, and reject <!DOCTYPE.
    bool skipMisc() {
        bool any = false;
        for (;;) {
            skipWs();
            if (p + 1 >= s.size() || s[p] != '<') break;
            if (s.compare(p, 4, "<!--") == 0) {
                size_t e = s.find("-->", p + 4);
                if (e == std::string::npos) fail("unterminated comment");
                p = e + 3; any = true; continue;
            }
            if (s.compare(p, 2, "<?") == 0) {
                size_t e = s.find("?>", p + 2);
                if (e == std::string::npos) fail("unterminated <? ?>");
                p = e + 2; any = true; continue;
            }
            if (s.compare(p, 9, "<!DOCTYPE") == 0) fail("DOCTYPE not allowed");
            break;
        }
        return any;
    }

    std::string parseName() {
        size_t start = p;
        while (p < s.size()) {
            char c = s[p];
            if (std::isalnum((unsigned char) c) || c == '-' || c == '_' || c == ':' || c == '.') p++;
            else break;
        }
        if (p == start) fail("expected name");
        return s.substr(start, p - start);
    }

    std::string parseQuoted() {
        char q = s[p];
        if (q != '"' && q != '\'') fail("expected quoted attribute value");
        size_t start = ++p;
        while (p < s.size() && s[p] != q) p++;
        if (p >= s.size()) fail("unterminated attribute value");
        std::string raw = s.substr(start, p - start);
        p++;                      // closing quote
        return unescape(raw);
    }

    static std::string unescape(const std::string& in) {
        std::string out; out.reserve(in.size());
        for (size_t i = 0; i < in.size(); i++) {
            if (in[i] == '&') {
                if (in.compare(i, 4, "&lt;") == 0) { out += '<'; i += 3; }
                else if (in.compare(i, 4, "&gt;") == 0) { out += '>'; i += 3; }
                else if (in.compare(i, 5, "&amp;") == 0) { out += '&'; i += 4; }
                else if (in.compare(i, 6, "&quot;") == 0) { out += '"'; i += 5; }
                else if (in.compare(i, 6, "&apos;") == 0) { out += '\''; i += 5; }
                else out += in[i];
            } else out += in[i];
        }
        return out;
    }

    // Parse one element (assumes current char is '<' of a start tag).
    Node parseElement() {
        skipMisc();
        if (p >= s.size() || s[p] != '<') fail("expected '<'");
        p++;                                  // consume '<'
        Node n;
        n.tag = parseName();

        // attributes
        for (;;) {
            skipWs();
            if (p >= s.size()) fail("unterminated start tag");
            if (s[p] == '/' && p + 1 < s.size() && s[p + 1] == '>') { p += 2; return n; } // self-closing
            if (s[p] == '>') { p++; break; }
            std::string an = parseName();
            skipWs();
            if (p >= s.size() || s[p] != '=') fail("expected '=' in attribute");
            p++; skipWs();
            n.attrs[an] = parseQuoted();
        }

        // content: children + text until the matching end tag
        for (;;) {
            // gather text up to next '<'
            size_t textStart = p;
            while (p < s.size() && s[p] != '<') p++;
            if (p > textStart) {
                std::string t = s.substr(textStart, p - textStart);
                // trim + append if non-empty
                size_t a = t.find_first_not_of(" \t\r\n");
                size_t b = t.find_last_not_of(" \t\r\n");
                if (a != std::string::npos) n.text += unescape(t.substr(a, b - a + 1));
            }
            if (p >= s.size()) fail("unterminated element <" + n.tag + ">");
            if (s.compare(p, 4, "<!--") == 0) { skipMisc(); continue; }
            if (s.compare(p, 2, "</") == 0) {  // end tag
                p += 2;
                std::string close = parseName();
                skipWs();
                if (p >= s.size() || s[p] != '>') fail("expected '>' in end tag");
                p++;
                if (close != n.tag) fail("mismatched end tag </" + close + "> for <" + n.tag + ">");
                return n;
            }
            n.children.push_back(parseElement());
        }
    }

    Node parseRoot() {
        skipMisc();
        Node root = parseElement();
        return root;
    }
};

// ---- type mapping (XCI-0001 §2) -------------------------------------------
std::string mapType(const std::string& tok, const std::string& selfName) {
    std::string t = tok;
    // normalize
    if (t == "void" || t == "unit" || t == "()" || t.empty()) return "void";
    if (t == "bool" || t == "boolean") return "boolean";
    if (t == "String" || t == "str" || t == "&str" || t == "char") return "String";
    if (t == "f32" || t == "f64" || t == "float" || t == "double") return "double";
    if (t == "i8" || t == "i16" || t == "i32" || t == "i64" ||
        t == "u8" || t == "u16" || t == "u32" || t == "u64" ||
        t == "usize" || t == "isize" || t == "int" || t == "long") return "int";
    if (t == "Self") return "int";                 // object handle
    (void) selfName;
    return "int";                                   // Variant / user types -> int handle
}

// ---- synthesized default-return body for a mapped return type -------------
std::unique_ptr<Block> synthBody(const std::string& retType) {
    auto blk = std::make_unique<Block>();
    if (retType == "void") return blk;              // implicit return
    auto ret = std::make_unique<ReturnStmt>();
    if (retType == "boolean") ret->value = std::make_unique<BoolLit>(false);
    else if (retType == "double") ret->value = std::make_unique<DoubleLit>(0.0);
    else if (retType == "String") ret->value = std::make_unique<StrLit>("");
    else ret->value = std::make_unique<IntLit>(0);  // int / handle
    blk->stmts.push_back(std::move(ret));
    return blk;
}

std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

} // namespace

// ---------------------------------------------------------------------------
// loadDocument: one <xclass> -> one ClassDecl + Meta.
// ---------------------------------------------------------------------------
void loadDocument(const std::string& xmlText, Program& prog, std::vector<Meta>& metas) {
    XmlParser xp(xmlText);
    Node root = xp.parseRoot();
    if (root.tag != "xclass")
        throw std::runtime_error("xclass: root element is <" + root.tag + ">, expected <xclass>");

    Meta meta;
    meta.edition = root.attr("edition");
    meta.compiler = root.attr("compiler");
    meta.compilerVersion = root.attr("compiler-version");

    const Node* id = root.child("identity");
    if (!id) throw std::runtime_error("xclass: missing <identity>");
    meta.name = trim(id->childText("name"));
    if (meta.name.empty()) throw std::runtime_error("xclass: <identity> has no <name>");
    meta.superclass = id->childText("superclass", "(none)");
    meta.isAbstract = id->childText("abstract") == "true";
    meta.isInterface = id->childText("interface") == "true";
    meta.isPublic = id->childText("public", "true") == "true";
    try { meta.methodCount = std::stoi(id->childText("method-count", "0")); } catch (...) {}
    try { meta.fieldCount = std::stoi(id->childText("field-count", "0")); } catch (...) {}
    try { meta.weight = std::stoi(id->childText("weight", "0")); } catch (...) {}
    if (const Node* ifaces = id->child("interfaces")) {
        for (auto& c : ifaces->children)
            if (c.tag == "implements") meta.interfaces.push_back(trim(c.text));
    }

    if (const Node* sec = root.child("security")) {
        try { meta.trustGrade = std::stoi(sec->childText("trust-grade", "0")); } catch (...) {}
        try { meta.classloadGrade = std::stoi(sec->childText("classload-grade", "0")); } catch (...) {}
    }
    if (const Node* prov = root.child("provenance")) {
        meta.sourceFile = prov->childText("source-file");
        meta.signer = prov->childText("signer");
        meta.signed_ = prov->childText("signed") == "true";
        if (const Node* sig = prov->child("signature")) {
            meta.signatureAlg = sig->attr("algorithm");
            meta.signatureHex = trim(sig->text);
        }
    }

    // Build the ClassDecl.
    ClassDecl cls;
    cls.name = meta.name;

    if (const Node* fields = root.child("fields")) {
        for (auto& f : fields->children) {
            if (f.tag != "field") continue;
            Field fld;
            fld.type = mapType(f.attr("type"), meta.name);
            fld.name = f.attr("name");
            if (fld.name.empty()) continue;
            fld.init = nullptr;      // defaults (0 / null) at runtime
            cls.fields.push_back(std::move(fld));
        }
    }

    if (const Node* methods = root.child("methods")) {
        for (auto& m : methods->children) {
            if (m.tag != "method") continue;
            Method meth;
            meth.name = m.attr("name");
            if (meth.name.empty()) continue;
            meth.retType = mapType(m.attr("return"), meta.name);
            int params = 0;
            try { params = std::stoi(m.attr("params", "0")); } catch (...) {}
            for (int i = 0; i < params; i++)
                meth.params.push_back(Param{"int", "p" + std::to_string(i)});
            meth.body = synthBody(meth.retType);
            cls.methods.push_back(std::move(meth));
        }
    }

    prog.classes.push_back(std::move(cls));
    metas.push_back(std::move(meta));
}

// ---------------------------------------------------------------------------
// Reconstructed-source emitter (for --emit / Loaded::emitted).
// ---------------------------------------------------------------------------
namespace {
void emitExpr(std::ostringstream& o, const Expr* e);

void emitExpr(std::ostringstream& o, const Expr* e) {
    if (!e) { o << "0"; return; }
    if (auto* i = dynamic_cast<const IntLit*>(e)) { o << i->value; return; }
    if (auto* d = dynamic_cast<const DoubleLit*>(e)) { o << d->value; return; }
    if (auto* b = dynamic_cast<const BoolLit*>(e)) { o << (b->value ? "true" : "false"); return; }
    if (auto* s = dynamic_cast<const StrLit*>(e)) { o << '"' << s->value << '"'; return; }
    if (auto* v = dynamic_cast<const VarExpr*>(e)) { o << v->name; return; }
    if (auto* c = dynamic_cast<const Call*>(e)) {
        o << c->callee << "(";
        for (size_t i = 0; i < c->args.size(); i++) { if (i) o << ", "; emitExpr(o, c->args[i].get()); }
        o << ")"; return;
    }
    if (auto* b = dynamic_cast<const Binary*>(e)) {
        o << "("; emitExpr(o, b->lhs.get()); o << " " << b->op << " "; emitExpr(o, b->rhs.get()); o << ")"; return;
    }
    if (auto* u = dynamic_cast<const Unary*>(e)) { o << u->op; emitExpr(o, u->operand.get()); return; }
    o << "0";
}

void emitStmt(std::ostringstream& o, const Stmt* s, int ind);

void indent(std::ostringstream& o, int n) { for (int i = 0; i < n; i++) o << "    "; }

void emitStmt(std::ostringstream& o, const Stmt* s, int ind) {
    if (auto* b = dynamic_cast<const Block*>(s)) {
        o << "{\n";
        for (auto& st : b->stmts) { indent(o, ind + 1); emitStmt(o, st.get(), ind + 1); o << "\n"; }
        indent(o, ind); o << "}";
        return;
    }
    if (auto* r = dynamic_cast<const ReturnStmt*>(s)) {
        o << "return";
        if (r->value) { o << " "; emitExpr(o, r->value.get()); }
        o << ";"; return;
    }
    if (auto* p = dynamic_cast<const PrintStmt*>(s)) { o << "print("; emitExpr(o, p->expr.get()); o << ");"; return; }
    if (auto* e = dynamic_cast<const ExprStmt*>(s)) { emitExpr(o, e->expr.get()); o << ";"; return; }
    o << ";";
}
} // namespace

std::string emitSource(const Program& prog) {
    std::ostringstream o;
    o << "// Reconstructed by the Sleela .xclass loader (XCI-0001)\n";
    o << "// Method bodies are synthesized skeletons: .xclass carries structure,\n";
    o << "// not executable bytecode. Type contracts are preserved.\n\n";
    for (const auto& cls : prog.classes) {
        o << "class " << cls.name << " {\n";
        for (const auto& f : cls.fields) {
            indent(o, 1); o << f.type << " " << f.name << ";\n";
        }
        if (!cls.fields.empty()) o << "\n";
        for (const auto& m : cls.methods) {
            indent(o, 1);
            o << m.retType << " " << m.name << "(";
            for (size_t i = 0; i < m.params.size(); i++) {
                if (i) o << ", ";
                o << m.params[i].type << " " << m.params[i].name;
            }
            o << ") ";
            if (m.body) emitStmt(o, m.body.get(), 1); else o << "{}";
            o << "\n";
        }
        o << "}\n\n";
    }
    return o.str();
}

std::string infoLine(const Meta& m) {
    std::ostringstream o;
    o << m.name
      << " (methods=" << m.methodCount
      << " fields=" << m.fieldCount
      << " trust=" << m.trustGrade
      << " classload=" << m.classloadGrade;
    if (m.isInterface) o << " interface";
    if (m.isAbstract)  o << " abstract";
    if (!m.interfaces.empty()) {
        o << " implements";
        for (auto& i : m.interfaces) o << " " << i;
    }
    if (m.signed_) o << " signed";
    o << ")";
    return o.str();
}

// ---------------------------------------------------------------------------
// loadFiles: ingest several .xclass, synthesize an entry shim if needed, emit.
// ---------------------------------------------------------------------------
Loaded loadFiles(const std::vector<std::string>& paths) {
    Loaded out;
    for (const auto& path : paths) {
        std::ifstream f(path, std::ios::binary);
        if (!f) throw std::runtime_error("xclass: cannot open '" + path + "'");
        std::ostringstream ss; ss << f.rdbuf();
        loadDocument(ss.str(), out.program, out.metas);
    }

    // Does any class already define `void main()` with no params?
    bool hasMain = false;
    for (const auto& c : out.program.classes)
        for (const auto& m : c.methods)
            if (m.name == "main" && m.params.empty()) hasMain = true;

    if (!hasMain) {
        ClassDecl shim;
        shim.name = "XClassMain";
        Method main;
        main.retType = "void";
        main.name = "main";
        main.body = std::make_unique<Block>();

        auto banner = std::make_unique<PrintStmt>();
        banner->expr = std::make_unique<StrLit>("xclass: ingested " +
            std::to_string(out.metas.size()) + " SecureJDK 28 class(es)");
        main.body->stmts.push_back(std::move(banner));

        for (const auto& m : out.metas) {
            auto line = std::make_unique<PrintStmt>();
            line->expr = std::make_unique<StrLit>("  " + infoLine(m));
            main.body->stmts.push_back(std::move(line));
        }
        shim.methods.push_back(std::move(main));
        out.program.classes.push_back(std::move(shim));
    }

    out.emitted = emitSource(out.program);
    return out;
}

} // namespace xclass
} // namespace sleela
