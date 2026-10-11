// ===========================================================================
// version.h -- Syntax-version awareness for the Sleela compiler.
// ===========================================================================
#ifndef SLEELA_VERSION_H
#define SLEELA_VERSION_H
#include <string>
namespace sleela {
struct SyntaxVersion { int major=0, minor=0; std::string str() const { return std::to_string(major)+"."+std::to_string(minor); } bool operator<(const SyntaxVersion&o)const{return major!=o.major?major<o.major:minor<o.minor;} bool operator>(const SyntaxVersion&o)const{return o<*this;} bool operator==(const SyntaxVersion&o)const{return major==o.major&&minor==o.minor;} bool operator<=(const SyntaxVersion&o)const{return !(*this>o);} bool operator>=(const SyntaxVersion&o)const{return !(*this<o);} };
inline SyntaxVersion minSupportedSyntax(){return SyntaxVersion{1,3};} inline SyntaxVersion maxSupportedSyntax(){return SyntaxVersion{1,11};} inline SyntaxVersion defaultSyntaxVersion(){return maxSupportedSyntax();}
enum class VersionStatus{OkDeclared,OkAssumed,TooNew,TooOld,Malformed};
struct VersionResolution{ VersionStatus status=VersionStatus::OkAssumed; SyntaxVersion declared; bool pragmaPresent=false; std::string raw; std::string message; bool accepted()const{return status==VersionStatus::OkDeclared||status==VersionStatus::OkAssumed;} bool isError()const{return !accepted();} bool isWarning()const{return status==VersionStatus::OkAssumed;} };
bool parseSyntaxVersion(const std::string& text,SyntaxVersion& out); VersionResolution resolveSyntaxVersion(const std::string& source);

// ---------------------------------------------------------------------------
// Creator-document schema reference (SL-META schema binding).
// A source that drives the OS Creator may declare, near the top (after the
// #sleela pragma), a schema it conforms to:
//
//     #schema lib/os/os-creator/os-creator.xsd 1.0.0
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