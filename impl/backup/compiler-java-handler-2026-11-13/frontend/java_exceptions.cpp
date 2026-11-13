// ===========================================================================
// java_exceptions.cpp -- Java source-level checked-exception analysis.
// ===========================================================================
#include "java_exceptions.h"
namespace sleela {
void JavaExceptionModel::addType(const JavaExceptionType& t){ types_[t.name]=t; }
bool JavaExceptionModel::isSubtype(const std::string& child,const std::string& parent) const {
 if(child==parent) return true;
 auto it=types_.find(child); std::set<std::string> seen;
 while(it!=types_.end() && !it->second.superType.empty() && seen.insert(it->first).second){
  if(it->second.superType==parent) return true;
  it=types_.find(it->second.superType);
 }
 return false;
}
bool JavaExceptionModel::isChecked(const std::string& t) const {
 auto it=types_.find(t); return it!=types_.end() && !it->second.unchecked;
}
bool JavaExceptionModel::catches(const std::string& c,const std::string& e) const { return isSubtype(e,c); }
JavaExceptionFlow JavaExceptionModel::check(const std::set<std::string>& thrown,const std::vector<std::string>& catchTypes,const std::set<std::string>& declared) const {
 JavaExceptionFlow r; r.thrown=thrown;
 for(const auto& e:thrown) if(isChecked(e)){
  bool handled=false; for(const auto& c:catchTypes) if(this->catches(c,e)){handled=true;break;}
  bool declaredHere=false; for(const auto& d:declared) if(isSubtype(e,d)){declaredHere=true;break;}
  if(!handled && !declaredHere) r.diagnostics.push_back({JavaExceptionDiagnosticKind::UnhandledCheckedException,e,"checked exception is neither caught nor declared"});
 }
 for(std::size_t i=0;i<catchTypes.size();++i) for(std::size_t j=0;j<i;++j)
  if(isSubtype(catchTypes[i],catchTypes[j])) r.diagnostics.push_back({JavaExceptionDiagnosticKind::RedundantCatch,catchTypes[i],"catch clause is shadowed by a preceding catch clause"});
 return r;
}
bool JavaExceptionModel::overrideThrowsCompatible(const std::set<std::string>& overriding,const std::set<std::string>& overridden) const {
 for(const auto& e:overriding) if(isChecked(e)){
  bool ok=false; for(const auto& d:overridden) if(isSubtype(e,d)){ok=true;break;}
  if(!ok) return false;
 }
 return true;
}
const char* javaExceptionDiagnosticName(JavaExceptionDiagnosticKind k){
 switch(k){case JavaExceptionDiagnosticKind::UnhandledCheckedException:return "unhandled-checked-exception";case JavaExceptionDiagnosticKind::InvalidCatch:return "invalid-catch";case JavaExceptionDiagnosticKind::RedundantCatch:return "redundant-catch";case JavaExceptionDiagnosticKind::IncompatibleOverrideThrows:return "incompatible-override-throws";}
 return "unknown";
}
}
