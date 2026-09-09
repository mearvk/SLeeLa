// ===========================================================================
// version.h  --  Syntax-version awareness for the Sleela compiler.
//
// Implements SL-META-0001 Section 4.4 (Syntax Versioning): a Wrapper(TM)
// (a .sleela source file) declares its syntax version via a pragma on the
// first non-blank, non-comment line:
//
//     #sleela 1.0
//
// The pragma accepts MAJOR.MINOR version strings. Per the metadocument, "A
// compiler must reject files whose declared version exceeds the compiler's
// supported version range." This module makes the compiler *version aware*:
// it knows the range of syntax versions it can accept, parses the declared
// version, and decides accept / reject / warn accordingly.
//
// Backward-compatibility (SL-META-0001 Section 4.4):
//   - a MINOR increment may add constructs but not remove/modify existing ones,
//     so any file declaring MAJOR.MINOR <= supported max is accepted;
//   - a MAJOR increment may break the grammar, so a file whose MAJOR exceeds the
//     compiler's supported MAJOR is rejected;
//   - a file declaring a version below the supported floor is rejected as too
//     old for this compiler.
// ===========================================================================
#ifndef SLEELA_VERSION_H
#define SLEELA_VERSION_H

#include <string>

namespace sleela {

// A MAJOR.MINOR syntax version. PATCH is intentionally not part of the syntax
// version (Section 4.4: a PATCH increment introduces no grammar changes).
struct SyntaxVersion {
    int major = 0;
    int minor = 0;

    std::string str() const {
        return std::to_string(major) + "." + std::to_string(minor);
    }

    // Ordering is (major, minor) lexicographic.
    bool operator<(const SyntaxVersion& o) const {
        return major != o.major ? major < o.major : minor < o.minor;
    }
    bool operator>(const SyntaxVersion& o) const { return o < *this; }
    bool operator==(const SyntaxVersion& o) const {
        return major == o.major && minor == o.minor;
    }
    bool operator<=(const SyntaxVersion& o) const { return !(*this > o); }
    bool operator>=(const SyntaxVersion& o) const { return !(*this < o); }
};

// ---- The versions this compiler supports -----------------------------------
// The compiler accepts any declared syntax version V with
//     MIN_SUPPORTED <= V <= MAX_SUPPORTED.
// Bump MAX_SUPPORTED when the front end learns a newer MINOR/MAJOR grammar;
// raise MIN_SUPPORTED when an old grammar is finally dropped.
inline SyntaxVersion minSupportedSyntax() { return SyntaxVersion{1, 0}; }
inline SyntaxVersion maxSupportedSyntax() { return SyntaxVersion{1, 0}; }

// The version assumed when a file omits the pragma (Section 4.4 says a file
// "must declare" its version; for backward compatibility with pre-pragma
// sources we assume the floor and emit a warning rather than a hard error).
inline SyntaxVersion defaultSyntaxVersion() { return minSupportedSyntax(); }

// Result of resolving a file's declared syntax version.
enum class VersionStatus {
    OkDeclared,     // pragma present, version within the supported range
    OkAssumed,      // pragma absent; assumed default (a warning is warranted)
    TooNew,         // declared version exceeds MAX_SUPPORTED  -> must reject
    TooOld,         // declared version below MIN_SUPPORTED     -> must reject
    Malformed       // a #sleela pragma was present but unparseable -> reject
};

struct VersionResolution {
    VersionStatus status = VersionStatus::OkAssumed;
    SyntaxVersion declared;    // the version the file asked for (or the default)
    bool pragmaPresent = false;
    std::string raw;           // the raw version text as written in the pragma
    std::string message;       // human-readable diagnostic (warning or error)

    // The compiler must reject exactly TooNew / TooOld / Malformed.
    bool accepted() const {
        return status == VersionStatus::OkDeclared ||
               status == VersionStatus::OkAssumed;
    }
    bool isError() const { return !accepted(); }
    bool isWarning() const { return status == VersionStatus::OkAssumed; }
};

// Parse a "MAJOR.MINOR" (or "MAJOR.MINOR.PATCH") string. Returns true on success
// and fills `out` with the MAJOR.MINOR components (PATCH ignored).
bool parseSyntaxVersion(const std::string& text, SyntaxVersion& out);

// Scan `source` for the #sleela pragma on the first non-blank, non-comment line
// and decide accept / warn / reject against the supported range. This is the
// single entry point the front end / driver calls to be "version aware".
VersionResolution resolveSyntaxVersion(const std::string& source);

} // namespace sleela

#endif // SLEELA_VERSION_H
