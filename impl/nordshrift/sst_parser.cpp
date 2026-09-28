// ===========================================================================
// sst_parser.cpp  --  Spec-conformant .sst parser (NS-SST-0001 Part XIV).
// ===========================================================================
#include "sst_parser.h"

#include <algorithm>
#include <cstdlib>

namespace nordshrift {
namespace {

// Recognized pipeline phase tokens, in mandatory order (§7.3).
const std::vector<std::string> kPhaseOrder = {
    "lex", "parse", "desugar", "bind", "infer", "effect-check",
    "rule-eval", "ir", "java-emit", "java-print", "diagnostic"
};
// Phases that may not be skipped (PIPE-02).
bool mandatoryPhase(const std::string& p) {
    return p == "lex" || p == "parse" || p == "bind" || p == "infer" || p == "java-print";
}
int phaseRank(const std::string& p) {
    for (size_t i = 0; i < kPhaseOrder.size(); i++) if (kPhaseOrder[i] == p) return (int)i;
    return -1;
}

struct Parser {
    const std::vector<Token>& t;
    const std::string& file;
    DiagnosticBag& diags;
    size_t i = 0;
    Sheet sheet;

    Parser(const std::vector<Token>& toks, const std::string& f, DiagnosticBag& d)
        : t(toks), file(f), diags(d) { sheet.file = f; }

    // ---- token cursor helpers ------------------------------------------
    const Token& cur() const { return t[i]; }
    const Token& peek(int off = 1) const {
        size_t p = i + (size_t)off; return p < t.size() ? t[p] : t.back();
    }
    bool is(Tok k) const { return cur().kind == k; }
    bool isIdent(const char* s) const { return cur().kind == Tok::Ident && cur().text == s; }
    const Token& take() { return t[i < t.size() - 1 ? i++ : i]; }
    void skipNewlines() { while (is(Tok::Newline)) i++; }

    void errAt(int line, const std::string& code, const std::string& msg,
               const std::string& ref, bool recover = false) {
        diags.error(code, file, line, msg, ref, recover);
    }

    // Consume tokens until the next Newline (error recovery).
    void syncToNewline() { while (!is(Tok::Newline) && !is(Tok::Eof)) i++; if (is(Tok::Newline)) i++; }

    // Enter a block body: expects Colon Newline Indent. Returns true on success.
    bool enterBlock() {
        if (!is(Tok::Colon)) return false;
        i++;                             // ':'
        if (is(Tok::Newline)) i++;
        if (!is(Tok::Indent)) return false;
        i++;                             // INDENT
        return true;
    }
    bool atBlockEnd() const { return is(Tok::Dedent) || is(Tok::Eof); }
    void exitBlock() { if (is(Tok::Dedent)) i++; }

    // ---- scalar / list readers -----------------------------------------
    std::string readScalarText() {
        // scalar-value ::= string | boolean | integer | version | identifier
        const Token& tk = cur();
        if (tk.kind == Tok::String || tk.kind == Tok::Integer ||
            tk.kind == Tok::Version || tk.kind == Tok::Ident) { i++; return tk.text; }
        return "";
    }

    // list-literal ::= "[" [ scalar { "," scalar } ] "]"
    // (line-continuation form is already stitched by the lexer into one line)
    std::vector<std::string> readList() {
        std::vector<std::string> items;
        if (is(Tok::LBracket)) {
            i++;
            while (!is(Tok::RBracket) && !is(Tok::Newline) && !is(Tok::Eof)) {
                if (is(Tok::Comma)) { i++; continue; }
                std::string s = readScalarText();
                if (s.empty() && !is(Tok::Comma) && !is(Tok::RBracket)) { i++; continue; }
                items.push_back(s);
            }
            if (is(Tok::RBracket)) i++;
        }
        return items;
    }

    bool parseBool(const std::string& s, bool dflt) {
        if (s == "true" || s == "on") return true;
        if (s == "false" || s == "off") return false;
        return dflt;
    }

    // ---- top level ------------------------------------------------------
    Sheet run() {
        parsePragmas();
        skipNewlines();
        parseSheetBlock();
        skipNewlines();
        parseImports();
        skipNewlines();
        // sections in any order
        while (!is(Tok::Eof)) {
            if (is(Tok::Newline)) { i++; continue; }
            if (is(Tok::Dedent)) { i++; continue; }
            if (!parseSection()) {
                // unknown top-level construct: report and recover
                errAt(cur().line, "NSS-E-0100",
                      "unexpected token '" + cur().text + "' at top level", "§2.1", true);
                syncToNewline();
            }
            skipNewlines();
        }

        // pragma presence check (NSS-E-0003 / 0004)
        if (!sheet.pragmas.hasNordshrift) {
            errAt(1, "NSS-E-0003", "missing required #nordshrift pragma", "§1.6");
        } else {
            const std::string& v = sheet.pragmas.nordshrift;
            if (v != "1.0" && v != "1.0.0") {
                errAt(sheet.pragmas.nordshriftLine, "NSS-E-0004",
                      "unsupported #nordshrift version '" + v + "' (this tool supports 1.0)", "§1.6");
            }
        }
        if (!sheet.meta.present) {
            errAt(1, "NSS-E-0010", "no sheet block found (exactly one is required)", "SST-01");
        }
        return sheet;
    }

    // ---- pragmas --------------------------------------------------------
    void parsePragmas() {
        while (is(Tok::Pragma)) {
            std::string name = cur().text;
            int line = cur().line;
            i++;
            std::string operand = readScalarText();
            if (name == "nordshrift") {
                sheet.pragmas.nordshrift = operand;
                sheet.pragmas.hasNordshrift = true;
                sheet.pragmas.nordshriftLine = line;
            } else if (name == "sleela") {
                sheet.pragmas.sleela = operand;
            } else if (name == "encoding") {
                sheet.pragmas.encoding = operand;
            }
            if (is(Tok::Newline)) i++;
        }
    }

    // ---- sheet block ----------------------------------------------------
    void parseSheetBlock() {
        if (!isIdent("sheet")) return;
        if (sheet.meta.present) {
            errAt(cur().line, "NSS-E-0010", "multiple sheet blocks in one file", "SST-01");
        }
        sheet.meta.present = true;
        sheet.meta.line = cur().line;
        i++;                                   // 'sheet'
        if (is(Tok::Ident)) sheet.meta.name = take().text;
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            std::string key = cur().text; i++;
            if (key == "version")          sheet.meta.version = readScalarText();
            else if (key == "author")      sheet.meta.author = readScalarText();
            else if (key == "description") sheet.meta.description = readScalarText();
            else if (key == "extends")     sheet.meta.extends = readScalarText();
            else if (key == "tags")        sheet.meta.tags = readList();
            else { /* unknown sheet directive: ignore */ }
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }

