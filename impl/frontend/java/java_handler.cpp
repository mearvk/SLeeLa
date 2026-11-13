// ===========================================================================
// java_handler.cpp -- the Java language handler module for the SLeeLa compiler.
// ===========================================================================
// Everything the compiler does specially for Java-authored or Java-referencing
// documents lives in this directory and is reached only through the neutral
// LanguageHandler interface (frontend/language_handler.h):
//
//   * the `spark` Preonem document marker (SLEELA.syntax section 43)
//   * resolving a package-qualified reference (java.lang.Throwable) to the
//     simple-named /lib class that stands for it
//   * erasing Java signature types (type variables, java.util.List<E>) to the
//     VM types SLeeLa supports
//   * naming the Java String method library and the VM-fault exception library
//     and deciding when the driver must seed them
//   * validating Java compatibility metadata (java_compat.h)
//
// The module registers itself with a static Registrar. Removing this file from
// FE_SRC in impl/Makefile removes Java support; the compiler, the VM and the
// runtime are untouched. Source semantics only: no JVM is involved here (the
// optional JVM host launch is a separate concern, see COMPILER-JAVA-REVIEW.md).
// Specification: Java Specific Section, lib/java/JAVA.SPEC.md.
// ===========================================================================
#include "java_handler.h"
#include "java_compat.h"
#include <regex>
namespace sleela {
namespace javahandler {
std::string simpleName(const std::string& q){
    const std::size_t dot=q.rfind('.');
    return dot==std::string::npos?q:q.substr(dot+1);
}
bool isQualified(const std::string& n){ return n.find('.')!=std::string::npos; }

class JavaHandler : public LanguageHandler {
public:
    const char* name() const override { return "java"; }

    std::vector<DocumentMarker> documentMarkers() const override {
        // Spark marker (syntax 3.0, SLEELA.syntax section 43): a Preonem
        // indicating Java-style declarative source. Declarative only.
        return { DocumentMarker{"spark",SyntaxVersion{3,0}} };
    }

    // A qualified name falls back to its simple name only when that class came
    // from /lib (fromLibrary), so a local class named Object cannot impersonate
    // java.lang.Object.
    std::string resolveReference(const ClassIndexMap& byName,const std::string& declared) const override {
        if(isQualified(declared)){
            auto it=byName.find(simpleName(declared));
            if(it!=byName.end() && it->second && it->second->fromLibrary) return it->first;
        }
        return "";
    }

    // A declared type variable (E, T, K ...) and any package-qualified
    // reference type (java.util.List<E>) erase to Any, java.lang.String to
    // String. Array suffixes are preserved; the source keeps the Java spelling.
    std::string eraseSignatureType(const std::string& source,const std::vector<std::vector<std::string>>& typeVars) const override {
        std::string base=source; std::string dims;
        while(base.size()>=2 && base.compare(base.size()-2,2,"[]")==0){ dims+="[]"; base.resize(base.size()-2); }
        std::string raw=base.substr(0,base.find('<'));
        for(auto it=typeVars.rbegin(); it!=typeVars.rend(); ++it)
            for(const auto& v:*it) if(v==raw) return "Any"+dims;
        if(raw.find('.')!=std::string::npos && !raw.empty() && raw[0]>='a' && raw[0]<='z'){
            if(raw=="java.lang.String") return "String"+dims;
            return "Any"+dims;
        }
        return "";
    }

    std::string stringMethodLibrary() const override { return "JavaStringMethods"; }
    std::string faultFunction() const override { return "SLJavaFaults::make"; }

    // SLEELA.syntax 37.5: a Java-style document (spark marker or an
    // `import java...`) that uses try gets the VM-fault exception library, so a
    // division by zero inside the try throws ArithmeticException.
    std::vector<std::string> libraryNeeds(const Program& program,const std::string& source) const override {
        for(const auto& cls:program.classes) if(cls.name=="SLJavaFaults") return {};
        bool javaStyle=program.markers.count("spark")>0;
        for(const auto& im:program.imports) if(im.rfind("java",0)==0) javaStyle=true;
        static const std::regex tryWord("\\btry\\b");
        if(javaStyle && std::regex_search(source,tryWord)) return {"SLJavaFaults"};
        return {};
    }

    void validateProgram(const Program& p) const override { validateJavaCompatibilityMetadata(p); }
};

static JavaHandler g_handler;
static lang::Registrar g_registrar(&g_handler);
}
}
