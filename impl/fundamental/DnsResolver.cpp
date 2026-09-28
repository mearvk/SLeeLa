#include "DnsResolver.hpp"
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
namespace sleela::fundamental { std::vector<std::string> DnsResolver::resolve(const std::string&host)const{std::vector<std::string>v;addrinfo h{},*r=nullptr;h.ai_family=AF_UNSPEC;if(getaddrinfo(host.c_str(),nullptr,&h,&r)!=0)return v;for(auto*a=r;a;a=a->ai_next){char b[INET6_ADDRSTRLEN]{};void*x=nullptr;if(a->ai_family==AF_INET)x=&reinterpret_cast<sockaddr_in*>(a->ai_addr)->sin_addr;else if(a->ai_family==AF_INET6)x=&reinterpret_cast<sockaddr_in6*>(a->ai_addr)->sin6_addr;else continue;if(inet_ntop(a->ai_family,x,b,sizeof b))v.emplace_back(b);}freeaddrinfo(r);return v;} }