    // ---- imports --------------------------------------------------------
    void parseImports() {
        while (isIdent("import")) {
            Import im; im.line = cur().line;
            i++;                                // 'import'
            im.path = readScalarText();
            if (isIdent("as")) { i++; if (is(Tok::Ident)) im.alias = take().text; }
            if (isIdent("only"))   { i++; im.only = readList();   im.hasOnly = true; }
            else if (isIdent("except")) { i++; im.except = readList(); im.hasExcept = true; }
            // IMPORT-05 alias uniqueness
            for (auto& e : sheet.imports)
                if (!im.alias.empty() && e.alias == im.alias)
                    errAt(im.line, "NSS-E-0021", "duplicate import alias '" + im.alias + "'", "IMPORT-05");
            sheet.imports.push_back(im);
            if (is(Tok::Newline)) i++;
            skipNewlines();
        }
    }

    // ---- section dispatch ----------------------------------------------
    bool parseSection() {
        if (!is(Tok::Ident)) return false;
        const std::string& kw = cur().text;
        if (kw == "source")   { parseSource();   return true; }
        if (kw == "target")   { parseTarget();   return true; }
        if (kw == "pipeline") { parsePipeline(sheet.pipeline); return true; }
        if (kw == "rules")    { parseRules();    return true; }
        if (kw == "effects")  { parseEffects();  return true; }
        if (kw == "derive")   { parseDerive(sheet.derive);   return true; }
        if (kw == "guards")   { parseGuards(sheet.guards);   return true; }
        if (kw == "interop")  { parseInterop();  return true; }
        if (kw == "network")  { parseNetwork();  return true; }
        if (kw == "finance")  { parseFinance();  return true; }
        if (kw == "reach")    { parseReach();    return true; }
        if (kw == "measure")  { parseMeasure();  return true; }
        if (kw == "subject")  { parseSubject();  return true; }
        if (kw == "object")   { parseInputObject(); return true; }
        if (kw == "rule")     { parseRuleBlock(); return true; }
        if (kw == "profile")  { parseProfile();  return true; }
        return false;
    }

    // ---- generic SLeeLa input object -------------------------------
    void parseInputObject() {
        InputObject obj;
        const int ln = cur().line;
        i++; // object
        std::string category = readScalarText();
        if (!inputObjectCategoryFromName(category, obj.category)) {
            errAt(ln, "NSS-E-OBJ-001", "unknown input-object category '" + category + "'", "SST-OBJECT-CATEGORY", true);
        }
        if (is(Tok::Ident) || is(Tok::String)) obj.identity = take().text;
        if (!enterBlock()) {
            errAt(ln, "NSS-E-OBJ-002", "input object requires a block", "SST-OBJECT-BLOCK", true);
            if (!obj.identity.empty()) sheet.inputObjects.push_back(std::move(obj));
            syncToNewline(); return;
        }
        obj.line = ln;
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int keyLine = cur().line;
            std::string key = cur().text; i++;
            if (is(Tok::Colon)) i++;
            if (key == "type") obj.type = readScalarText();
            else if (key == "source") obj.source = readScalarText();
            else if (key == "target") obj.target = readScalarText();
            else if (key == "value") obj.value = readScalarText();
            else if (key == "inputs") obj.inputs = readListOrScalar();
            else if (key == "outputs") obj.outputs = readListOrScalar();
            else if (key == "property") {
                std::string name = readScalarText();
                std::string value = readScalarText();
                if (name.empty()) errAt(keyLine, "NSS-E-OBJ-003", "object property requires a name", "SST-OBJECT-PROPERTY", true);
                else if (obj.properties.size() >= 64) errAt(keyLine, "NSS-E-OBJ-004", "input object property limit exceeded", "SST-OBJECT-BOUND", true);
                else obj.properties[name] = value;
            } else {
                // Unknown object members are rejected rather than silently
                // becoming a different API contract.
                errAt(keyLine, "NSS-E-OBJ-005", "unknown input object member '" + key + "'", "SST-OBJECT-MEMBER", true);
                (void)readScalarText();
            }
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
        std::string error;
        if (!inputObjectValidate(obj, error))
            errAt(ln, "NSS-E-OBJ-006", error, "SST-OBJECT-VALIDATE", true);
        else
            sheet.inputObjects.push_back(std::move(obj));
    }

    // ---- source ---------------------------------------------------------
    void parseSource() {
        sheet.source.present = true;
        sheet.source.line = cur().line;
        i++;
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int ln = cur().line;
            std::string key = cur().text; i++;
            if (key == "root")     { sheet.source.root = readScalarText(); sheet.source.hasRoot = true; sheet.source.rootLine = ln; }
            else if (key == "glob") {
                std::string g = readScalarText();
                if (!g.empty() && g[0] == '/')
                    errAt(ln, "NSS-E-0032", "absolute path in source.glob is not permitted: '" + g + "'", "SRC-03");
                sheet.source.globs.push_back(g);
            }
            else if (key == "exclude")  sheet.source.exclude = readList();
            else if (key == "encoding") sheet.source.encoding = readScalarText();
            else if (key == "watch")    sheet.source.watch = parseBool(readScalarText(), false);
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }

    // ---- target ---------------------------------------------------------
    Layout parseLayout(const std::string& s) {
        if (s == "flat") return Layout::Flat;
        if (s == "package-mapped") return Layout::PackageMapped;
        if (s == "custom") return Layout::Custom;
        return Layout::MirrorSource;
    }
    void parseTarget() {
        sheet.target.present = true;
        sheet.target.line = cur().line;
        i++;
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int ln = cur().line;
            std::string key = cur().text; i++;
            if (key == "root")        { sheet.target.root = readScalarText(); sheet.target.hasRoot = true; }
            else if (key == "layout") { sheet.target.layout = parseLayout(readScalarText()); sheet.target.hasLayout = true; }
            else if (key == "java-version") {
                std::string v = readScalarText();
                sheet.target.javaVersion = v.empty() ? 0 : std::stoi(v);
                sheet.target.hasJavaVersion = true;
                sheet.target.javaVersionLine = ln;
                if (sheet.target.javaVersion < 17)
                    errAt(ln, "NSS-E-0040", "target.java-version must be >= 17 (found " + v + ")", "TGT-01");
            }
            else if (key == "package-root") sheet.target.packageRoot = readScalarText();
            else if (key == "overwrite")    sheet.target.overwrite = parseBool(readScalarText(), true);
            else if (key == "source-map")   sheet.target.sourceMap = parseBool(readScalarText(), true);
            else if (key == "optimize")     sheet.target.optimize = parseBool(readScalarText(), false);
            else if (key == "clean")        sheet.target.clean = parseBool(readScalarText(), false);
            else if (key == "target-language") {          // Nordshrift triplet superset
                std::string v = readScalarText();
                if (v == "sleela") sheet.target.language = TargetLang::Sleela;
                else if (v == "c") sheet.target.language = TargetLang::C;
                else sheet.target.language = TargetLang::Java;
            }
            else if (key == "layout-map")   { sheet.target.layoutMap = readPairBlock(); continue; }
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }

