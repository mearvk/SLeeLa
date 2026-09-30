// ===========================================================================
// java_exceptions.h -- Java source-level checked-exception analysis.
// ===========================================================================
// Source semantics only. No JVM/SLVM runtime is required.
#ifndef SLEELA_JAVA_EXCEPTIONS_H
#define SLEELA_JAVA_EXCEPTIONS_H
#include <map>
#include <set>
#include <string>
#include <vector>
namespace sleela {
struct JavaExceptionType { std::string name; std::string superType; bool unchecked=false; };
enum class JavaExceptionDiagnosticKind { UnhandledCheckedException, InvalidCatch, RedundantCatch, IncompatibleOverrideThrows };
struct JavaExceptionDiagnostic { JavaExceptionDiagnosticKind kind; std::string exceptionType; std::string message; };
struct JavaExceptionFlow { std::set<std::string> thrown; std::vector<JavaExceptionDiagnostic> diagnostics; };
class JavaExceptionModel {
public:
 void addType(const JavaExceptionType& type);
 bool isSubtype(const std::string& child,const std::string& parent) const;
 bool isChecked(const std::string& type) const;
 bool catches(const std::string& catchType,const std::string& thrownType) const;
 JavaExceptionFlow check(const std::set<std::string>& thrown,const std::vector<std::string>& catches,const std::set<std::string>& declaredThrows) const;
 bool overrideThrowsCompatible(const std::set<std::string>& overriding,const std::set<std::string>& overridden) const;
private:
 std::map<std::string,JavaExceptionType> types_;
};
const char* javaExceptionDiagnosticName(JavaExceptionDiagnosticKind kind);
}
#endif
