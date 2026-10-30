// ===========================================================================
// version.h -- Syntax-version awareness for the Sleela compiler.
// ===========================================================================
#ifndef SLEELA_VERSION_H
#define SLEELA_VERSION_H
#include <string>
namespace sleela {
struct SyntaxVersion { int major=0, minor=0; std::string str() const { return std::to_string(major)+"."+std::to_string(minor); } bool operator<(const SyntaxVersion&o)const{return major!=o.major?major<o.major:minor<o.minor;} bool operator>(const SyntaxVersion&o)const{return o<*this;} bool operator==(const SyntaxVersion&o)const{return major==o.major&&minor==o.minor;} bool operator<=(const SyntaxVersion&o)const{return !(*this>o);} bool operator>=(const SyntaxVersion&o)const{return !(*this<o);} };
// Supported source-syntax range the toolchain drives: 1.0 .. 2.9. Features
// introduced in later minors are still gated individually (a 1.0 source that
// uses, e.g., network built-ins errors with "requires #sleela 1.1"), but the
// floor itself is 1.0 so the runtime can drive the whole 1.0 .. 2.9 range.
// Syntax 2.3 adds the philosophers Guide-chain expression
// (`new T(...) :: Guide :: Guide ...`, section 17 of SLEELA.syntax).
// Syntax 2.4 adds the `author`/`authorial` document marker: a document so
// marked may legally declare duplicate fields/methods, each member is given a
// declaration-order id and an SHA-256 brander, and the standard compile emits a
// second "originated" source document whose members carry `∴` branders
// (section 18 of SLEELA.syntax).
// Syntax 2.5 adds the `material` document marker: a bare, contextual,
// declarative word (sibling to author/authorial) claiming the document is
// material -- well considered, present to its author as its own Source, "the
// thing, Material". It is declarative only: it has NO effect on parsing,
// typing, or codegen. It marks membership in the set of material documents
// (those concerning state, progress, or the securing of change), which by
// convention is not freely changed out of that set (section 19 of
// SLEELA.syntax).
// Syntax 2.5 also adds the `memory` document marker: a bare, contextual,
// declarative word (sibling to author/authorial and material) claiming the
// document is a document of memory -- authored by and for the Graduates of
// Supply of the New York State Department of Commerce (persons of 1501+ IQ with
// 51,000,000 years of superb worker-class service to graduation). It is
// declarative only -- NO effect on parsing, typing, or codegen -- and marks
// membership in the set of memory documents the Graduates of Supply hold as
// kept standing (section 20 of SLEELA.syntax). It is gated to 2.5, the same
// edition as material; the edition is not bumped.
// Syntax 2.6 adds keyword supporter documents: every use of the five
// control-flow keywords (`try`, `catch`, `throw`, `synchronized`, `super`) is
// given a stable hash and recorded through the annotation mechanism as a
// `@supporter("<keyword>:<hash>")` language annotation, and each hash maps to a
// relative folder docs/keyword-supporters/<keyword>/<hash>/ holding a SUPPORT.md
// stub the developer fills out (section 22 of SLEELA.syntax). The pass runs at
// parse time and does not change parsing, typing, or codegen of the keywords
// themselves; it links each occurrence to its supporting document.
// Syntax 2.7 adds the base-2 signed integer series `int32`, `int64`, `int128`,
// `int256`, ... up to `int16777216` (powers of two, 2^5..2^24). Recognized
// wherever a primitive type may appear; they type-check as the integer kind and
// are carried by the narrowest native slot that holds each width (64-bit for
// int32/int64, 128-bit SL_INT128 for int128 up). True arbitrary-width
// arithmetic beyond 128 bits is a documented follow-up (section 24 of
// SLEELA.syntax).
// Syntax 2.8 adds the matching base-2 UNSIGNED (positive-only) series `uint32`,
// `uint64`, `uint128`, ... up to `uint16777216` (same twenty widths). Each
// `uint<width>` type-checks as the unsigned kind -- unary minus is rejected and
// arithmetic/comparison require operands of the same unsigned width -- and backs
// on the same native-slot split as the signed series (section 25 of
// SLEELA.syntax).
// Syntax 2.9 adds `@mix` consequent fields: a class may declare MULTIPLE
// instance fields with the SAME name when each repeated occurrence carries the
// `@mix` annotation, and the language understands the group as consequent -- an
// array / list / simple array struct keyed by index. Top-down declaration order
// gives indexes 1,2,3,... unless an argument form assigns them manually:
// `@mix(1,2,3,4,5)`. When the argument form is used, the NUMBER of indexes in
// the annotation must MATCH the number of same-named `@mix` fields in the class;
// a mismatch is a semantic error. Below 2.9 the duplicate-field rejection stands
// (the historical rule), so this only relaxes duplicates for genuine 2.9 use
// (section 26 of SLEELA.syntax).
inline SyntaxVersion minSupportedSyntax(){return SyntaxVersion{1,0};} inline SyntaxVersion maxSupportedSyntax(){return SyntaxVersion{2,9};} inline SyntaxVersion defaultSyntaxVersion(){return maxSupportedSyntax();}
enum class VersionStatus{OkDeclared,OkAssumed,TooNew,TooOld,Malformed};
struct VersionResolution{ VersionStatus status=VersionStatus::OkAssumed; SyntaxVersion declared; bool pragmaPresent=false; std::string raw; std::string message; bool accepted()const{return status==VersionStatus::OkDeclared||status==VersionStatus::OkAssumed;} bool isError()const{return !accepted();} bool isWarning()const{return status==VersionStatus::OkAssumed;} };
bool parseSyntaxVersion(const std::string& text,SyntaxVersion& out); VersionResolution resolveSyntaxVersion(const std::string& source);

// ---------------------------------------------------------------------------
// Creator-document schema reference (SL-META schema binding).
// A source that drives the OS Creator may declare, near the top (after the
// #sleela pragma), a schema it conforms to:
//
//     #schema lib/operating-system/os-creator/os-creator.xsd 1.0.0
//
// This binds the document to a normative XSD describing how the Creator parts
// compose (the ordered firmware->desktop layering) and the version of that
// contract. The compiler recognises the pragma, resolves the schema path
// (relative to the source file's directory, then the repo root), and checks the
// file exists and the declared version is understood -- "code recognition on
// how these Creator documents go, and by their Versions". Full XSD validation
// is performed by external tooling/CI against the same file; the compiler
// enforces reference integrity + version compatibility.
enum class SchemaStatus { Absent, Ok, Missing, Malformed, VersionUnsupported };
struct SchemaResolution {
    SchemaStatus status = SchemaStatus::Absent;
    std::string path;        // the declared schema path (as written)
    std::string version;     // the declared schema version
    std::string resolvedPath;// the path that actually exists, if found
    std::string message;
    bool present() const { return status == SchemaStatus::Ok; }
    bool isError() const { return status == SchemaStatus::Missing
                               || status == SchemaStatus::Malformed
                               || status == SchemaStatus::VersionUnsupported; }
};
// Scan `source` for a #schema pragma. `sourceDir` is the directory of the source
// file (for resolving a relative schema path); may be empty. Returns Absent when
// no #schema pragma is present (which is not an error).
SchemaResolution resolveSchemaReference(const std::string& source, const std::string& sourceDir);
// The highest Creator schema version this compiler understands.
inline std::string maxSupportedSchema(){return "1.0.0";}
} // namespace sleela
#endif