    // ---- pipeline (also used inside profiles) ---------------------------
    Verbosity parseVerbosity(const std::string& s) {
        if (s == "silent") return Verbosity::Silent;
        if (s == "quiet") return Verbosity::Quiet;
        if (s == "verbose") return Verbosity::Verbose;
        if (s == "debug") return Verbosity::Debug;
        return Verbosity::Normal;
    }
    void parsePipeline(Pipeline& p) {
        p.present = true; p.line = cur().line;
        i++;
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int ln = cur().line;
            std::string key = cur().text; i++;
            if (key == "phases")        { p.phases = readList(); p.phasesLine = ln; checkPhaseOrder(p.phases, ln); }
            else if (key == "skip")     { p.skip = readList(); checkSkip(p.skip, ln); }
            else if (key == "parallel-threshold") { std::string v = readScalarText(); p.parallelThreshold = v.empty()?1:std::stoi(v); }
            else if (key == "cache")    p.cache = parseBool(readScalarText(), true);
            else if (key == "cache-dir")p.cacheDir = readScalarText();
            else if (key == "verbosity")p.verbosity = parseVerbosity(readScalarText());
            else if (key == "fail-fast")p.failFast = parseBool(readScalarText(), false);
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }
    void checkPhaseOrder(const std::vector<std::string>& phases, int ln) {
        int last = -1;
        for (auto& p : phases) {
            int r = phaseRank(p);
            if (r < 0) continue;
            if (r < last) { errAt(ln, "NSS-E-0050", "pipeline phases specified out of mandatory order at '" + p + "'", "PIPE-01"); return; }
            last = r;
        }
    }
    void checkSkip(const std::vector<std::string>& skip, int ln) {
        for (auto& p : skip)
            if (mandatoryPhase(p))
                errAt(ln, "NSS-E-0051", "cannot skip mandatory phase '" + p + "'", "PIPE-02");
    }

    // ---- rules ----------------------------------------------------------
    Severity parseSeverity(const std::string& s) {
        if (s == "warning") return Severity::Warning;
        if (s == "notice") return Severity::Notice;
        return Severity::Error;
    }
    void parseRules() {
        sheet.rules.present = true; sheet.rules.line = cur().line;
        i++;
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            std::string key = cur().text; i++;
            if (key == "activate")        sheet.rules.activate = readList();
            else if (key == "deactivate") sheet.rules.deactivate = readList();
            else if (key == "severity") {                 // nested block
                if (enterBlock()) {
                    while (!atBlockEnd()) {
                        if (is(Tok::Newline)) { i++; continue; }
                        if (!is(Tok::Ident)) { syncToNewline(); continue; }
                        std::string rn = take().text;
                        Severity sv = parseSeverity(readScalarText());
                        sheet.rules.severity[rn] = sv;
                        if (is(Tok::Newline)) i++;
                    }
                    exitBlock();
                }
                continue;
            }
            else if (key == "config") {                   // nested config block
                if (enterBlock()) {
                    // config: <RuleName>: key value ...  (we accept flat key value)
                    while (!atBlockEnd()) {
                        if (is(Tok::Newline)) { i++; continue; }
                        if (!is(Tok::Ident)) { syncToNewline(); continue; }
                        std::string k = take().text;
                        std::string v = readScalarText();
                        (void)k; (void)v;   // stored under rule blocks in a later pass
                        if (is(Tok::Newline)) i++;
                    }
                    exitBlock();
                }
                continue;
            }
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }

    // ---- effects --------------------------------------------------------
    EffectPolicy parsePolicy(const std::string& s) {
        if (s == "lenient") return EffectPolicy::Lenient;
        if (s == "audit") return EffectPolicy::Audit;
        if (s == "off") return EffectPolicy::Off;
        return EffectPolicy::Strict;
    }
    void parseEffects() {
        sheet.effects.present = true; sheet.effects.line = cur().line;
        i++;
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            std::string key = cur().text; i++;
            if (key == "policy")             sheet.effects.policy = parsePolicy(readScalarText());
            else if (key == "declare")       sheet.effects.declare = readList();
            else if (key == "default-effect")sheet.effects.defaultEffect = readScalarText();
            else if (key == "aliases") {                  // nested block
                if (enterBlock()) {
                    while (!atBlockEnd()) {
                        if (is(Tok::Newline)) { i++; continue; }
                        if (!is(Tok::Ident)) { syncToNewline(); continue; }
                        std::string an = take().text;
                        sheet.effects.aliases[an] = readList();
                        if (is(Tok::Newline)) i++;
                    }
                    exitBlock();
                }
                continue;
            }
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }

    // ---- derive ---------------------------------------------------------
    DeriveStyle parseStyle(const std::string& s) {
        if (s == "immutable-class") return DeriveStyle::ImmutableClass;
        if (s == "builder") return DeriveStyle::Builder;
        if (s == "lombok") return DeriveStyle::Lombok;
        return DeriveStyle::Record;
    }
    void parseDerive(Derive& d) {
        d.present = true; d.line = cur().line;
        i++;
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int ln = cur().line;
            std::string key = cur().text; i++;
            if (key == "lens")            d.lens = parseBool(readScalarText(), false);
            else if (key == "projection")  d.projection = parseBool(readScalarText(), false);
            else if (key == "equality")    d.equality = parseBool(readScalarText(), false);
            else if (key == "hashcode")    d.hashcode = parseBool(readScalarText(), false);
            else if (key == "toString")    d.toStringD = parseBool(readScalarText(), false);
            else if (key == "copy")        d.copy = parseBool(readScalarText(), false);
            else if (key == "builder")   { d.builder = parseBool(readScalarText(), false); d.hasBuilder = true; }
            else if (key == "target-style"){ d.targetStyle = parseStyle(readScalarText()); d.hasTargetStyle = true; }
            (void)ln;
            if (is(Tok::Newline)) i++;
        }
        // DRV-01: builder true with target-style record
        if (d.hasBuilder && d.builder && d.hasTargetStyle && d.targetStyle == DeriveStyle::Record)
            errAt(d.line, "NSS-E-0080", "derive builder true combined with target-style record", "DRV-01");
        exitBlock();
    }

    // ---- guards ---------------------------------------------------------
    GuardMode parseGuardMode(const std::string& s) {
        if (s == "compile-only") return GuardMode::CompileOnly;
        if (s == "runtime-only") return GuardMode::RuntimeOnly;
        if (s == "off") return GuardMode::Off;
        return GuardMode::CompileAndRuntime;
    }
    FailureAction parseFailure(const std::string& s) {
        if (s == "assert") return FailureAction::Assert;
        if (s == "log-and-continue") return FailureAction::LogAndContinue;
        if (s == "halt") return FailureAction::Halt;
        return FailureAction::Throw;
    }
    void parseGuards(Guards& g) {
        g.present = true; g.line = cur().line;
        i++;
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            std::string key = cur().text; i++;
            if (key == "mode")             g.mode = parseGuardMode(readScalarText());
            else if (key == "on-failure")  g.onFailure = parseFailure(readScalarText());
            else if (key == "message-format") g.messageFormat = readScalarText();
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }

