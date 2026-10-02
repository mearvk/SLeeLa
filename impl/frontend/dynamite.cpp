// Dynamite Connector marker discovery. Metadata is comment-based.
#include "dynamite.h"
#include <sstream>
namespace sleela { namespace dynamite {
static std::string trim(const std::string& s){size_t a=s.find_first_not_of(" \t\r");if(a==std::string::npos)return "";size_t b=s.find_last_not_of(" \t\r");return s.substr(a,b-a+1);}
static std::string after(const std::string& line,const std::string& key){size_t p=line.find(key);if(p==std::string::npos)return "";p+=key.size();size_t e=line.find_first_of(" \t",p);return line.substr(p,e==std::string::npos?std::string::npos:e-p);}
Marker discover(const std::string& source,const std::string& sourcePath){
 Marker m; m.sourcePath=sourcePath; std::istringstream in(source); std::string line; size_t n=0;
 while(std::getline(in,line)){++n;std::string t=trim(line);
  if(t.find("// @dynamite class=")==0){m.present=true;m.line=n;m.className=after(t,"class=");continue;}
  if(t.find("// @dynamite.config=")==0){m.present=true;if(!m.line)m.line=n;m.configName=trim(t.substr(19));continue;}
  if(t.find("// @dynamite.property ")==0){m.present=true;if(!m.line)m.line=n;std::string p=t.substr(23);size_t eq=p.find('=');if(eq!=std::string::npos)m.properties.push_back({trim(p.substr(0,eq)),trim(p.substr(eq+1))});}
 }
 return m;
}
bool isImplicitLoadCandidate(const Marker& m){return m.present&&!m.className.empty()&&(!m.configName.empty()||!m.properties.empty());}
}}
