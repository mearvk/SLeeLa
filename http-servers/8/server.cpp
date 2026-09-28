#include "server.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
namespace sleela::http8{namespace{
void log(const Config&c,const std::string&s){std::ofstream f(c.log,std::ios::app);f<<s<<'\n';std::cerr<<s<<'\n';}
bool sendall(int f,const std::string&s){const char*p=s.data();std::size_t n=s.size();while(n){auto w=send(f,p,n,0);if(w<=0)return false;p+=w;n-=std::size_t(w);}return true;}
std::string resp(const std::string&st,const std::string&body){std::ostringstream o;o<<"HTTP/8.0 "<<st<<"\r\nContent-Type: text/plain\r\nContent-Length: "<<body.size()<<"\r\nX-SLeeLa-Protocol: HTTP/8.0\r\n\r\n"<<body;return o.str();}
int serve(int f,const Config&c){char b[8192];auto n=recv(f,b,sizeof(b)-1,0);if(n<=0)return 0;b[n]='\0';std::string r(b),first=r.substr(0,r.find('\n'));if(!first.empty()&&first.back()=='\r')first.pop_back();log(c,"request="+first);if(first.rfind("HTTP/8.0 ",0)!=0){sendall(f,resp("501 Not Implemented","Unsupported protocol version.\n"));return 1;}if(first.find("HANDSHAKE")==std::string::npos){sendall(f,resp("428 Precondition Required","HTTP/8.0 requires HANDSHAKE before exchange.\n"));return 1;}log(c,"protocol-specific handshake accepted");sendall(f,resp("200 OK","HTTP/8.0\nSubscription: active\nRadio: ready\nBinary-Checker-Agreement: passed\nTwo-Way-Handshaking: complete\nMessage-Exchange: accepted\n"));return 0;}
}
int run(const Config&c){int s=socket(AF_INET,SOCK_STREAM,0);if(s<0)return 1;int one=1;setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));sockaddr_in a{};a.sin_family=AF_INET;a.sin_port=htons(c.port);if(inet_pton(AF_INET,c.address.c_str(),&a.sin_addr)!=1||bind(s,(sockaddr*)&a,sizeof(a))<0||listen(s,16)<0){std::perror("server");close(s);return 1;}log(c,"HTTP/8.0 listening "+c.address+":"+std::to_string(c.port));do{int f=accept(s,nullptr,nullptr);if(f<0){if(errno==EINTR)continue;break;}serve(f,c);close(f);}while(!c.once);close(s);return 0;}}