    // ---- interop --------------------------------------------------------
    NullWrap parseNullWrap(const std::string& s) {
        if (s == "nullable-annotation") return NullWrap::NullableAnnotation;
        if (s == "trust") return NullWrap::Trust;
        return NullWrap::Maybe;
    }
    CheckedEx parseCheckedEx(const std::string& s) {
        if (s == "propagate") return CheckedEx::Propagate;
        if (s == "rethrow-unchecked") return CheckedEx::RethrowUnchecked;
        return CheckedEx::Wrap;
    }
    void parseInterop() {
        sheet.interop.present = true; sheet.interop.line = cur().line;
        i++;
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int ln = cur().line;
            std::string key = cur().text; i++;
            if (key == "assume-impure")      sheet.interop.assumeImpure = parseBool(readScalarText(), false);
            else if (key == "null-wrapping") {
                sheet.interop.nullWrapping = parseNullWrap(readScalarText());
                sheet.interop.hasNullWrapping = true;
                if (sheet.interop.nullWrapping == NullWrap::Trust)
                    diags.warning("NSS-W-0100", file, ln, "null-wrapping trust mode activated", "§12.4");
            }
            else if (key == "checked-exceptions") sheet.interop.checkedExceptions = parseCheckedEx(readScalarText());
            else if (key == "package-allow") sheet.interop.packageAllow = readList();
            else if (key == "package-deny")  sheet.interop.packageDeny = readList();
            else if (key == "type-mapping")  { sheet.interop.typeMapping = readPairBlock(); continue; }
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }

    // A nested block of "key value" or "string string" pairs.
    std::map<std::string, std::string> readPairBlock() {
        std::map<std::string, std::string> m;
        if (enterBlock()) {
            while (!atBlockEnd()) {
                if (is(Tok::Newline)) { i++; continue; }
                std::string k = readScalarText();
                std::string v = readScalarText();
                if (!k.empty()) m[k] = v;
                if (is(Tok::Newline)) i++;
            }
            exitBlock();
        }
        return m;
    }

    // Read either a list literal (`[a, b]`) or a single scalar as a 1-item
    // list. Used by the network/finance blocks which accept both plural
    // (`objects: [ ... ]`) and singular repeated (`object: X`) forms.
    std::vector<std::string> readListOrScalar() {
        if (is(Tok::LBracket)) return readList();
        std::vector<std::string> one;
        std::string s = readScalarText();
        if (!s.empty()) one.push_back(s);
        return one;
    }

    // ---- network block (NS-SST-NET-0001) -------------------------------
    void parseNetwork() {
        sheet.network.present = true;
        sheet.network.line = cur().line;
        i++;                                   // 'network'
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int ln = cur().line;
            std::string key = cur().text; i++;
            if (is(Tok::Colon)) i++;           // optional ':' after the key
            if (key == "objects" || key == "object") {
                for (const auto& name : readListOrScalar()) {
                    NetworkObject o;
                    if (!networkObjectFromName(name, o)) {
                        errAt(ln, "NSS-E-NET-001",
                              "unknown network object '" + name + "'",
                              "SST-NET-OBJECT", true);
                    } else if (std::find(sheet.network.objects.begin(),
                                         sheet.network.objects.end(), o)
                               == sheet.network.objects.end()) {
                        sheet.network.objects.push_back(o);
                    }
                }
            } else if (key == "transports" || key == "transport") {
                for (const auto& name : readListOrScalar()) {
                    NetworkTransport tr;
                    if (!networkTransportFromName(name, tr)) {
                        errAt(ln, "NSS-E-NET-002",
                              "unknown network transport '" + name + "'",
                              "SST-NET-TRANSPORT", true);
                    } else if (std::find(sheet.network.transports.begin(),
                                         sheet.network.transports.end(), tr)
                               == sheet.network.transports.end()) {
                        sheet.network.transports.push_back(tr);
                    }
                }
            } else if (key == "address-family") {
                std::string v = readScalarText();
                if (!networkAddressFamilyFromName(v, sheet.network.addressFamily))
                    errAt(ln, "NSS-E-NET-003",
                          "unknown network address-family '" + v + "'",
                          "SST-NET-ADDRESS-FAMILY", true);
            } else if (key == "tls") {
                sheet.network.tls = parseBool(readScalarText(), false);
            } else {
                // unknown network directive: ignore the value
                (void)readScalarText();
            }
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }

    // ---- reach block (SST.MUNCTION.md, Munction 1.3) -------------------
    void parseReach() {
        sheet.reach.present = true;
        sheet.reach.line = cur().line;
        i++;                                   // 'reach'
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int ln = cur().line;
            std::string key = cur().text; i++;
            if (is(Tok::Colon)) i++;
            if (key == "verbs" || key == "verb") {
                for (const auto& name : readListOrScalar()) {
                    ReachVerb v;
                    if (!reachVerbFromName(name, v)) {
                        errAt(ln, "NSS-E-MUN-001", "unknown Munction verb '" + name + "'",
                              "SST-MUN-VERB", true);
                    } else if (std::find(sheet.reach.verbs.begin(), sheet.reach.verbs.end(), v)
                               == sheet.reach.verbs.end()) {
                        sheet.reach.verbs.push_back(v);
                    }
                }
            } else if (key == "channels" || key == "channel") {
                for (const auto& name : readListOrScalar()) {
                    ReachChannel c;
                    if (!reachChannelFromName(name, c)) {
                        errAt(ln, "NSS-E-MUN-002", "unknown Munction channel '" + name + "'",
                              "SST-MUN-CHANNEL", true);
                    } else if (std::find(sheet.reach.channels.begin(), sheet.reach.channels.end(), c)
                               == sheet.reach.channels.end()) {
                        sheet.reach.channels.push_back(c);
                    }
                }
            } else if (key == "min-verbs") {
                sheet.reach.minVerbs = (int)std::strtol(readScalarText().c_str(), nullptr, 10);
            } else if (key == "max-verbs") {
                sheet.reach.maxVerbs = (int)std::strtol(readScalarText().c_str(), nullptr, 10);
            } else if (key == "receivable") {
                sheet.reach.receivable = parseBool(readScalarText(), true);
            } else if (key == "coherent") {
                sheet.reach.coherent = parseBool(readScalarText(), true);
            } else {
                (void)readScalarText();
            }
            if (is(Tok::Newline)) i++;
        }
        // The sanity bound (4..16) is fixed by the language; a sheet may narrow
        // but never widen it. Clamp and report if it tries to widen.
        if (sheet.reach.minVerbs < 4) { errAt(sheet.reach.line, "NSS-E-MUN-003",
              "reach min-verbs below the Munction floor of 4", "SST-MUN-BOUND", true); sheet.reach.minVerbs = 4; }
        if (sheet.reach.maxVerbs > 16) { errAt(sheet.reach.line, "NSS-E-MUN-004",
              "reach max-verbs above the Munction ceiling of 16", "SST-MUN-BOUND", true); sheet.reach.maxVerbs = 16; }
        exitBlock();
    }

    // ---- measure block (SST.SYNCHRO.md, Synchro 1.3) -------------------
    void parseMeasure() {
        sheet.measure.present = true;
        sheet.measure.line = cur().line;
        i++;                                   // 'measure'
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int ln = cur().line;
            std::string key = cur().text; i++;
            if (is(Tok::Colon)) i++;
            if (key == "metrics" || key == "metric") {
                for (const auto& name : readListOrScalar()) {
                    MeasureMetric mm;
                    if (!measureMetricFromName(name, mm)) {
                        errAt(ln, "NSS-E-SYN-001", "unknown Synchro metric '" + name + "'",
                              "SST-SYN-METRIC", true);
                    } else if (std::find(sheet.measure.metrics.begin(), sheet.measure.metrics.end(), mm)
                               == sheet.measure.metrics.end()) {
                        sheet.measure.metrics.push_back(mm);
                    }
                }
            } else if (key == "timeout-ms" || key == "timeout") {
                sheet.measure.timeoutMs = (int)std::strtol(readScalarText().c_str(), nullptr, 10);
                sheet.measure.hasTimeout = true;
            } else if (key == "honest") {
                bool h = parseBool(readScalarText(), true);
                // Synchro is honest by construction; a sheet cannot disable it.
                if (!h) errAt(ln, "NSS-E-SYN-002",
                              "Synchro is honest by construction; 'honest false' is not permitted",
                              "SST-SYN-HONEST", true);
                sheet.measure.honest = true;
            } else {
                (void)readScalarText();
            }
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }

