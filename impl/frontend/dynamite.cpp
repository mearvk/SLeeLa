// Dynamite Connector import discovery. The language construct is explicit.
#include "dynamite.h"
#include <sstream>
namespace sleela { namespace dynamite {
static std::string trim(const std::string& s){
    size_t a=s.find_first_not_of(" \t\r");
    if(a==std::string::npos) return "";
    size_t b=s.find_last_not_of(" \t\r");
    return s.substr(a,b-a+1);
}
static bool starts(const std::string& s,const std::string& p){ return s.rfind(p,0)==0; }
std::vector<Import> discoverAll(const std::string& source,const std::string& sourcePath){
    std::vector<Import> out;
    std::istringstream in(source);
    std::string line; size_t n=0;
    while(std::getline(in,line)){
        ++n;
        std::string t=trim(line);
        if(starts(t,"import dynamite connector ")){
            std::string rest=trim(t.substr(26));
            if(!rest.empty()&&rest.back()==';') rest.pop_back();
            rest=trim(rest);
            Import m; m.present=true; m.line=n; m.sourcePath=rest;
            const std::string marker=" = ";
            size_t eq=rest.find(marker);
            if(eq!=std::string::npos){
                m.referenceName=trim(rest.substr(0,eq));
                m.sourcePath=trim(rest.substr(eq+marker.size()));
            }
            out.push_back(std::move(m));
            continue;
        }
        if(starts(t,"import :: dynamite :: connector :: ")){
            std::string rest=trim(t.substr(35));
            if(!rest.empty()&&rest.back()==';') rest.pop_back();
            rest=trim(rest);
            Import m; m.present=true; m.line=n;
            size_t sep=rest.find(" :: ");
            if(sep!=std::string::npos){
                m.referenceName=trim(rest.substr(0,sep));
                m.sourcePath=trim(rest.substr(sep+4));
            } else m.sourcePath=rest;
            out.push_back(std::move(m));
            continue;
        }
        if(starts(t,"dynamite config ")){
            std::string rest=trim(t.substr(16));
            if(!rest.empty()&&rest.back()==';') rest.pop_back();
            rest=trim(rest);
            std::string ref;
            size_t sep=rest.find(" :: ");
            if(sep!=std::string::npos){ ref=trim(rest.substr(0,sep)); rest=trim(rest.substr(sep+4)); }
            else { sep=rest.find(" = "); if(sep!=std::string::npos){ ref=trim(rest.substr(0,sep)); rest=trim(rest.substr(sep+3)); } }
            if(!out.empty()){
                size_t target=out.size()-1;
                if(!ref.empty()){ for(size_t i=0;i<out.size();++i) if(out[i].referenceName==ref){target=i;break;} }
                out[target].configName=rest;
            }
            continue;
        }
        if(starts(t,"dynamite property ")){
            std::string rest=trim(t.substr(18));
            if(!rest.empty()&&rest.back()==';') rest.pop_back();
            rest=trim(rest);
            std::string ref;
            size_t scope=rest.find(" :: ");
            if(scope!=std::string::npos){ ref=trim(rest.substr(0,scope)); rest=trim(rest.substr(scope+4)); }
            size_t eq=rest.find('=');
            if(eq!=std::string::npos && !out.empty()){
                size_t target=out.size()-1;
                if(!ref.empty()){ for(size_t i=0;i<out.size();++i) if(out[i].referenceName==ref){target=i;break;} }
                out[target].properties.push_back({trim(rest.substr(0,eq)),trim(rest.substr(eq+1))});
            }
        }
    }
    return out;
}
Import discover(const std::string& source,const std::string& sourcePath){
    auto all=discoverAll(source,sourcePath);
    if(all.empty()) { Import m; m.sourcePath=sourcePath; return m; }
    return all.front();
}
bool isImplicitLoadCandidate(const Import& m){ return m.present&&!m.sourcePath.empty(); }
}}