#ifndef SLEELA_SECURITY_SUPERVISOR_HPP
#define SLEELA_SECURITY_SUPERVISOR_HPP
#include <cstddef>
#include <string>
#include <unordered_map>
namespace sleela::runtime {
enum class SecurityRole { User, Business, Corporation, LLC, Fiduciary, Contract, Attorney, Senator, Democrat, Custom };
struct ClassPolicy { bool allowed=false; std::size_t maxInstances=0,maxBytes=0,instances=0,bytes=0; };
struct RolePolicy { std::size_t maxTotalInstances=0,maxTotalBytes=0; };
class SecuritySupervisor {
 SecurityRole activeRole_; std::unordered_map<std::string,ClassPolicy> classes_; std::unordered_map<int,RolePolicy> roles_; std::size_t totalInstances_=0,totalBytes_=0;
 static int key(SecurityRole r){return static_cast<int>(r);}
public:
 explicit SecuritySupervisor(SecurityRole r=SecurityRole::User):activeRole_(r){}
 void setClassPolicy(const std::string&,bool,std::size_t,std::size_t);
 void setRolePolicy(SecurityRole,std::size_t,std::size_t);
 bool authorizeClass(const std::string&)const;
 bool reserve(const std::string&,std::size_t);
 void release(const std::string&,std::size_t);
 void setActiveRole(SecurityRole r){activeRole_=r;}
 std::size_t totalInstances()const{return totalInstances_;}
 std::size_t totalBytes()const{return totalBytes_;}
};
}
#endif