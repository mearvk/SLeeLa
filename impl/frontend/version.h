// ===========================================================================
// version.h -- Syntax-version awareness for the Sleela compiler.
// ===========================================================================
#ifndef SLEELA_VERSION_H
#define SLEELA_VERSION_H
#include <string>
namespace sleela {
struct SyntaxVersion { int major=0, minor=0; std::string str() const { return std::to_string(major)+"."+std::to_string(minor); } bool operator<(const SyntaxVersion&o)const{return major!=o.major?major<o.major:minor<o.minor;} bool operator>(const SyntaxVersion&o)const{return o<*this;} bool operator==(const SyntaxVersion&o)const{return major==o.major&&minor==o.minor;} bool operator<=(const SyntaxVersion&o)const{return !(*this>o);} bool operator>=(const SyntaxVersion&o)const{return !(*this<o);} };
inline SyntaxVersion minSupportedSyntax(){return SyntaxVersion{1,3};} inline SyntaxVersion maxSupportedSyntax(){return SyntaxVersion{1,6};} inline SyntaxVersion defaultSyntaxVersion(){return maxSupportedSyntax();}
enum class VersionStatus{OkDeclared,OkAssumed,TooNew,TooOld,Malformed};
struct VersionResolution{ VersionStatus status=VersionStatus::OkAssumed; SyntaxVersion declared; bool pragmaPresent=false; std::string raw; std::string message; bool accepted()const{return status==VersionStatus::OkDeclared||status==VersionStatus::OkAssumed;} bool isError()const{return !accepted();} bool isWarning()const{return status==VersionStatus::OkAssumed;} };
bool parseSyntaxVersion(const std::string& text,SyntaxVersion& out); VersionResolution resolveSyntaxVersion(const std::string& source);
} // namespace sleela
#endif