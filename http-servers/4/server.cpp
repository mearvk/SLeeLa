#include "server.hpp"
#include "../common/packet_spec.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
namespace sleela::http4{namespace{
constexpr std::size_t H=sleela::packet::kHeaderSize; constexpr std::size_t MAX=sleela::packet::kMaxPayload;constexpr unsigned V=4,MAXTYPE=9;
enum:std::uint8_t{OPEN=1,DATA,END,RESET,WINDOW,PING,PONG,RESUME,CAPSULE};
std::uint16_t g16(const std::uint8_t*p){return(std::uint16_t(p[0])<<8)|p[1];} std::uint32_t g32(const std::uint8_t*p){return(std::uint32_t(p[0])<<24)|(std::uint32_t(p[1])<<16)|(std::uint32_t(p[2])<<8)|p[3];} std::uint64_t g64(const std::uint8_t*p){std::uint64_t v=0;for(int i=0;i<8;i++)v=(v<<8)|p[i];return v;}
void p16(std::vector<std::uint8_t>&b,std::size_t o,std::uint16_t v){b[o]=v>>8;b[o+1]=v;} void p32(std::vector<std::uint8_t>&b,std::size_t o,std::uint32_t v){b[o]=v>>24;b[o+1]=v>>16;b[o+2]=v>>8;b[o+3]=v;} void p64(std::vector<std::uint8_t>&b,std::size_t o,std::uint64_t v){for(int i=7;i>=0;i--){b[o+i]=std::uint8_t(v);v>>=8;}}
bool io(int f,void*x,std::size_t n,bool w){auto*p=(std::uint8_t*)x;while(n){auto r=w?send(f,p,n,0):recv(f,p,n,0);if(r<=0)return false;p+=r;n-=std::size_t(r);}return true;}
void log(const Config&c,const std::string&s){std::ofstream f(c.log,std::ios::app);f<<s<<'\n';std::cerr<<s<<'\n';}
const char*name(unsigned t){switch(t){case OPEN:return"OPEN";case DATA:return"DATA";case END:return"END";case RESET:return"RESET";case WINDOW:return"WINDOW";case PING:return"PING";case PONG:return"PONG";case RESUME:return"RESUME";case CAPSULE:return"CAPSULE";default:return"UNKNOWN";}}
int serve(int f,const Config&c){std::uint8_t h[H];bool opened=false;std::uint64_t last=0;for(;;){if(!io(f,h,H,false))return 0;const auto t=h[1];const auto flags=g16(h+2);const auto stream=g64(h+4);const auto request=g64(h+12);const auto seq=g64(h+20);const auto len=g32(h+28);if(h[0]!=V||t<1||t>MAXTYPE||len>MAX){log(c,"REJECT invalid frame");return 1;}std::vector<std::uint8_t>p(len);if(len&&!io(f,p.data(),len,false))return 0;std::ostringstream q;q<<"packet type="<<name(t)<<" flags=0x"<<std::hex<<flags<<std::dec<<" stream="<<stream<<" request="<<request<<" sequence="<<seq<<" payload="<<len;log(c,q.str());if(!opened){if(t!=OPEN||seq!=0){log(c,"REJECT handshake: first frame must be OPEN sequence=0");return 1;}opened=true;}else if(seq<last){log(c,"REJECT sequence regression");return 1;}last=seq;std::string body=t==PING?"PONG":(t==OPEN?"SLeeLa HTTP/4.0 READY":std::string("ACK ")+name(t));std::vector<std::uint8_t>o(H+body.size());o[0]=V;o[1]=t==PING?PONG:OPEN;p16(o,2,2);p64(o,4,stream);p64(o,12,request);p64(o,20,seq);p32(o,28,std::uint32_t(body.size()));std::memcpy(o.data()+H,body.data(),body.size());if(!io(f,o.data(),o.size(),true))return 0;if(t==END)return 0;}}}
int run(const Config&c){int s=socket(AF_INET,SOCK_STREAM,0);if(s<0)return 1;int one=1;setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));sockaddr_in a{};a.sin_family=AF_INET;a.sin_port=htons(c.port);if(inet_pton(AF_INET,c.address.c_str(),&a.sin_addr)!=1||bind(s,(sockaddr*)&a,sizeof(a))<0||listen(s,16)<0){std::perror("server");close(s);return 1;}log(c,"HTTP/4.0 listening "+c.address+":"+std::to_string(c.port));do{int f=accept(s,nullptr,nullptr);if(f<0){if(errno==EINTR)continue;break;}serve(f,c);close(f);}while(!c.once);close(s);return 0;}}
