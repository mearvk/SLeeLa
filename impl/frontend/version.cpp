// ===========================================================================
// version.cpp  --  Implementation of Sleela syntax-version awareness (SL-META
// 0001 Section 4.4).
// ===========================================================================
#include "version.h"

#include <cctype>

namespace sleela {

bool parseSyntaxVersion(const std::string& text, SyntaxVersion& out) {
    // Accept MAJOR.MINOR or MAJOR.MINOR.PATCH; MAJOR and MINOR are required.
    size_t i = 0;
    auto readInt = [&](int& v) -> bool {
        if (i >= text.size() || !std::isdigit((unsigned char)text[i])) return false;
        long n = 0;
        size_t start = i;
        while (i < text.size() && std::isdigit((unsigned char)text[i])) {
            n = n * 10 + (text[i] - '0');
            if (n > 1000000) return false;  // guard absurd input
            i++;
        }
        (void)start;
        v = (int)n;
        return true;
    };

    SyntaxVersion v;
    if (!readInt(v.major)) return false;
    if (i >= text.size() || text[i] != '.') return false;
    i++;  // '.'
    if (!readInt(v.minor)) return false;
    // Optional .PATCH -- parsed and ignored (Section 4.4: PATCH is not part of
    // the syntax version).
    if (i < text.size() && text[i] == '.') {
        i++;
        int patch = 0;
        if (!readInt(patch)) return false;
    }
    // Any trailing garbage is a malformed version.
    if (i != text.size()) return false;

    out = v;
    return true;
}

namespace {

// Advance `p` over ASCII whitespace (including newlines).
void skipWs(const std::string& s, size_t& p) {
    while (p < s.size() &&
           (s[p] == ' ' || s[p] == '\t' || s[p] == '\r' || s[p] == '\n'))
        p++;
}

// From `p`, skip over any run of whitespace and full comments so that `p` lands
// on the first byte of the first non-blank, non-comment line. Returns false if
// an unterminated block comment is hit (treated as "no pragma" by the caller).
bool skipTrivia(const std::string& s, size_t& p) {
    for (;;) {
        skipWs(s, p);
        if (p + 1 < s.size() && s[p] == '/' && s[p + 1] == '/') {
            p += 2;
            while (p < s.size() && s[p] != '\n') p++;
        } else if (p + 1 < s.size() && s[p] == '/' && s[p + 1] == '*') {
            p += 2;
            while (p + 1 < s.size() && !(s[p] == '*' && s[p + 1] == '/')) p++;
            if (p + 1 >= s.size()) return false;  // unterminated
            p += 2;
        } else {
            return true;
        }
    }
}

}  // namespace

VersionResolution resolveSyntaxVersion(const std::string& source) {
    VersionResolution r;

    size_t p = 0;
    bool ok = skipTrivia(source, p);

    bool hasPragma = false;
    if (ok && p < source.size() && source[p] == '#') {
        // The first meaningful line begins with '#': it must be the pragma.
        size_t q = p + 1;
        // keyword "sleela" (case-sensitive, per the metadocument spelling)
        const std::string kw = "sleela";
        if (source.compare(q, kw.size(), kw) == 0) {
            hasPragma = true;
            q += kw.size();
        }
        // Also tolerate the whole-line form and report a malformed pragma if the
        // keyword is present but the version text is bad.
        if (hasPragma) {
            // gather the rest of the pragma line
            size_t lineEnd = q;
            while (lineEnd < source.size() && source[lineEnd] != '\n') lineEnd++;
            std::string rest = source.substr(q, lineEnd - q);

            // trim leading/trailing whitespace from `rest`
            size_t a = 0, b = rest.size();
            while (a < b && std::isspace((unsigned char)rest[a])) a++;
            while (b > a && std::isspace((unsigned char)rest[b - 1])) b--;
            std::string verText = rest.substr(a, b - a);

            r.pragmaPresent = true;
            r.raw = verText;

            SyntaxVersion declared;
            if (verText.empty() || !parseSyntaxVersion(verText, declared)) {
                r.status = VersionStatus::Malformed;
                r.declared = defaultSyntaxVersion();
                r.message = "malformed #sleela version pragma: expected "
                            "'#sleela MAJOR.MINOR', got '#sleela " + verText + "'";
                return r;
            }

            r.declared = declared;
            if (declared > maxSupportedSyntax()) {
                r.status = VersionStatus::TooNew;
                r.message = "source declares Sleela syntax " + declared.str() +
                            ", which exceeds this compiler's supported range (" +
                            minSupportedSyntax().str() + " .. " +
                            maxSupportedSyntax().str() +
                            "). Upgrade the compiler or lower the #sleela pragma.";
            } else if (declared < minSupportedSyntax()) {
                r.status = VersionStatus::TooOld;
                r.message = "source declares Sleela syntax " + declared.str() +
                            ", which is older than this compiler's minimum "
                            "supported syntax (" + minSupportedSyntax().str() +
                            ").";
            } else {
                r.status = VersionStatus::OkDeclared;
                r.message = "accepted Sleela syntax " + declared.str();
            }
            return r;
        }
    }

    // No pragma on the first meaningful line: assume the default and warn.
    r.pragmaPresent = false;
    r.status = VersionStatus::OkAssumed;
    r.declared = defaultSyntaxVersion();
    r.message = "no #sleela version pragma found; assuming syntax " +
                defaultSyntaxVersion().str() +
                " (declare it with '#sleela " + defaultSyntaxVersion().str() +
                "' on the first line)";
    return r;
}

}  // namespace sleela