    // ---- finance block (SST.FINANCIAL.md) ------------------------------
    void parseFinance() {
        sheet.finance.present = true;
        sheet.finance.line = cur().line;
        i++;                                   // 'finance'
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int ln = cur().line;
            std::string key = cur().text; i++;
            if (is(Tok::Colon)) i++;           // optional ':' after the key
            if (key == "objects" || key == "object" ||
                key == "components" || key == "component") {
                for (const auto& name : readListOrScalar()) {
                    FinanceObject o;
                    if (!financeObjectFromName(name, o)) {
                        errAt(ln, "NSS-E-FIN-001",
                              "unknown finance object '" + name + "'",
                              "SST-FIN-OBJECT", true);
                    } else if (std::find(sheet.finance.objects.begin(),
                                         sheet.finance.objects.end(), o)
                               == sheet.finance.objects.end()) {
                        sheet.finance.objects.push_back(o);
                    }
                }
            } else if (key == "currency") {
                sheet.finance.currency = readScalarText();
                sheet.finance.hasCurrency = !sheet.finance.currency.empty();
            } else if (key == "period" || key == "period-convention") {
                std::string v = readScalarText();
                if (!financePeriodFromName(v, sheet.finance.period))
                    errAt(ln, "NSS-E-FIN-002",
                          "unknown finance period convention '" + v + "'",
                          "SST-FIN-PERIOD", true);
                else
                    sheet.finance.hasPeriod = true;
            } else if (key == "discounting") {
                std::string v = readScalarText();
                if (!financeDiscountingFromName(v, sheet.finance.discounting))
                    errAt(ln, "NSS-E-FIN-003",
                          "unknown finance discounting '" + v + "'",
                          "SST-FIN-DISCOUNTING", true);
            } else {
                // unknown finance directive: ignore the value
                (void)readScalarText();
            }
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }

    // ---- subject block (NS-SST-0001 2.0 semantic layer) ----------------
    // A subject declares identity/domain/dependencies plus repeatable nested
    // quantity/assumption/relation/transformation/comparison/evidence/
    // explanation/todo blocks. It fills a nordshrift::semantic::Subject and is
    // attached to sheet.subjects. Canonical chain: Subject -> Quantity -> Unit
    // -> Assumption -> Relation -> Formula -> Transformation -> Result ->
    // ComparativeNorm -> Evidence -> Explanation -> Validation.
    void parseSubject() {
        using namespace semantic;
        Subject subj;
        int subjLine = cur().line;
        i++;                                   // 'subject'
        if (is(Tok::Ident) || is(Tok::String)) subj.identity = take().text;
        if (!enterBlock()) { syncToNewline();
            if (!subj.identity.empty()) sheet.subjects.push_back(std::move(subj));
            return;
        }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int ln = cur().line;
            std::string key = cur().text; i++;
            // Scalar keys consume their own ':'; nested-block keys leave the
            // ':' for the sub-parser's enterBlock() (a named block like
            // `quantity range:` puts the name before the ':').
            if (key == "domain")             { if (is(Tok::Colon)) i++; subj.domain = readScalarText(); }
            else if (key == "depends" || key == "dependencies")
                                             { if (is(Tok::Colon)) i++; subj.dependencies = readListOrScalar(); }
            else if (key == "quantity")      parseQuantity(subj);
            else if (key == "assumption")    parseAssumption(subj);
            else if (key == "relation")      parseRelation(subj);
            else if (key == "transformation")parseTransformation(subj);
            else if (key == "comparison")    parseComparison(subj);
            else if (key == "evidence")      parseEvidence(subj);
            else if (key == "explanation")   parseExplanation(subj);
            else if (key == "todo")          parseTodo(subj, ln);
            else { if (is(Tok::Colon)) i++; (void)readScalarText(); }
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
        if (subj.identity.empty())
            errAt(subjLine, "NSS-E-SUB-001", "subject block without an identity", "SST-SUB-IDENTITY", true);
        else
            sheet.subjects.push_back(std::move(subj));
    }

    // Read an EvidenceStatus scalar with validation (default Specified).
    semantic::EvidenceStatus readEvidence(int ln) {
        semantic::EvidenceStatus st = semantic::EvidenceStatus::Specified;
        std::string v = readScalarText();
        if (!v.empty() && !semantic::evidenceStatusFromName(v, st))
            errAt(ln, "NSS-E-SUB-002", "unknown evidence status '" + v + "'", "SST-SUB-EVIDENCE", true);
        return st;
    }

    void parseQuantity(semantic::Subject& subj) {
        semantic::Quantity q;
        if (is(Tok::Ident) || is(Tok::String)) q.identity = take().text;
        if (enterBlock()) {
            while (!atBlockEnd()) {
                if (is(Tok::Newline)) { i++; continue; }
                if (!is(Tok::Ident)) { syncToNewline(); continue; }
                int ln = cur().line;
                std::string k = cur().text; i++;
                if (is(Tok::Colon)) i++;
                if (k == "value")          q.value = readScalarText();
                else if (k == "unit")      q.unit = readScalarText();
                else if (k == "dimension") q.dimension = readScalarText();
                else if (k == "domain")    q.domain = readScalarText();
                else if (k == "status")    q.status = readEvidence(ln);
                else { (void)readScalarText(); }
                if (is(Tok::Newline)) i++;
            }
            exitBlock();
        }
        subj.quantities.push_back(std::move(q));
    }

    void parseAssumption(semantic::Subject& subj) {
        semantic::Assumption a;
        if (is(Tok::Ident) || is(Tok::String)) a.identity = take().text;
        if (enterBlock()) {
            while (!atBlockEnd()) {
                if (is(Tok::Newline)) { i++; continue; }
                if (!is(Tok::Ident)) { syncToNewline(); continue; }
                int ln = cur().line;
                std::string k = cur().text; i++;
                if (is(Tok::Colon)) i++;
                if (k == "statement")  a.statement = readScalarText();
                else if (k == "scope") a.scope = readScalarText();
                else if (k == "source")a.source = readScalarText();
                else if (k == "status")a.status = readEvidence(ln);
                else { (void)readScalarText(); }
                if (is(Tok::Newline)) i++;
            }
            exitBlock();
        }
        subj.assumptions.push_back(std::move(a));
    }

