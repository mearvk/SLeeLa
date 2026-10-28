#include "sleela_network.hpp"
#include <cerrno>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
namespace sleela::network {
static int fam(AddressFamily f){return f==AddressFamily::IPv4?AF_INET:f==AddressFamily::IPv6?AF_INET6:AF_UNSPEC;}
static Status resolve(const Endpoint&e,int type,sockaddr_storage*out,socklen_t*len){addrinfo h{};h.ai_socktype=type;h.ai_family=fam(e.family);addrinfo*r=nullptr;auto p=std::to_string(e.port);if(getaddrinfo(e.host.c_str(),p.c_str(),&h,&r)!=0||!r)return Status::Error;std::memcpy(out,r->ai_addr,r->ai_addrlen);*len=(socklen_t)r->ai_addrlen;freeaddrinfo(r);return Status::Ok;}
Socket::Socket()=default;Socket::~Socket(){close();}
Status Socket::open(Transport t,AddressFamily f){if(fd_!=-1)return Status::Invalid;int type=t==Transport::TCP?SOCK_STREAM:SOCK_DGRAM;addrinfo h{};h.ai_socktype=type;h.ai_family=fam(f);h.ai_flags=AI_PASSIVE;addrinfo*r=nullptr;if(getaddrinfo(nullptr,"0",&h,&r)!=0||!r)return Status::Error;fd_=::socket(r->ai_family,type,0);family_=f;transport_=t;freeaddrinfo(r);return fd_>=0?Status::Ok:Status::Error;}
Status Socket::bind(const Endpoint&e){if(fd_<0)return Status::Closed;sockaddr_storage a{};socklen_t n=0;if(resolve(e,transport_==Transport::TCP?SOCK_STREAM:SOCK_DGRAM,&a,&n)!=Status::Ok)return Status::Error;int one=1;setsockopt(fd_,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));return ::bind(fd_,reinterpret_cast<sockaddr*>(&a),n)==0?Status::Ok:Status::Error;}
Status Socket::connect(const Endpoint&e){if(fd_<0)return Status::Closed;sockaddr_storage a{};socklen_t n=0;if(resolve(e,transport_==Transport::TCP?SOCK_STREAM:SOCK_DGRAM,&a,&n)!=Status::Ok)return Status::Error;return ::connect(fd_,reinterpret_cast<sockaddr*>(&a),n)==0?(connected_=true,Status::Ok):Status::Error;}
Status Socket::send(const std::vector<std::uint8_t>&d){if(fd_<0)return Status::Closed;return ::send(fd_,d.data(),d.size(),0)==(ssize_t)d.size()?Status::Ok:Status::Error;}
ReceiveResult Socket::receive(std::size_t n){ReceiveResult r;if(fd_<0){r.status=Status::Closed;return r;}std::vector<std::uint8_t>b(n);ssize_t z=::recv(fd_,b.data(),b.size(),0);if(z<0){r.status=(errno==EAGAIN||errno==EWOULDBLOCK)?Status::Timeout:Status::Error;return r;}if(z==0){r.status=Status::Closed;return r;}b.resize((size_t)z);r.status=Status::Ok;r.data=std::move(b);return r;}
Status Socket::close(){if(fd_>=0){::close(fd_);fd_=-1;}connected_=false;return Status::Ok;} bool Socket::openState()const noexcept{return fd_>=0;}
TcpConnection::TcpConnection():socket_(std::make_unique<Socket>()){} TcpConnection::~TcpConnection(){close();}
Status TcpConnection::connect(const Endpoint&e){auto s=socket_->open(Transport::TCP,e.family);return s==Status::Ok?socket_->connect(e):s;}
Status TcpConnection::send(const std::vector<std::uint8_t>&d){return socket_->send(d);} ReceiveResult TcpConnection::receive(std::size_t n){return socket_->receive(n);} Status TcpConnection::close(){return socket_->close();}
}