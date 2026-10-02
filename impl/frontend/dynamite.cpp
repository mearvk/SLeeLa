// Dynamite Connector import discovery. The language construct is explicit.
#include "dynamite.h"
#include <sstream>
namespace sleela { namespace dynamite {
static std::string trim(const std::string& s){size_t a=s.find_first_not_of(" \t\r");if(a==std::string::npos)return "";size_t b=s.find_last_not_of(" \t\r");return s.substr(a,b-a+1);}
Import discover(const std::string& source,const std::string& sourcePath){
 Import m; m.sourcePath=sourcePath; std::istringstream in(source); std::string line; size_t n=0;
 while(std::getline(in,line)){++n;std::string t=trim(line);
  const std::string p="import dynamite connector ";
  const std::string q="import :: dynamite :: connector :: ";
  if(t.rfind(p,0)==0 || t.rfind(q,0)==0){const std::string& prefix=(t.rfind(p,0)==0?p:q);m.present=true;m.line=n;m.sourcePath=trim(t.substr(prefix.size()));if(!m.sourcePath.empty()&&m.sourcePath.back()==';')m.sourcePath.pop_back();m.sourcePath=trim(m.sourcePath);continue;}
  if(t.rfind("dynamite config ",0)==0){m.present=true;if(!m.line)m.line=n;m.configName=trim(t.substr(16));if(!m.configName.empty()&&m.configName.back()==';')m.configName.pop_back();continue;}
  if(t.rfind("dynamite property ",0)==0){m.present=true;if(!m.line)m.line=n;std::string q=trim(t.substr(18));if(!q.empty()&&q.back()==';')q.pop_back();size_t eq=q.find('=');if(eq!=std::string::npos)m.properties.push_back({trim(q.substr(0,eq)),trim(q.substr(eq+1))});}
 } return m; }
bool isImplicitLoadCandidate(const Import& m){return m.present&&!m.sourcePath.empty();}
}}