    void parseRelation(semantic::Subject& subj) {
        semantic::Relation r;
        if (is(Tok::Ident) || is(Tok::String)) r.identity = take().text;
        if (enterBlock()) {
            while (!atBlockEnd()) {
                if (is(Tok::Newline)) { i++; continue; }
                if (!is(Tok::Ident)) { syncToNewline(); continue; }
                std::string k = cur().text; i++;
                if (is(Tok::Colon)) i++;
                if (k == "formula")       r.formula = readScalarText();
                else if (k == "inputs")   r.inputs = readListOrScalar();
                else if (k == "outputs")  r.outputs = readListOrScalar();
                else { (void)readScalarText(); }
                if (is(Tok::Newline)) i++;
            }
            exitBlock();
        }
        subj.relations.push_back(std::move(r));
    }

    void parseTransformation(semantic::Subject& subj) {
        semantic::Transformation t;
        if (is(Tok::Ident) || is(Tok::String)) t.identity = take().text;
        if (enterBlock()) {
            while (!atBlockEnd()) {
                if (is(Tok::Newline)) { i++; continue; }
                if (!is(Tok::Ident)) { syncToNewline(); continue; }
                std::string k = cur().text; i++;
                if (is(Tok::Colon)) i++;
                if (k == "source")           t.source = readScalarText();
                else if (k == "operation")   t.operation = readScalarText();
                else if (k == "parameters")  t.parameters = readScalarText();
                else if (k == "destination") t.destination = readScalarText();
                else if (k == "approximate") t.approximate = parseBool(readScalarText(), false);
                else { (void)readScalarText(); }
                if (is(Tok::Newline)) i++;
            }
            exitBlock();
        }
        subj.transformations.push_back(std::move(t));
    }

    void parseComparison(semantic::Subject& subj) {
        semantic::ComparativeNorm c;
        if (enterBlock()) {
            while (!atBlockEnd()) {
                if (is(Tok::Newline)) { i++; continue; }
                if (!is(Tok::Ident)) { syncToNewline(); continue; }
                std::string k = cur().text; i++;
                if (is(Tok::Colon)) i++;
                if (k == "prior")          c.prior_subject = readScalarText();
                else if (k == "current")   c.current_subject = readScalarText();
                else if (k == "reference") c.reference_subject = readScalarText();
                else if (k == "norm")      c.norm = readScalarText();
                else { (void)readScalarText(); }
                if (is(Tok::Newline)) i++;
            }
            exitBlock();
        }
        subj.comparisons.push_back(std::move(c));
    }

    void parseEvidence(semantic::Subject& subj) {
        semantic::Evidence e;
        if (enterBlock()) {
            while (!atBlockEnd()) {
                if (is(Tok::Newline)) { i++; continue; }
                if (!is(Tok::Ident)) { syncToNewline(); continue; }
                int ln = cur().line;
                std::string k = cur().text; i++;
                if (is(Tok::Colon)) i++;
                if (k == "status")     e.status = readEvidence(ln);
                else if (k == "source")e.source = readScalarText();
                else if (k == "note")  e.note = readScalarText();
                else { (void)readScalarText(); }
                if (is(Tok::Newline)) i++;
            }
            exitBlock();
        }
        subj.evidence.push_back(std::move(e));
    }

    void parseExplanation(semantic::Subject& subj) {
        semantic::Explanation ex;
        if (is(Tok::Ident) || is(Tok::String)) ex.subject = take().text;
        if (enterBlock()) {
            while (!atBlockEnd()) {
                if (is(Tok::Newline)) { i++; continue; }
                if (!is(Tok::Ident)) { syncToNewline(); continue; }
                std::string k = cur().text; i++;
                if (is(Tok::Colon)) i++;
                if (k == "steps")      ex.steps = readListOrScalar();
                else if (k == "step")  { for (auto& s : readListOrScalar()) ex.steps.push_back(s); }
                else if (k == "subject") ex.subject = readScalarText();
                else { (void)readScalarText(); }
                if (is(Tok::Newline)) i++;
            }
            exitBlock();
        }
        subj.explanations.push_back(std::move(ex));
    }

    void parseTodo(semantic::Subject& subj, int todoLine) {
        semantic::Todo td;
        td.subject = subj.identity;
        if (is(Tok::Ident) || is(Tok::String)) td.identity = take().text;
        if (enterBlock()) {
            while (!atBlockEnd()) {
                if (is(Tok::Newline)) { i++; continue; }
                if (!is(Tok::Ident)) { syncToNewline(); continue; }
                int ln = cur().line;
                std::string k = cur().text; i++;
                if (is(Tok::Colon)) i++;
                if (k == "priority")             { std::string v = readScalarText(); td.priority = v.empty()?0:std::stoi(v); }
                else if (k == "depends" || k == "dependencies") td.dependencies = readListOrScalar();
                else if (k == "preconditions")   td.preconditions = readScalarText();
                else if (k == "action")          td.action = readScalarText();
                else if (k == "expected" || k == "expected-result") td.expected_result = readScalarText();
                else if (k == "validation")      td.validation = readScalarText();
                else if (k == "status") {
                    std::string v = readScalarText();
                    if (!v.empty() && !semantic::workStatusFromName(v, td.status))
                        errAt(ln, "NSS-E-SUB-003", "unknown work status '" + v + "'", "SST-SUB-WORKSTATUS", true);
                }
                else { (void)readScalarText(); }
                if (is(Tok::Newline)) i++;
            }
            exitBlock();
        }
        (void)todoLine;
        subj.workplan.push_back(std::move(td));
    }

    // ---- inline rule block (§8.6) --------------------------------------
    void parseRuleBlock() {
        RuleConfig rc; rc.line = cur().line;
        i++;                                   // 'rule'
        if (is(Tok::Ident)) rc.name = take().text;
        if (enterBlock()) {
            while (!atBlockEnd()) {
                if (is(Tok::Newline)) { i++; continue; }
                if (!is(Tok::Ident)) { syncToNewline(); continue; }
                std::string key = cur().text; i++;
                if (key == "severity") { rc.severity = parseSeverity(readScalarText()); rc.hasSeverity = true; }
                else if (key == "config") {
                    if (enterBlock()) {
                        while (!atBlockEnd()) {
                            if (is(Tok::Newline)) { i++; continue; }
                            if (!is(Tok::Ident)) { syncToNewline(); continue; }
                            std::string k = take().text;
                            std::string v = readScalarText();
                            rc.config[k] = v;
                            if (is(Tok::Newline)) i++;
                        }
                        exitBlock();
                    }
                    continue;
                }
                if (is(Tok::Newline)) i++;
            }
            exitBlock();
        }
        sheet.ruleBlocks.push_back(rc);
    }

