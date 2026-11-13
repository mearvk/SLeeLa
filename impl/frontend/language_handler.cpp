// language_handler.cpp -- registry and dispatch for source-language handlers.
// See language_handler.h. Contains no knowledge of any particular language.
#include "language_handler.h"
#include <algorithm>
namespace sleela {
namespace lang {
static std::vector<LanguageHandler*>& registry(){ static std::vector<LanguageHandler*> r; return r; }
void registerHandler(LanguageHandler* h){
    if(!h) return;
    auto& r=registry();
    if(std::find(r.begin(),r.end(),h)==r.end()) r.push_back(h);
}
const std::vector<LanguageHandler*>& handlers(){ return registry(); }

std::vector<DocumentMarker> documentMarkers(){
    std::vector<DocumentMarker> out;
    for(auto* h:registry()) for(auto& m:h->documentMarkers()) out.push_back(m);
    return out;
}
const DocumentMarker* findDocumentMarker(const std::string& word){
    static std::vector<DocumentMarker> cache; cache=documentMarkers();
    for(const auto& m:cache) if(m.word==word) return &m;
    return nullptr;
}
bool isDocumentMarker(const std::string& word){ return findDocumentMarker(word)!=nullptr; }
std::string resolveReference(const ClassIndexMap& byName,const std::string& declared){
    if(declared.empty()) return declared;
    if(byName.count(declared)) return declared;          // an exact match always wins
    for(auto* h:registry()){ std::string r=h->resolveReference(byName,declared); if(!r.empty()) return r; }
    return declared;
}
std::string eraseSignatureType(const std::string& source,const std::vector<std::vector<std::string>>& tv){
    for(auto* h:registry()){ std::string r=h->eraseSignatureType(source,tv); if(!r.empty()) return r; }
    return source;
}
std::string stringMethodLibrary(){
    for(auto* h:registry()){ std::string r=h->stringMethodLibrary(); if(!r.empty()) return r; }
    return "";
}
std::string faultFunction(){
    for(auto* h:registry()){ std::string r=h->faultFunction(); if(!r.empty()) return r; }
    return "";
}
std::vector<std::string> libraryNeeds(const Program& p,const std::string& src){
    std::vector<std::string> out;
    for(auto* h:registry()) for(auto& n:h->libraryNeeds(p,src)) out.push_back(n);
    return out;
}
void validateProgram(const Program& p){ for(auto* h:registry()) h->validateProgram(p); }
std::vector<std::string> filterLibraryCandidates(const std::string& name,const std::vector<std::string>& paths,const std::vector<std::string>& roots){
    std::vector<std::string> cur=paths;
    for(auto* h:registry()) cur=h->filterLibraryCandidates(name,cur,roots);
    return cur;
}
bool runHost(const std::vector<std::string>& args,int& rc){
    for(auto* h:registry()) if(h->runHost(args,rc)) return true;
    return false;
}
}
}
