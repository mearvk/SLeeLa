#include "TcpTransport.hpp"
#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
using sleela_socket_t=SOCKET; static constexpr sleela_socket_t invalid_socket=INVALID_SOCKET;
#else
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
using sleela_socket_t=int; static constexpr sleela_socket_t invalid_socket=-1;
#endif
namespace sleela::fundamental {
TcpTransport::~TcpTransport(){close();}
bool TcpTransport::connect(const NetworkEndpoint&e){
#if defined(_WIN32)
static bool w=false;if(!w){WSADATA d{};w=WSAStartup(MAKEWORD(2,2),&d)==0;}if(!w)return false;
#endif
addrinfo h{},*r=nullptr;h.ai_socktype=SOCK_STREAM;h.ai_family=AF_UNSPEC;auto p=std::to_string(e.port());if(getaddrinfo(e.host().c_str(),p.c_str(),&h,&r)!=0)return false;
for(auto*a=r;a;a=a->ai_next){auto s=::socket(a->ai_family,a->ai_socktype,a->ai_protocol);if(s==invalid_socket)continue;if(::connect(s,a->ai_addr,(int)a->ai_addrlen)==0){handle_=(long long)s;freeaddrinfo(r);return true;}
#if defined(_WIN32)
closesocket(s);
#else
::close(s);
#endif
}freeaddrinfo(r);return false;}
std::ptrdiff_t TcpTransport::send(const std::uint8_t*b,std::size_t n){return handle_<0 ? -1 : ::send((sleela_socket_t)handle_,reinterpret_cast<const char*>(b),(int)n,0);}
std::ptrdiff_t TcpTransport::receive(std::uint8_t*b,std::size_t n){return handle_<0 ? -1 : ::recv((sleela_socket_t)handle_,reinterpret_cast<char*>(b),(int)n,0);}
void TcpTransport::close()noexcept{if(handle_<0)return;auto s=(sleela_socket_t)handle_;
#if defined(_WIN32)
closesocket(s);
#else
::close(s);
#endif
handle_=-1;} bool TcpTransport::connected()const noexcept{return handle_>=0;} }