    // ---- profile (§XIII) -----------------------------------------------
    void parseProfile() {
        Profile pf; pf.line = cur().line;
        i++;                                   // 'profile'
        if (is(Tok::Ident)) pf.name = take().text;
        pf.overlay = std::make_shared<Sheet>();
        if (enterBlock()) {
            // optional 'inherits <identifier>'
            if (isIdent("inherits")) {
                i++;
                if (is(Tok::Ident)) { pf.inherits = take().text; pf.hasInherits = true; }
                // PROF-02: a second inherits (or a list) is multiple-parent
                if (is(Tok::Comma) || is(Tok::Ident))
                    errAt(pf.line, "NSS-E-0110", "profile '" + pf.name + "' declares multiple parents", "PROF-02");
                if (is(Tok::Newline)) i++;
            }
            // nested sections override into the overlay sheet
            Sheet* saved = &sheet;
            // Parse nested sections directly into pf.overlay by temporarily
            // retargeting: simplest is to parse into local structures.
            while (!atBlockEnd()) {
                if (is(Tok::Newline)) { i++; continue; }
                if (!is(Tok::Ident)) { syncToNewline(); continue; }
                const std::string& kw = cur().text;
                if (kw == "pipeline") parsePipeline(pf.overlay->pipeline);
                else if (kw == "target") { parseTargetInto(pf.overlay->target); }
                else if (kw == "guards") parseGuards(pf.overlay->guards);
                else if (kw == "derive") parseDerive(pf.overlay->derive);
                else { syncToNewline(); }
            }
            (void)saved;
            exitBlock();
        }
        // PROF-03 circular inheritance is checked during resolution, not here.
        sheet.profiles.push_back(pf);
    }

    // target parser variant writing into an arbitrary Target (for profiles)
    void parseTargetInto(Target& tg) {
        tg.present = true; tg.line = cur().line;
        i++;
        if (!enterBlock()) { syncToNewline(); return; }
        while (!atBlockEnd()) {
            if (is(Tok::Newline)) { i++; continue; }
            if (!is(Tok::Ident)) { syncToNewline(); continue; }
            int ln = cur().line;
            std::string key = cur().text; i++;
            if (key == "root")        { tg.root = readScalarText(); tg.hasRoot = true; }
            else if (key == "layout") { tg.layout = parseLayout(readScalarText()); tg.hasLayout = true; }
            else if (key == "java-version") { std::string v = readScalarText(); tg.javaVersion = v.empty()?0:std::stoi(v); tg.hasJavaVersion = true; if (tg.javaVersion && tg.javaVersion < 17) errAt(ln, "NSS-E-0040", "target.java-version must be >= 17", "TGT-01"); }
            else if (key == "overwrite")  tg.overwrite = parseBool(readScalarText(), true);
            else if (key == "source-map") tg.sourceMap = parseBool(readScalarText(), true);
            else if (key == "optimize")   tg.optimize = parseBool(readScalarText(), false);
            else if (key == "clean")      tg.clean = parseBool(readScalarText(), false);
            else if (key == "target-language") { std::string v = readScalarText(); tg.language = (v=="sleela")?TargetLang::Sleela:(v=="c")?TargetLang::C:TargetLang::Java; }
            if (is(Tok::Newline)) i++;
        }
        exitBlock();
    }
};

} // namespace

// ---------------------------------------------------------------------------
// Closed network object series mappings (SST.NETWORK.md / NS-SST-NET-0001).
// ---------------------------------------------------------------------------
bool networkObjectFromName(const std::string& s, NetworkObject& out) {
    if (s == "Endpoint")     { out = NetworkObject::Endpoint;     return true; }
    if (s == "NIC")          { out = NetworkObject::NIC;          return true; }
    if (s == "Link")         { out = NetworkObject::Link;         return true; }
    if (s == "Packet")       { out = NetworkObject::Packet;       return true; }
    if (s == "Queue")        { out = NetworkObject::Queue;        return true; }
    if (s == "Switch")       { out = NetworkObject::Switch;       return true; }
    if (s == "Router")       { out = NetworkObject::Router;       return true; }
    if (s == "Fabric")       { out = NetworkObject::Fabric;       return true; }
    if (s == "Listener")     { out = NetworkObject::Listener;     return true; }
    if (s == "Connector")    { out = NetworkObject::Connector;    return true; }
    if (s == "Gateway")      { out = NetworkObject::Gateway;      return true; }
    if (s == "LoadBalancer") { out = NetworkObject::LoadBalancer; return true; }
    if (s == "Service")      { out = NetworkObject::Service;      return true; }
    if (s == "TLS")          { out = NetworkObject::TLS;          return true; }
    if (s == "DNS")          { out = NetworkObject::DNS;          return true; }
    return false;
}
bool networkTransportFromName(const std::string& s, NetworkTransport& out) {
    if (s == "tcp")  { out = NetworkTransport::TCP;  return true; }
    if (s == "udp")  { out = NetworkTransport::UDP;  return true; }
    if (s == "tls")  { out = NetworkTransport::TLS;  return true; }
    if (s == "quic") { out = NetworkTransport::QUIC; return true; }
    return false;
}
bool networkAddressFamilyFromName(const std::string& s, NetworkAddressFamily& out) {
    if (s == "ipv4") { out = NetworkAddressFamily::IPv4; return true; }
    if (s == "ipv6") { out = NetworkAddressFamily::IPv6; return true; }
    if (s == "dual") { out = NetworkAddressFamily::Dual; return true; }
    return false;
}
const char* networkObjectName(NetworkObject o) {
    switch (o) {
        case NetworkObject::Endpoint:     return "Endpoint";
        case NetworkObject::NIC:          return "NIC";
        case NetworkObject::Link:         return "Link";
        case NetworkObject::Packet:       return "Packet";
        case NetworkObject::Queue:        return "Queue";
        case NetworkObject::Switch:       return "Switch";
        case NetworkObject::Router:       return "Router";
        case NetworkObject::Fabric:       return "Fabric";
        case NetworkObject::Listener:     return "Listener";
        case NetworkObject::Connector:    return "Connector";
        case NetworkObject::Gateway:      return "Gateway";
        case NetworkObject::LoadBalancer: return "LoadBalancer";
        case NetworkObject::Service:      return "Service";
        case NetworkObject::TLS:          return "TLS";
        case NetworkObject::DNS:          return "DNS";
    }
    return "?";
}
const char* networkTransportName(NetworkTransport t) {
    switch (t) {
        case NetworkTransport::TCP:  return "tcp";
        case NetworkTransport::UDP:  return "udp";
        case NetworkTransport::TLS:  return "tls";
        case NetworkTransport::QUIC: return "quic";
    }
    return "?";
}
const char* networkAddressFamilyName(NetworkAddressFamily a) {
    switch (a) {
        case NetworkAddressFamily::IPv4: return "ipv4";
        case NetworkAddressFamily::IPv6: return "ipv6";
        case NetworkAddressFamily::Dual: return "dual";
    }
    return "?";
}

