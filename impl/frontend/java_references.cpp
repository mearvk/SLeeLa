// ===========================================================================
// java_references.cpp -- Java name references (extends / superclass).
// See java_references.h.
// ===========================================================================
#include "java_references.h"
namespace sleela {
namespace javaref {
std::string simpleName(const std::string& q){
    const std::size_t dot=q.rfind('.');
    return dot==std::string::npos?q:q.substr(dot+1);
}
bool isQualified(const std::string& n){ return n.find('.')!=std::string::npos; }
std::string resolveBaseName(const ClassIndex& byName,const std::string& declared){
    if(declared.empty()) return declared;
    if(byName.count(declared)) return declared;
    if(isQualified(declared)){
        auto it=byName.find(simpleName(declared));
        if(it!=byName.end() && it->second && it->second->fromLibrary) return it->first;
    }
    return declared;
}
std::string superclassOf(const ClassIndex& byName,const ClassDecl& cls){
    return resolveBaseName(byName,cls.java.superclass);
}
}
}
