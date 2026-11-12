// ===========================================================================
// java_references.cpp -- Java name references (extends / superclass) and the
// Sleela.home lineage built on them.
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

bool sparkDocument(const Program& p){ return p.sparkMarked; }

std::string lineageParentOf(const ClassIndex& classes,const std::string& n){
    auto it=classes.find(n);
    if(it==classes.end()) return "";
    const std::string s=superclassOf(classes,*it->second);
    return classes.count(s)?s:"";
}
std::string lineageRootOf(const ClassIndex& classes,std::string n){
    std::set<std::string> seen;
    for(;;){
        seen.insert(n);
        std::string p=lineageParentOf(classes,n);
        if(p.empty()||seen.count(p)) return n;
        n=p;
    }
}
std::string lineageValue(const ClassIndex& classes,const std::string& currentClass,const LineageExpr& x){
    const std::string vm="Sleela.System";
    switch(x.kind){
    case LineageExpr::Vm: return vm;
    case LineageExpr::InitParent: { std::string p=lineageParentOf(classes,currentClass); return p.empty()?vm:p; }
    case LineageExpr::InitChild:
        for(const auto& kv:classes)
            if(superclassOf(classes,*kv.second)==currentClass && kv.first!=currentClass) return kv.first;
        return vm;
    case LineageExpr::InitRoot: return lineageRootOf(classes,currentClass);
    case LineageExpr::Parents: {
        if(x.depth<=0) return vm;
        std::string n=currentClass;
        for(int i=0;i<x.depth;i++){ std::string p=lineageParentOf(classes,n); if(p.empty()) break; n=p; }  // clamps to the root
        return n; }
    case LineageExpr::Boot: break;
    }
    return vm;
}
}
}