// ---------------------------------------------------------------------------
// Closed finance object series mappings (SST.FINANCIAL.md).
// ---------------------------------------------------------------------------
bool financeObjectFromName(const std::string& s, FinanceObject& out) {
    if (s == "FutureValue")       { out = FinanceObject::FutureValue;       return true; }
    if (s == "PresentValue")      { out = FinanceObject::PresentValue;      return true; }
    if (s == "AnnuityPresent")    { out = FinanceObject::AnnuityPresent;    return true; }
    if (s == "AnnuityFuture")     { out = FinanceObject::AnnuityFuture;     return true; }
    if (s == "NetPresentValue")   { out = FinanceObject::NetPresentValue;   return true; }
    if (s == "BondPrice")         { out = FinanceObject::BondPrice;         return true; }
    if (s == "CAPM")              { out = FinanceObject::CAPM;              return true; }
    if (s == "WACC")              { out = FinanceObject::WACC;              return true; }
    if (s == "Determinant2x2")    { out = FinanceObject::Determinant2x2;    return true; }
    if (s == "LinearSolve2x2")    { out = FinanceObject::LinearSolve2x2;    return true; }
    if (s == "QuadraticEquation") { out = FinanceObject::QuadraticEquation; return true; }
    if (s == "Ratio")             { out = FinanceObject::Ratio;             return true; }
    return false;
}
bool financePeriodFromName(const std::string& s, FinancePeriodConvention& out) {
    if (s == "annual")      { out = FinancePeriodConvention::Annual;      return true; }
    if (s == "semi-annual") { out = FinancePeriodConvention::SemiAnnual;  return true; }
    if (s == "quarterly")   { out = FinancePeriodConvention::Quarterly;   return true; }
    if (s == "monthly")     { out = FinancePeriodConvention::Monthly;     return true; }
    if (s == "continuous")  { out = FinancePeriodConvention::Continuous;  return true; }
    return false;
}
bool financeDiscountingFromName(const std::string& s, FinanceDiscounting& out) {
    if (s == "discrete")   { out = FinanceDiscounting::Discrete;   return true; }
    if (s == "continuous") { out = FinanceDiscounting::Continuous; return true; }
    return false;
}
const char* financeObjectName(FinanceObject o) {
    switch (o) {
        case FinanceObject::FutureValue:       return "FutureValue";
        case FinanceObject::PresentValue:      return "PresentValue";
        case FinanceObject::AnnuityPresent:    return "AnnuityPresent";
        case FinanceObject::AnnuityFuture:     return "AnnuityFuture";
        case FinanceObject::NetPresentValue:   return "NetPresentValue";
        case FinanceObject::BondPrice:         return "BondPrice";
        case FinanceObject::CAPM:              return "CAPM";
        case FinanceObject::WACC:              return "WACC";
        case FinanceObject::Determinant2x2:    return "Determinant2x2";
        case FinanceObject::LinearSolve2x2:    return "LinearSolve2x2";
        case FinanceObject::QuadraticEquation: return "QuadraticEquation";
        case FinanceObject::Ratio:             return "Ratio";
    }
    return "?";
}
const char* financePeriodName(FinancePeriodConvention p) {
    switch (p) {
        case FinancePeriodConvention::Annual:     return "annual";
        case FinancePeriodConvention::SemiAnnual: return "semi-annual";
        case FinancePeriodConvention::Quarterly:  return "quarterly";
        case FinancePeriodConvention::Monthly:    return "monthly";
        case FinancePeriodConvention::Continuous: return "continuous";
    }
    return "?";
}
const char* financeDiscountingName(FinanceDiscounting d) {
    switch (d) {
        case FinanceDiscounting::Discrete:   return "discrete";
        case FinanceDiscounting::Continuous: return "continuous";
    }
    return "?";
}

// ---- Munction reach enum <-> name mappings (1.3) --------------------------
bool reachVerbFromName(const std::string& s, ReachVerb& out) {
    if (s == "start")            { out = ReachVerb::Start;            return true; }
    if (s == "connect")          { out = ReachVerb::Connect;          return true; }
    if (s == "open")             { out = ReachVerb::Open;             return true; }
    if (s == "enable")           { out = ReachVerb::Enable;           return true; }
    if (s == "send")             { out = ReachVerb::Send;             return true; }
    if (s == "thatch")           { out = ReachVerb::Thatch;           return true; }
    if (s == "consume")          { out = ReachVerb::Consume;          return true; }
    if (s == "observe")          { out = ReachVerb::Observe;          return true; }
    if (s == "propagate")        { out = ReachVerb::Propagate;        return true; }
    if (s == "latch")            { out = ReachVerb::Latch;            return true; }
    if (s == "contain")          { out = ReachVerb::Contain;          return true; }
    if (s == "close")            { out = ReachVerb::Close;            return true; }
    if (s == "closeWithReceipt") { out = ReachVerb::CloseWithReceipt; return true; }
    if (s == "abort")            { out = ReachVerb::Abort;            return true; }
    return false;
}
const char* reachVerbName(ReachVerb v) {
    switch (v) {
        case ReachVerb::Start: return "start"; case ReachVerb::Connect: return "connect";
        case ReachVerb::Open: return "open"; case ReachVerb::Enable: return "enable";
        case ReachVerb::Send: return "send"; case ReachVerb::Thatch: return "thatch";
        case ReachVerb::Consume: return "consume"; case ReachVerb::Observe: return "observe";
        case ReachVerb::Propagate: return "propagate"; case ReachVerb::Latch: return "latch";
        case ReachVerb::Contain: return "contain"; case ReachVerb::Close: return "close";
        case ReachVerb::CloseWithReceipt: return "closeWithReceipt"; case ReachVerb::Abort: return "abort";
    }
    return "?";
}
bool reachChannelFromName(const std::string& s, ReachChannel& out) {
    if (s == "pipe")   { out = ReachChannel::Pipe;   return true; }
    if (s == "file")   { out = ReachChannel::File;   return true; }
    if (s == "tcp")    { out = ReachChannel::Tcp;    return true; }
    if (s == "http")   { out = ReachChannel::Http;   return true; }
    if (s == "sdps")   { out = ReachChannel::Sdps;   return true; }
    if (s == "crypto") { out = ReachChannel::Crypto; return true; }
    return false;
}
const char* reachChannelName(ReachChannel c) {
    switch (c) {
        case ReachChannel::Pipe: return "pipe"; case ReachChannel::File: return "file";
        case ReachChannel::Tcp: return "tcp"; case ReachChannel::Http: return "http";
        case ReachChannel::Sdps: return "sdps"; case ReachChannel::Crypto: return "crypto";
    }
    return "?";
}

// ---- Synchro measure enum <-> name mappings (1.3) -------------------------
bool measureMetricFromName(const std::string& s, MeasureMetric& out) {
    if (s == "sent")     { out = MeasureMetric::Sent;     return true; }
    if (s == "received") { out = MeasureMetric::Received; return true; }
    if (s == "loss")     { out = MeasureMetric::Loss;     return true; }
    if (s == "mean")     { out = MeasureMetric::Mean;     return true; }
    if (s == "min")      { out = MeasureMetric::Min;      return true; }
    if (s == "max")      { out = MeasureMetric::Max;      return true; }
    if (s == "p95")      { out = MeasureMetric::P95;      return true; }
    if (s == "report")   { out = MeasureMetric::Report;   return true; }
    return false;
}
const char* measureMetricName(MeasureMetric m) {
    switch (m) {
        case MeasureMetric::Sent: return "sent"; case MeasureMetric::Received: return "received";
        case MeasureMetric::Loss: return "loss"; case MeasureMetric::Mean: return "mean";
        case MeasureMetric::Min: return "min"; case MeasureMetric::Max: return "max";
        case MeasureMetric::P95: return "p95"; case MeasureMetric::Report: return "report";
    }
    return "?";
}

Sheet parseSheet(const std::vector<Token>& toks, const std::string& file, DiagnosticBag& diags) {
    Parser p(toks, file, diags);
    return p.run();
}

} // namespace nordshrift
