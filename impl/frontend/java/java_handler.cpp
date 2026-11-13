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
#include <algorithm>
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


    // Java tier selection among library units that share a simple name.
    // Generated JDK API documentation (docs/api envelopes) duplicates real
    // class names; java/jdk-28/<module>/... holds one reference file per JDK 28
    // type. Rules (SLEELA.syntax 43; lib/java/jdk-28/README.md "Name clashes"):
    //   * a real source unit beats docs/api and jdk-28 reference files;
    //   * with only reference files, java/jdk-28 beats docs/api;
    //   * `import java.jdk-28;` selects the JDK 28 tier outright;
    //   * several JDK 28 clashes (java.util.List vs java.awt.List, ...) resolve
    //     by rank: longest matching import root, module (java.base, other java.*,
    //     jdk.*), package (java.lang, java.util, java.io, java.nio, java.net,
    //     java.time, java.math, java.text, rest), fewer segments, byte order.
    std::vector<std::string> filterLibraryCandidates(const std::string&,const std::vector<std::string>& paths,const std::vector<std::string>& roots) const override {
        static const std::string kJdk28="java/jdk-28/";
        auto isJdk28=[&](const std::string& p){ return p.rfind(kJdk28,0)==0; };
        auto isReference=[&](const std::string& p){ return p.find("/docs/api/")!=std::string::npos || isJdk28(p); };
        bool hasNonDoc=false, hasJdk28=false;
        for(const auto& p:paths){ if(!isReference(p)) hasNonDoc=true; if(isJdk28(p)) hasJdk28=true; }
        const bool selected=std::find(roots.begin(),roots.end(),std::string("java/jdk-28"))!=roots.end();
        const bool onlyJdk28=selected&&hasJdk28;
        if(onlyJdk28) hasNonDoc=false;
        std::vector<std::string> kept, jdk;
        for(const auto& p:paths){
            if(onlyJdk28 && !isJdk28(p)) continue;
            if(hasNonDoc && isReference(p)) continue;
            if(!hasNonDoc && hasJdk28 && !isJdk28(p)) continue;
            kept.push_back(p);
            if(isJdk28(p)) jdk.push_back(p);
        }
        if(jdk.size()<=1) return kept;
        auto rootLen=[&](const std::string& p){
            size_t best=0;
            for(const auto& r:roots) if(p.rfind(r+"/",0)==0) best=std::max(best,r.size());
            return best;
        };
        auto moduleTier=[&](const std::string& p){
            const std::string rest=p.substr(kJdk28.size());
            if(rest.rfind("java.base/",0)==0) return 0;
            if(rest.rfind("java.",0)==0) return 1;
            return 2;
        };
        auto packageRank=[&](const std::string& p){
            static const char* order[]={"/java/lang/","/java/util/","/java/io/","/java/nio/","/java/net/","/java/time/","/java/math/","/java/text/"};
            const std::string rest=p.substr(kJdk28.size());
            const size_t slash=rest.find('/');
            const std::string pkg=slash==std::string::npos?rest:rest.substr(slash);
            for(int i=0;i<8;++i){
                const std::string o=order[i];
                if(pkg.rfind(o,0)==0 && pkg.find('/',o.size())==std::string::npos) return i;
            }
            return 8;
        };
        auto depth=[](const std::string& p){ return (int)std::count(p.begin(),p.end(),'/'); };
        auto better=[&](const std::string& a,const std::string& b){
            const size_t ra=rootLen(a), rb=rootLen(b);
            if(ra!=rb) return ra>rb;
            const int ma=moduleTier(a), mb=moduleTier(b);
            if(ma!=mb) return ma<mb;
            const int pa=packageRank(a), pb=packageRank(b);
            if(pa!=pb) return pa<pb;
            const int da=depth(a), db=depth(b);
            if(da!=db) return da<db;
            return a<b;
        };
        std::string choice=jdk[0];
        for(const auto& c:jdk) if(better(c,choice)) choice=c;
        std::vector<std::string> out;
        for(const auto& p:kept) if(!isJdk28(p) || p==choice) out.push_back(p);
        return out;
    }

    void validateProgram(const Program& p) const override { validateJavaCompatibilityMetadata(p); }
};

static JavaHandler g_handler;
static lang::Registrar g_registrar(&g_handler);
}
}
