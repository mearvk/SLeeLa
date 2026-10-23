// ===========================================================================
// xlt_parser.cpp  --  Implementation of the XLT (.xlt) parser.
// ===========================================================================
#include "xlt_parser.h"

namespace xlt {

namespace {

// Decode an integer-literal token text that may carry a radix prefix
// (0x hex, 0b binary, 0o octal) or be plain decimal -- parity with the SLeeLa
// frontend (SLEELA.syntax 1.4). Underscores are already stripped by the lexer.
long long decodeInt(const std::string& t) {
    if (t.size() > 2 && t[0] == '0' && (t[1] == 'b' || t[1] == 'B'))
        return std::stoll(t.substr(2), nullptr, 2);
    if (t.size() > 2 && t[0] == '0' && (t[1] == 'o' || t[1] == 'O'))
        return std::stoll(t.substr(2), nullptr, 8);
    // base 0 auto-detects 0x hex and a leading-0 octal; plain decimal otherwise.
    return std::stoll(t, nullptr, 0);
}

class Parser {
public:
    Parser(const std::vector<Token>& toks, const std::string& file, DiagnosticBag& diags)
        : toks_(toks), file_(file), diags_(diags) {}

    XltModel parse() {
        XltModel m;
        parsePragma(m);
        parseModel(m);
        return m;
    }

private:
    const std::vector<Token>& toks_;
    std::string file_;
    DiagnosticBag& diags_;
    size_t p_ = 0;

    const Token& cur() const { return toks_[p_]; }
    const Token& peek(int n = 1) const {
        size_t q = p_ + n; return q < toks_.size() ? toks_[q] : toks_.back();
    }
    void advance() { if (p_ + 1 < toks_.size()) p_++; }
    bool is(Tok k) const { return cur().kind == k; }

    void err(const std::string& code, const std::string& msg, const std::string& rule) {
        diags_.error(code, file_, cur().line, msg, rule);
    }

    // #xlt 1.0
    void parsePragma(XltModel& m) {
        if (is(Tok::Pragma) && cur().text == "xlt") {
            m.headerSeen = true;
            advance();
            if (is(Tok::Version) || is(Tok::Integer)) { m.version = cur().text; advance(); }
            else { err("XLT-E-0010", "#xlt pragma is missing a version", "XLT-SRC-01"); }
        } else {
            err("XLT-E-0011", "sheet must begin with an #xlt pragma", "XLT-SRC-01");
        }
    }

    // model IDENT { ... }
    void parseModel(XltModel& m) {
        if (!(is(Tok::Ident) && cur().text == "model")) {
            err("XLT-E-0012", "expected 'model' declaration", "XLT-SRC-02");
            return;
        }
        advance();
        if (is(Tok::Ident)) { m.name = cur().text; advance(); }
        else { err("XLT-E-0013", "'model' must be followed by a name", "XLT-SRC-02"); }
        if (!is(Tok::LBrace)) { err("XLT-E-0014", "expected '{' to open the model body", "XLT-SRC-02"); return; }
        advance();
        m.modelSeen = true;
        while (!is(Tok::RBrace) && !is(Tok::Eof)) { parseDirective(m); }
        if (is(Tok::RBrace)) advance();
        else err("XLT-E-0015", "model body is not closed with '}'", "XLT-SRC-02");
    }

