// ===========================================================================
// java_compat.h -- Java authorship -> SLeeLa runtime transition contract.
// ===========================================================================
// This layer records what the parser preserved from Java authorship and gives
// the compiler a single compatibility gate. It does not pretend that a
// declaration is behaviorally equivalent until a runtime binding exists.
#ifndef SLEELA_JAVA_COMPAT_H
#define SLEELA_JAVA_COMPAT_H
#include "ast.h"
#include <stdexcept>
namespace sleela {
struct JavaCompatibilityManifest {
    int types=0, fields=0, methods=0, constructors=0, thrownTypes=0, interfaces=0, previewTypes=0;
    bool sourceVersion28=true;
};
inline JavaCompatibilityManifest buildJavaCompatibilityManifest(const Program& p){
    JavaCompatibilityManifest m;
    for(const auto& c:p.classes){
        ++m.types;
        if(c.java.kind==JavaTypeKind::Interface) ++m.interfaces;
        if(c.java.preview) ++m.previewTypes;
        if(c.java.sourceVersion!="28") m.sourceVersion28=false;
        m.fields += (int)c.fields.size(); m.methods += (int)c.methods.size();
        for(const auto& x:c.methods){ if(x.java.constructor) ++m.constructors; m.thrownTypes += (int)x.java.thrownTypes.size(); }
    }
    return m;
}
inline void validateJavaCompatibilityMetadata(const Program& p){
    for(const auto& c:p.classes){
        if(c.java.qualifiedName.empty()) throw std::runtime_error("Java compatibility: type has no qualified name");
        if(c.java.kind==JavaTypeKind::Record && (c.java.modifiers&JavaAbstract))
            throw std::runtime_error("Java compatibility: record cannot be abstract");
        if((c.java.modifiers&JavaSealed) && (c.java.modifiers&JavaNonSealed))
            throw std::runtime_error("Java compatibility: type cannot be both sealed and non-sealed");
        for(const auto& m:c.methods){
            if(m.java.constructor && m.name!=c.name)
                throw std::runtime_error("Java compatibility: constructor name '"+m.name+"' does not match type '"+c.name+"'");
        }
    }
}
} // namespace sleela
#endif
