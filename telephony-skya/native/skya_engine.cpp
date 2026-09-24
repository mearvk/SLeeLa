#include "skya_engine.h"
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <cerrno>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
using skya_socket_t = SOCKET;
static constexpr skya_socket_t SKYA_INVALID_SOCKET = INVALID_SOCKET;
static void skya_close_socket(skya_socket_t s) { if (s != INVALID_SOCKET) closesocket(s); }
static int skya_socket_error() { return WSAGetLastError(); }
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using skya_socket_t = int;
static constexpr skya_socket_t SKYA_INVALID_SOCKET = -1;
static void skya_close_socket(skya_socket_t s) { if (s >= 0) close(s); }
static int skya_socket_error() { return errno; }
#endif
struct skya_engine {
 skya_options_t o{}; skya_role_t role{SKYA_BOTH}; bool running{}; bool port_bound{}; uint16_t bound_port{};
 std::string room; std::string status{"created"}; std::vector<skya_peer_t> peers; std::vector<skya_socket_t> clients;
 skya_socket_t listen_socket{SKYA_INVALID_SOCKET}; std::thread accept_thread; mutable std::mutex m;
};
#ifdef _WIN32
static bool skya_socket_runtime_init(){WSADATA d{};return WSAStartup(MAKEWORD(2,2),&d)==0;}
static void skya_socket_runtime_cleanup(){WSACleanup();}
#else
static bool skya_socket_runtime_init(){return true;} static void skya_socket_runtime_cleanup(){}
#endif
static bool skya_bind_listener(skya_engine*e){
 if(!skya_socket_runtime_init()){e->status="socket-init-failed";return false;}
 skya_socket_t s=::socket(AF_INET,SOCK_STREAM,0); if(s==SKYA_INVALID_SOCKET){e->status="socket-create-failed";skya_socket_runtime_cleanup();return false;}
 int reuse=1;
#ifdef _WIN32
 setsockopt(s,SOL_SOCKET,SO_REUSEADDR,reinterpret_cast<const char*>(&reuse),sizeof(reuse));
#else
 setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
#endif
 sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_ANY);a.sin_port=htons(e->o.port);
 if(::bind(s,reinterpret_cast<sockaddr*>(&a),sizeof(a))!=0){int err=skya_socket_error();skya_close_socket(s);e->status="socket-bind-failed";std::fprintf(stderr,"Skya Server: socket bind failed on port %u (error %d).\n",(unsigned)e->o.port,err);skya_socket_runtime_cleanup();return false;}
 if(::listen(s,64)!=0){int err=skya_socket_error();skya_close_socket(s);e->status="socket-listen-failed";std::fprintf(stderr,"Skya Server: socket listen failed on port %u (error %d).\n",(unsigned)e->o.port,err);skya_socket_runtime_cleanup();return false;}
 e->listen_socket=s;e->port_bound=true;e->bound_port=e->o.port;e->status="running-listening";return true;
}
static void skya_accept_loop(skya_engine*e){
 while(e->running){
  sockaddr_in peer{};
#ifdef _WIN32
  int peer_len=sizeof(peer);
#else
  socklen_t peer_len=sizeof(peer);
#endif
  skya_socket_t c=::accept(e->listen_socket,reinterpret_cast<sockaddr*>(&peer),&peer_len);
  if(c==SKYA_INVALID_SOCKET){if(e->running)std::fprintf(stderr,"Skya Server: accept failed (error %d).\n",skya_socket_error());break;}
  char address[INET_ADDRSTRLEN]{};::inet_ntop(AF_INET,&peer.sin_addr,address,sizeof(address));
  std::string room;
  {std::lock_guard<std::mutex> lock(e->m);room=e->room;if(e->clients.size()>=e->o.max_peers){
    const char*full="SKYA/1 server-busy\r\n";
#ifdef _WIN32
    ::send(c,full,(int)std::strlen(full),0);
#else
    ::send(c,full,std::strlen(full),0);
#endif
    skya_close_socket(c);continue;}e->clients.push_back(c);}
  char ready[256]{};std::snprintf(ready,sizeof(ready),"SKYA/1 server-ready port=%u room=%s http=%u\r\n",(unsigned)e->o.port,room.c_str(),(unsigned)e->o.http_version);
#ifdef _WIN32
  ::send(c,ready,(int)std::strlen(ready),0);
#else
  ::send(c,ready,std::strlen(ready),0);
#endif
  skya_peer_t p{};p.id=(uint64_t)e->clients.size();std::snprintf(p.address,sizeof(p.address),"%s",address);p.port=ntohs(peer.sin_port);p.http_version=e->o.http_version;
  {std::lock_guard<std::mutex> lock(e->m);e->peers.push_back(p);}
 }
}
extern "C" skya_engine_t*skya_create(const skya_options_t*o){auto*e=new skya_engine;if(o)e->o=*o;if(!e->o.max_peers)e->o.max_peers=256;if(!e->o.port)e->o.port=8443;if(!e->o.http_version)e->o.http_version=3;return e;}
extern "C" void skya_destroy(skya_engine_t*e){if(e){skya_stop(e);delete e;}}
extern "C" int skya_start(skya_engine_t*e,skya_role_t r){
 if(!e)return -1;{std::lock_guard<std::mutex>l(e->m);if(e->running)return 0;e->role=r;e->running=true;e->status="starting";}
 if(r==SKYA_CLIENT){std::lock_guard<std::mutex>l(e->m);e->status="running";return 0;}
 if(!skya_bind_listener(e)){std::lock_guard<std::mutex>l(e->m);e->running=false;e->port_bound=false;return -3;}
 e->accept_thread=std::thread(skya_accept_loop,e);std::fprintf(stdout,"Skya Server: listening on 0.0.0.0:%u (TCP control/session listener).\n",(unsigned)e->bound_port);return 0;
}
extern "C" void skya_stop(skya_engine_t*e){
 if(!e)return;bool was_running;{std::lock_guard<std::mutex>l(e->m);was_running=e->running;e->running=false;}
 if(!was_running&&e->listen_socket==SKYA_INVALID_SOCKET)return;
 if(e->listen_socket!=SKYA_INVALID_SOCKET){skya_close_socket(e->listen_socket);e->listen_socket=SKYA_INVALID_SOCKET;}
 if(e->accept_thread.joinable())e->accept_thread.join();
 {std::lock_guard<std::mutex>l(e->m);for(auto s:e->clients)skya_close_socket(s);e->clients.clear();e->peers.clear();e->port_bound=false;e->bound_port=0;e->status="stopped";}
 skya_socket_runtime_cleanup();
}
extern "C" int skya_join(skya_engine_t*e,const char*r){if(!e||!r||!*r)return -1;std::lock_guard<std::mutex>l(e->m);if(!e->running)return -2;e->room=r;return 0;}
extern "C" int skya_message(skya_engine_t*e,const char*r,const char*t){if(!e||!r||!t)return -1;std::lock_guard<std::mutex>l(e->m);return e->running&&e->room==r?0:-2;}
extern "C" size_t skya_peer_count(const skya_engine_t*e){if(!e)return 0;std::lock_guard<std::mutex>l(e->m);return e->peers.size();}
extern "C" int skya_peer_at(const skya_engine_t*e,size_t i,skya_peer_t*out){if(!e||!out)return -1;std::lock_guard<std::mutex>l(e->m);if(i>=e->peers.size())return -2;*out=e->peers[i];return 0;}
extern "C" const char*skya_status(const skya_engine_t*e){return e?e->status.c_str():"invalid";}
extern "C" int skya_port_bound(const skya_engine_t*e){return e&&e->port_bound?1:0;}
extern "C" uint16_t skya_bound_port(const skya_engine_t*e){return e?e->bound_port:0;}