    // one `slot "ref" [int];` or `target ident;`
    void parseDirective(XltModel& m) {
        if (!is(Tok::Ident)) { err("XLT-E-0016", "expected a directive keyword", "XLT-SRC-03"); advance(); return; }
        std::string kw = cur().text;
        int kwLine = cur().line;
        advance();

        if (kw == "target") {
            if (is(Tok::Ident)) { m.target = cur().text; m.targetLine = kwLine; advance(); }
            else err("XLT-E-0017", "'target' expects a target name", "XLT-SRC-04");
            expectSemicolon();
            return;
        }

        // memory tiers: `memtier "ref" usual|accurate;`
        if (kw == "memtier") {
            std::string ref;
            if (is(Tok::String)) { ref = cur().text; advance(); }
            else err("XLT-E-0023", "'memtier' expects a quoted memory reference", "XLT-TIER-01");
            std::string which;
            if (is(Tok::Ident)) { which = cur().text; advance(); }
            else err("XLT-E-0024", "'memtier' expects a tier name (usual|accurate)", "XLT-TIER-01");
            if (which == "usual") {
                if (m.usualTier.present) err("XLT-E-0025", "duplicate 'usual' memtier breaks document orthogonality", "XLT-ORTHO-01");
                m.usualTier.ref = ref; m.usualTier.present = true; m.usualTier.line = kwLine;
            } else if (which == "accurate") {
                if (m.accurateTier.present) err("XLT-E-0025", "duplicate 'accurate' memtier breaks document orthogonality", "XLT-ORTHO-01");
                m.accurateTier.ref = ref; m.accurateTier.present = true; m.accurateTier.line = kwLine;
            } else if (!which.empty()) {
                err("XLT-E-0026", "memtier name must be 'usual' or 'accurate', got '" + which + "'", "XLT-TIER-01");
            }
            expectSemicolon();
            return;
        }
        // main tier: `maintier "usual|accurate";`
        if (kw == "maintier") {
            if (!m.mainTier.empty()) err("XLT-E-0027", "duplicate 'maintier' breaks document orthogonality", "XLT-ORTHO-01");
            if (is(Tok::String)) { m.mainTier = cur().text; m.mainTierLine = kwLine; advance(); }
            else err("XLT-E-0028", "'maintier' expects a quoted tier name", "XLT-TIER-01");
            expectSemicolon();
            return;
        }

        // program: `program "path" <sleela|c|cpp|java>;`
        if (kw == "program") {
            if (!m.programPath.empty()) err("XLT-E-0029", "duplicate 'program' breaks document orthogonality", "XLT-ORTHO-01");
            if (is(Tok::String)) { m.programPath = cur().text; m.programLine = kwLine; advance(); }
            else err("XLT-E-0030", "'program' expects a quoted source path", "XLT-PROG-01");
            if (is(Tok::Ident)) { m.programLang = cur().text; advance(); }
            else err("XLT-E-0031", "'program' expects a language (sleela|c|cpp|java)", "XLT-PROG-01");
            expectSemicolon();
            return;
        }
        // grant: `grant <words:int> <budget:int>;`
        if (kw == "grant") {
            if (m.grantWords != 0 || m.grantBudget != 0) err("XLT-E-0032", "duplicate 'grant' breaks document orthogonality", "XLT-ORTHO-01");
            m.grantLine = kwLine;
            if (is(Tok::Integer)) { m.grantWords = decodeInt(cur().text); advance(); }
            else err("XLT-E-0033", "'grant' expects a memory-words integer", "XLT-PROG-02");
            if (is(Tok::Integer)) { m.grantBudget = decodeInt(cur().text); advance(); }
            else err("XLT-E-0034", "'grant' expects a cycle-budget integer", "XLT-PROG-02");
            expectSemicolon();
            return;
        }

        // bare capability directives: keyword directly followed by ';'.
        if (kw == "gpu" || kw == "dma" || kw == "nativeram") {
            bool* flag = (kw == "gpu") ? &m.gpu : (kw == "dma") ? &m.dma : &m.nativeRam;
            int*  fln  = (kw == "gpu") ? &m.gpuLine : (kw == "dma") ? &m.dmaLine : &m.nativeRamLine;
            if (*flag) err("XLT-E-0022", "duplicate '" + kw + "' capability breaks document orthogonality", "XLT-ORTHO-01");
            *flag = true; *fln = kwLine;
            expectSemicolon();
            return;
        }

        // part slots take a quoted ref.
        Slot* slot = slotFor(m, kw);
        if (slot == nullptr) {
            err("XLT-E-0018", "unknown directive '" + kw + "'", "XLT-SRC-03");
            skipToSemicolon();
            return;
        }
        if (slot->present) {
            err("XLT-E-0020", "duplicate '" + kw + "' declaration breaks document orthogonality", "XLT-ORTHO-01");
        }
        if (is(Tok::String)) {
            slot->ref = cur().text; slot->present = true; slot->line = kwLine; advance();
        } else {
            err("XLT-E-0019", "'" + kw + "' expects a quoted part reference", "XLT-SRC-03");
        }
        // memory takes an optional integer capacity.
        if (kw == "memory" && is(Tok::Integer)) {
            m.memoryCapMiB = (int)decodeInt(cur().text); advance();
        }
        expectSemicolon();
    }

    Slot* slotFor(XltModel& m, const std::string& kw) {
        if (kw == "cpu")     return &m.cpu;
        if (kw == "board")   return &m.board;
        if (kw == "memory")  return &m.memory;
        if (kw == "cadence") return &m.cadence;
        if (kw == "storage") return &m.storage;
        if (kw == "bus")     return &m.bus;
        return nullptr;
    }

    void expectSemicolon() {
        if (is(Tok::Semicolon)) advance();
        else err("XLT-E-0021", "directive must end with ';'", "XLT-SRC-03");
    }
    void skipToSemicolon() {
        while (!is(Tok::Semicolon) && !is(Tok::RBrace) && !is(Tok::Eof)) advance();
        if (is(Tok::Semicolon)) advance();
    }
};

} // namespace

XltModel parseModel(const std::vector<Token>& toks, const std::string& file,
                    DiagnosticBag& diags) {
    Parser parser(toks, file, diags);
    return parser.parse();
}

} // namespace xlt
