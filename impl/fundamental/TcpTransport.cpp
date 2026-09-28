#include "TcpTransport.hpp"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
using sleela_socket_t=SOCKET;
static constexpr sleela_socket_t invalid_socket=INVALID_SOCKET;
#else
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
using sleela_socket_t=int;
static constexpr sleela_socket_t invalid_socket=-1;
#endif
namespace sleela::fundamental {
TcpTransport::~TcpTransport(){ close(); }
bool TcpTransport::connect(const NetworkEndpoint& e){
#if defined(_WIN32)
 static bool w=false; if(!w){WSADATA d{}; w=WSAStartup(MAKEWORD(2,2),&d)==0;} if(!w)return false;
#endif
 if(e.host().empty() || e.port() == 0) return false;
 close();
 addrinfo h{},*r=nullptr; h.ai_socktype=SOCK_STREAM; h.ai_family=AF_UNSPEC;
 auto p=std::to_string(e.port());
 if(getaddrinfo(e.host().c_str(),p.c_str(),&h,&r)!=0) return false;
 for(auto*a=r;a;a=a->ai_next){
   if(a->ai_addrlen > static_cast<decltype(a->ai_addrlen)>(std::numeric_limits<socklen_t>::max())) continue;
   auto s=::socket(a->ai_family,a->ai_socktype,a->ai_protocol); if(s==invalid_socket) continue;
   if(::connect(s,a->ai_addr,static_cast<socklen_t>(a->ai_addrlen))==0){
     handle_=static_cast<long long>(s); freeaddrinfo(r); return true;
   }
#if defined(_WIN32)
   closesocket(s);
#else
   ::close(s);
#endif
 }
 freeaddrinfo(r); return false;
}
std::ptrdiff_t TcpTransport::send(const std::uint8_t* b,std::size_t n){
 if(handle_<0 || (n!=0 && b==nullptr) || n>static_cast<std::size_t>(std::numeric_limits<int>::max())) return -1;
 if(n==0) return 0;
 const int len=static_cast<int>(n);
#if defined(_WIN32)
 return static_cast<std::ptrdiff_t>(::send(static_cast<SOCKET>(handle_),reinterpret_cast<const char*>(b),len,0));
#else
 return static_cast<std::ptrdiff_t>(::send(static_cast<int>(handle_),b,n,MSG_NOSIGNAL));
#endif
}
std::ptrdiff_t TcpTransport::receive(std::uint8_t* b,std::size_t n){
 if(handle_<0 || (n!=0 && b==nullptr) || n>static_cast<std::size_t>(std::numeric_limits<int>::max())) return -1;
 if(n==0) return 0;
 const int len=static_cast<int>(n);
#if defined(_WIN32)
 return static_cast<std::ptrdiff_t>(::recv(static_cast<SOCKET>(handle_),reinterpret_cast<char*>(b),len,0));
#else
 return static_cast<std::ptrdiff_t>(::recv(static_cast<int>(handle_),b,n,0));
#endif
}
void TcpTransport::close() noexcept{
 if(handle_<0)return;
#if defined(_WIN32)
 closesocket(static_cast<SOCKET>(handle_));
#else
 ::close(static_cast<int>(handle_));
#endif
 handle_=-1;
}
bool TcpTransport::connected()const noexcept{return handle_>=0;}
}
