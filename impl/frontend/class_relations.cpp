// class_relations.cpp -- language-neutral class relations. See class_relations.h.
#include "class_relations.h"
#include <algorithm>
namespace sleela {
namespace classrel {
std::string resolveBaseName(const ClassIndex& byName,const std::string& declared){ return lang::resolveReference(byName,declared); }
std::string superclassOf(const ClassIndex& byName,const ClassDecl& cls){
    return resolveBaseName(byName,cls.java.superclass);
}

std::string stripTypeArguments(const std::string& t){
    const std::size_t lt=t.find('<');
    std::string s=lt==std::string::npos?t:t.substr(0,lt);
    while(!s.empty() && s.back()==' ') s.pop_back();
    return s;
}
bool isSubtypeOf(const ClassIndex& classes,const std::string& from,const std::string& to){
    if(from==to) return true;
    std::set<std::string> seen;
    std::vector<std::string> work{from};
    while(!work.empty()){
        const std::string n=work.back(); work.pop_back();
        if(!seen.insert(n).second) continue;
        if(n==to) return true;
        auto it=classes.find(n);
        if(it==classes.end()||!it->second) continue;
        const std::string sup=superclassOf(classes,*it->second);
        if(!sup.empty()) work.push_back(sup);
        for(const auto& i:it->second->java.interfaces){
            const std::string r=resolveBaseName(classes,stripTypeArguments(i));
            if(!r.empty()) work.push_back(r);
        }
    }
    return false;
}
int inheritanceDepth(const ClassIndex& classes,const std::string& n){
    std::set<std::string> seen; std::string cur=n; int d=0;
    for(;;){
        seen.insert(cur);
        const std::string p=lineageParentOf(classes,cur);
        if(p.empty()||seen.count(p)) return d;
        ++d; cur=p;
    }
}

std::vector<std::string> mostDerivedFirst(const ClassIndex& classes,const std::vector<std::string>& names){
    std::vector<std::pair<int,std::string>> ranked;
    for(const auto& n:names){
        int above=0;
        for(const auto& o:names) if(o!=n && isSubtypeOf(classes,n,o)) ++above;
        ranked.emplace_back(above,n);
    }
    std::stable_sort(ranked.begin(),ranked.end(),[](const auto& a,const auto& b){return a.first>b.first;});
    std::vector<std::string> out; for(const auto& r:ranked) out.push_back(r.second);
    return out;
}

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
