// ===========================================================================
// sst_parser.cpp  --  Spec-conformant .sst parser (NS-SST-0001 Part XIV).
// ===========================================================================
#include "sst_parser.h"

#include <algorithm>

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
        if (kw == "rule")     { parseRuleBlock(); return true; }
        if (kw == "profile")  { parseProfile();  return true; }
        return false;
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

Sheet parseSheet(const std::vector<Token>& toks, const std::string& file, DiagnosticBag& diags) {
    Parser p(toks, file, diags);
    return p.run();
}

} // namespace nordshrift
