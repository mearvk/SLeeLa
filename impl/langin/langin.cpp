// ===========================================================================
// langin.cpp -- native ingest of the JVM language family into Sleela.
// ===========================================================================
#include "langin.h"
#include "../xclass/xclass_loader.h"   // reuse emitSource() for .sleela rendering

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <regex>

namespace sleela {
namespace langin {

const char* languageName(Language lang) noexcept {
    switch (lang) {
        case Language::Java:    return "Java";
        case Language::Kotlin:  return "Kotlin";
        case Language::Scala:   return "Scala";
        case Language::Groovy:  return "Groovy";
        case Language::Clojure: return "Clojure";
        default:                return "Unknown";
    }
}

static std::string lower(std::string s) {
    for (char& c : s) c = (char)std::tolower((unsigned char)c);
    return s;
}

static bool endsWith(const std::string& s, const std::string& suf) {
    return s.size() >= suf.size() && s.compare(s.size() - suf.size(), suf.size(), suf) == 0;
}

Language languageForPath(const std::string& path) {
    std::string p = lower(path);
    if (endsWith(p, ".java"))   return Language::Java;
    if (endsWith(p, ".kt") || endsWith(p, ".kts")) return Language::Kotlin;
    if (endsWith(p, ".scala") || endsWith(p, ".sc")) return Language::Scala;
    if (endsWith(p, ".groovy") || endsWith(p, ".gvy")) return Language::Groovy;
    if (endsWith(p, ".clj") || endsWith(p, ".cljs") || endsWith(p, ".cljc")) return Language::Clojure;
    return Language::Unknown;
}

// --- type mapping: source-language types -> Sleela's surface types ----------
// Sleela's surface types are int/double/boolean/String/void (parsed, not
// enforced). Anything else maps to a struct-handle "int" per the xclass model.
static std::string mapType(std::string t) {
    // strip Kotlin/Scala nullability + whitespace + generics
    std::string s;
    for (char c : t) { if (!std::isspace((unsigned char)c)) s += c; }
    // drop a trailing '?' (Kotlin nullable) and any generic <...>
    if (!s.empty() && s.back() == '?') s.pop_back();
    auto lt = s.find('<'); if (lt != std::string::npos) s = s.substr(0, lt);
    std::string l = lower(s);
    if (l == "void" || l == "unit" || l == "nothing") return "void";
    if (l == "int" || l == "integer" || l == "long" || l == "short" || l == "byte" ||
        l == "int" ) return "int";
    if (l == "double" || l == "float" || l == "bigdecimal") return "double";
    if (l == "boolean" || l == "bool") return "boolean";
    if (l == "string" || l == "char" || l == "charsequence") return "String";
    // any object/reference type is carried as a struct handle (int)
    return "int";
}

// Build a default-return body for a mapped return type (mirrors xclass synthBody).
static std::unique_ptr<Block> synthBody(const std::string& retType) {
    auto blk = std::make_unique<Block>();
    if (retType == "void") return blk;
    auto ret = std::make_unique<ReturnStmt>();
    if (retType == "boolean") ret->value = std::make_unique<BoolLit>(false);
    else if (retType == "double") ret->value = std::make_unique<DoubleLit>(0.0);
    else if (retType == "String") ret->value = std::make_unique<StrLit>("");
    else ret->value = std::make_unique<IntLit>(0);
    blk->stmts.push_back(std::move(ret));
    return blk;
}

// Split a parameter list "a: Int, b: String" or "int a, String b" into
// (type,name) pairs. Handles both Java-style (type name) and Kotlin/Scala-style
// (name: Type). Best-effort and skeleton-only.
static std::vector<Param> parseParams(const std::string& raw, Language lang) {
    std::vector<Param> out;
    std::string inner = raw;
    // trim surrounding parens
    auto lp = inner.find('('); auto rp = inner.rfind(')');
    if (lp != std::string::npos && rp != std::string::npos && rp > lp)
        inner = inner.substr(lp + 1, rp - lp - 1);
    if (inner.find_first_not_of(" \t\r\n") == std::string::npos) return out;
    std::stringstream ss(inner);
    std::string item;
    int depth = 0; std::string acc;
    // split on top-level commas (ignore commas inside generics)
    for (char c : inner) {
        if (c == '<') depth++;
        else if (c == '>') depth--;
        if (c == ',' && depth == 0) { out.push_back({}); acc.clear(); }
        // handled below via a cleaner tokenizer
    }
    out.clear();
    std::vector<std::string> items; acc.clear(); depth = 0;
    for (char c : inner) {
        if (c == '<') depth++;
        if (c == '>') depth--;
        if (c == ',' && depth == 0) { items.push_back(acc); acc.clear(); }
        else acc += c;
    }
    if (!acc.empty()) items.push_back(acc);
    for (auto& it : items) {
        Param p;
        auto colon = it.find(':');
        if ((lang == Language::Kotlin || lang == Language::Scala || lang == Language::Groovy)
            && colon != std::string::npos) {
            // name: Type
            std::string name = it.substr(0, colon);
            std::string type = it.substr(colon + 1);
            // trim
            auto trim = [](std::string& s){ size_t a=s.find_first_not_of(" \t\r\n"); size_t b=s.find_last_not_of(" \t\r\n"); s = (a==std::string::npos)?"":s.substr(a,b-a+1); };
            trim(name); trim(type);
            // strip default values
            auto eq = name.find('='); if (eq != std::string::npos) name = name.substr(0, eq);
            auto eq2 = type.find('='); if (eq2 != std::string::npos) type = type.substr(0, eq2);
            trim(name); trim(type);
            p.name = name; p.type = mapType(type);
        } else {
            // Java/Groovy style: Type name  (take last token as name)
            std::stringstream ts(it); std::vector<std::string> toks; std::string t;
            while (ts >> t) toks.push_back(t);
            if (toks.empty()) continue;
            p.name = toks.back();
            p.type = mapType(toks.size() >= 2 ? toks[toks.size()-2] : "int");
        }
        if (p.name.empty()) p.name = "arg" + std::to_string(out.size());
        if (p.type.empty()) p.type = "int";
        out.push_back(p);
    }
    return out;
}

// --- primary-name / package extraction --------------------------------------
static std::string firstMatch(const std::string& text, const std::regex& re, int group = 1) {
    std::smatch m;
    if (std::regex_search(text, m, re) && (int)m.size() > group) return m[group].str();
    return "";
}

// =========================================================================
// Per-language skeleton ingest. Each fills a ClassDecl (fields + method
// signatures) and a Meta. Bodies are synthesized (skeleton transpile).
// =========================================================================
static void ingest(const std::string& text, Language lang, const std::string& path,
                   Program& prog, std::vector<Meta>& metas) {
    Meta meta;
    meta.language = lang;
    meta.sourceFile = path;

    ClassDecl cls;

    if (lang == Language::Clojure) {
        // Clojure: (ns my.space) and (defn name [args] ...) / (def name ...).
        meta.packageName = firstMatch(text, std::regex(R"(\(ns\s+([A-Za-z_][\w\.\-]*))"));
        std::string base = meta.packageName.empty() ? "ClojureUnit" : meta.packageName;
        // sanitize namespace into a Sleela identifier
        std::string name; for (char c : base) name += (std::isalnum((unsigned char)c) ? c : '_');
        cls.name = name.empty() ? "ClojureUnit" : name;

        // functions: (defn foo [a b] ...)
        std::regex defn(R"(\(defn-?\s+([A-Za-z_][\w\-\?!*]*)\s*\[([^\]]*)\])");
        for (auto it = std::sregex_iterator(text.begin(), text.end(), defn);
             it != std::sregex_iterator(); ++it) {
            Method m; std::string fn = (*it)[1].str();
            for (char& c : fn) if (!(std::isalnum((unsigned char)c) || c=='_')) c = '_';
            m.name = fn; m.retType = "int";
            // arg vector -> untyped params (Clojure is dynamic) => int handles
            std::stringstream as((*it)[2].str()); std::string a; int i=0;
            while (as >> a) { Param p; p.type="int"; std::string an; for(char c:a) an += (std::isalnum((unsigned char)c)?c:'_'); p.name = an.empty()?("arg"+std::to_string(i)):an; cls.methods.push_back({}); cls.methods.pop_back(); m.params.push_back(p); i++; }
            m.body = synthBody(m.retType);
            cls.methods.push_back(std::move(m));
        }
        // (def name ...) -> fields
        std::regex defv(R"(\(def\s+([A-Za-z_][\w\-\?!*]*))");
        for (auto it = std::sregex_iterator(text.begin(), text.end(), defv);
             it != std::sregex_iterator(); ++it) {
            Field f; std::string fn = (*it)[1].str();
            for (char& c : fn) if (!(std::isalnum((unsigned char)c) || c=='_')) c = '_';
            f.type = "int"; f.name = fn; cls.fields.push_back(std::move(f));
        }
    } else {
        // Java / Kotlin / Scala / Groovy: package + class/interface/object + members.
        meta.packageName = firstMatch(text, std::regex(R"(package\s+([A-Za-z_][\w\.]*))"));
        // class / interface / object / trait name
        std::smatch cm;
        std::regex cdecl(R"((?:public\s+|final\s+|abstract\s+|open\s+|data\s+|sealed\s+)*(class|interface|object|trait)\s+([A-Za-z_]\w*))");
        if (std::regex_search(text, cm, cdecl)) {
            std::string kw = cm[1].str();
            cls.name = cm[2].str();
            meta.isInterface = (kw == "interface" || kw == "trait");
        } else {
            cls.name = "Unit";
        }
        if (text.find("abstract ") != std::string::npos) meta.isAbstract = true;

        // Methods:
        //  Java/Groovy:  [mods] RetType name(params) { | ;
        //  Kotlin:       fun name(params): RetType
        //  Scala:        def name(params): RetType
        if (lang == Language::Kotlin) {
            std::regex fn(R"(fun\s+([A-Za-z_]\w*)\s*\(([^\)]*)\)\s*(?::\s*([A-Za-z_][\w\.<>\?]*))?)");
            for (auto it = std::sregex_iterator(text.begin(), text.end(), fn);
                 it != std::sregex_iterator(); ++it) {
                Method m; m.name = (*it)[1].str();
                m.retType = (*it)[3].matched ? mapType((*it)[3].str()) : "void";
                m.params = parseParams("(" + (*it)[2].str() + ")", lang);
                m.body = synthBody(m.retType); cls.methods.push_back(std::move(m));
            }
            // val/var fields:  val name: Type
            std::regex fld(R"((?:val|var)\s+([A-Za-z_]\w*)\s*:\s*([A-Za-z_][\w\.<>\?]*))");
            for (auto it = std::sregex_iterator(text.begin(), text.end(), fld);
                 it != std::sregex_iterator(); ++it) {
                Field f; f.name = (*it)[1].str(); f.type = mapType((*it)[2].str());
                cls.fields.push_back(std::move(f));
            }
        } else if (lang == Language::Scala) {
            std::regex fn(R"(def\s+([A-Za-z_]\w*)\s*\(([^\)]*)\)\s*(?::\s*([A-Za-z_][\w\.<>\[\]]*))?)");
            for (auto it = std::sregex_iterator(text.begin(), text.end(), fn);
                 it != std::sregex_iterator(); ++it) {
                Method m; m.name = (*it)[1].str();
                m.retType = (*it)[3].matched ? mapType((*it)[3].str()) : "void";
                m.params = parseParams("(" + (*it)[2].str() + ")", lang);
                m.body = synthBody(m.retType); cls.methods.push_back(std::move(m));
            }
            std::regex fld(R"((?:val|var)\s+([A-Za-z_]\w*)\s*:\s*([A-Za-z_][\w\.<>\[\]]*))");
            for (auto it = std::sregex_iterator(text.begin(), text.end(), fld);
                 it != std::sregex_iterator(); ++it) {
                Field f; f.name = (*it)[1].str(); f.type = mapType((*it)[2].str());
                cls.fields.push_back(std::move(f));
            }
        } else {
            // Java / Groovy: RetType name(params) followed by { or ;
            std::regex fn(R"((?:public\s+|private\s+|protected\s+|static\s+|final\s+|abstract\s+|def\s+)*([A-Za-z_][\w\.<>\[\]]*)\s+([A-Za-z_]\w*)\s*\(([^\)]*)\)\s*[\{;])");
            for (auto it = std::sregex_iterator(text.begin(), text.end(), fn);
                 it != std::sregex_iterator(); ++it) {
                std::string ret = (*it)[1].str(); std::string nm = (*it)[2].str();
                if (ret == "new" || ret == "return" || ret == "if" || ret == "while" || ret == "for") continue;
                Method m; m.name = nm; m.retType = mapType(ret);
                m.params = parseParams("(" + (*it)[3].str() + ")", lang);
                m.body = synthBody(m.retType); cls.methods.push_back(std::move(m));
            }
            // Java fields:  [mods] Type name ; | =
            std::regex fld(R"((?:public\s+|private\s+|protected\s+|static\s+|final\s+)+([A-Za-z_][\w\.<>\[\]]*)\s+([A-Za-z_]\w*)\s*[;=])");
            for (auto it = std::sregex_iterator(text.begin(), text.end(), fld);
                 it != std::sregex_iterator(); ++it) {
                Field f; f.type = mapType((*it)[1].str()); f.name = (*it)[2].str();
                cls.fields.push_back(std::move(f));
            }
        }
    }

    if (cls.name.empty()) cls.name = "Unit";
    meta.name = cls.name;
    meta.fieldCount = (int)cls.fields.size();
    meta.methodCount = (int)cls.methods.size();
    metas.push_back(meta);
    prog.classes.push_back(std::move(cls));
}

void loadDocument(const std::string& text, Language lang, const std::string& path,
                  Program& prog, std::vector<Meta>& metas) {
    if (lang == Language::Unknown) {
        throw std::runtime_error("langin: unrecognized JVM language for '" + path + "'");
    }
    size_t before = prog.classes.size();
    ingest(text, lang, path, prog, metas);
    if (prog.classes.size() == before) {
        throw std::runtime_error("langin: no class/namespace recognized in '" + path + "'");
    }
}

std::string infoLine(const Meta& m) {
    std::ostringstream o;
    o << m.name << " [" << languageName(m.language) << "]";
    if (!m.packageName.empty()) o << " pkg=" << m.packageName;
    if (m.isInterface) o << " interface";
    if (m.isAbstract) o << " abstract";
    o << " fields=" << m.fieldCount << " methods=" << m.methodCount;
    return o.str();
}

Loaded loadFiles(const std::vector<std::string>& paths) {
    Loaded out;
    for (const auto& p : paths) {
        Language lang = languageForPath(p);
        std::ifstream in(p, std::ios::binary);
        if (!in) throw std::runtime_error("langin: cannot open '" + p + "'");
        std::stringstream ss; ss << in.rdbuf();
        loadDocument(ss.str(), lang, p, out.program, out.metas);
    }

    // Synthesize an entry `main` if none present, so the Program runs.
    bool hasMain = false;
    for (const auto& c : out.program.classes)
        for (const auto& m : c.methods)
            if (m.name == "main" && m.params.empty()) hasMain = true;
    if (!hasMain) {
        ClassDecl shim; shim.name = "LangInMain";
        Method main; main.retType = "void"; main.name = "main";
        main.body = std::make_unique<Block>();
        auto banner = std::make_unique<PrintStmt>();
        banner->expr = std::make_unique<StrLit>(
            "langin: ingested " + std::to_string(out.metas.size()) + " JVM-family unit(s)");
        main.body->stmts.push_back(std::move(banner));
        for (const auto& m : out.metas) {
            auto line = std::make_unique<PrintStmt>();
            line->expr = std::make_unique<StrLit>("  " + infoLine(m));
            main.body->stmts.push_back(std::move(line));
        }
        shim.methods.push_back(std::move(main));
        out.program.classes.push_back(std::move(shim));
    }

    out.emitted = sleela::xclass::emitSource(out.program);
    return out;
}

// --- .xclass emission (structural descriptor) -------------------------------
static void xmlEscape(std::ostream& o, const std::string& s) {
    for (char c : s) {
        switch (c) {
            case '&': o << "&amp;"; break;
            case '<': o << "&lt;"; break;
            case '>': o << "&gt;"; break;
            default: o << c;
        }
    }
}

std::string emitXclass(const Program& prog, const std::vector<Meta>& metas) {
    std::ostringstream o;
    o << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    for (size_t i = 0; i < prog.classes.size(); ++i) {
        const ClassDecl& c = prog.classes[i];
        const Meta* m = i < metas.size() ? &metas[i] : nullptr;
        o << "<xclass version=\"1\" compiler=\"langin\" compiler-version=\"1.0.0\"";
        if (m) o << " source-language=\"" << languageName(m->language) << "\"";
        o << ">\n";
        o << "  <identity>\n";
        o << "    <name>"; xmlEscape(o, c.name); o << "</name>\n";
        o << "    <superclass>(none)</superclass>\n";
        o << "    <abstract>" << ((m && m->isAbstract) ? "true" : "false") << "</abstract>\n";
        o << "    <interface>" << ((m && m->isInterface) ? "true" : "false") << "</interface>\n";
        o << "    <public>true</public>\n";
        o << "    <method-count>" << c.methods.size() << "</method-count>\n";
        o << "    <field-count>" << c.fields.size() << "</field-count>\n";
        o << "  </identity>\n";
        if (m && !m->packageName.empty()) {
            o << "  <provenance><package>"; xmlEscape(o, m->packageName);
            o << "</package>";
            if (!m->sourceFile.empty()) { o << "<source>"; xmlEscape(o, m->sourceFile); o << "</source>"; }
            o << "</provenance>\n";
        }
        o << "  <members>\n";
        for (const auto& f : c.fields) {
            o << "    <field type=\""; xmlEscape(o, f.type); o << "\" name=\"";
            xmlEscape(o, f.name); o << "\"/>\n";
        }
        for (const auto& mm : c.methods) {
            o << "    <method ret=\""; xmlEscape(o, mm.retType); o << "\" name=\"";
            xmlEscape(o, mm.name); o << "\" params=\"" << mm.params.size() << "\"/>\n";
        }
        o << "  </members>\n";
        o << "</xclass>\n";
    }
    return o.str();
}

} // namespace langin
} // namespace sleela
