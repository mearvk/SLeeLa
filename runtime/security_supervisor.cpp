#include "security_supervisor.hpp"
namespace sleela::runtime {
void SecuritySupervisor::setClassPolicy(const std::string&n,bool a,std::size_t i,std::size_t b){classes_[n]=ClassPolicy{a,i,b,0,0};}
void SecuritySupervisor::setRolePolicy(SecurityRole r,std::size_t i,std::size_t b){roles_[key(r)]=RolePolicy{i,b};}
bool SecuritySupervisor::authorizeClass(const std::string&n)const{auto i=classes_.find(n);return i!=classes_.end()&&i->second.allowed;}
bool SecuritySupervisor::reserve(const std::string&n,std::size_t b){auto i=classes_.find(n);if(i==classes_.end()||!i->second.allowed)return false;auto&p=i->second;if(p.maxInstances&&p.instances+1>p.maxInstances)return false;if(p.maxBytes&&p.bytes+b>p.maxBytes)return false;auto r=roles_.find(key(activeRole_));if(r!=roles_.end()){if(r->second.maxTotalInstances&&totalInstances_+1>r->second.maxTotalInstances)return false;if(r->second.maxTotalBytes&&totalBytes_+b>r->second.maxTotalBytes)return false;}++p.instances;p.bytes+=b;++totalInstances_;totalBytes_+=b;return true;}
void SecuritySupervisor::release(const std::string&n,std::size_t b){auto i=classes_.find(n);if(i==classes_.end())return;auto&p=i->second;if(p.instances)--p.instances;p.bytes=p.bytes>b?p.bytes-b:0;if(totalInstances_)--totalInstances_;totalBytes_=totalBytes_>b?totalBytes_-b:0;}
}