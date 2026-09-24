#include "skya_engine.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <string>
#include <thread>
#include <chrono>
static volatile std::sig_atomic_t g_stop=0;
static void handle_signal(int){g_stop=1;}
int main(int argc,char**argv){
 skya_options_t o{};o.max_peers=256;o.port=8443;o.http_version=3;std::string room="lobby";
 for(int i=1;i<argc;i++){if(!std::strcmp(argv[i],"--http2"))o.http_version=2;else if(!std::strcmp(argv[i],"--http3"))o.http_version=3;else if(!std::strcmp(argv[i],"--port")&&i+1<argc)o.port=(unsigned short)std::strtoul(argv[++i],nullptr,10);else if(!std::strcmp(argv[i],"--room")&&i+1<argc)room=argv[++i];else if(!std::strcmp(argv[i],"--max-peers")&&i+1<argc)o.max_peers=(unsigned)std::strtoul(argv[++i],nullptr,10);else if(!std::strcmp(argv[i],"--help")){std::printf("Skya Server\n  --room <name>\n  --port <port>\n  --max-peers <n>\n  --http2 | --http3\n");return 0;}}
 std::signal(SIGINT,handle_signal);std::signal(SIGTERM,handle_signal);
 skya_engine_t*e=skya_create(&o);if(!e){std::fprintf(stderr,"Skya Server: engine initialization failed.\n");return 1;}
 int rc=skya_start(e,SKYA_SERVER);
 if(rc!=0){std::fprintf(stderr,"Skya Server: FAILED to initialize listener on port %u; status=%s\n",(unsigned)o.port,skya_status(e));skya_destroy(e);return 1;}
 rc=skya_join(e,room.c_str());if(rc!=0){std::fprintf(stderr,"Skya Server: FAILED to initialize room '%s'.\n",room.c_str());skya_destroy(e);return 1;}
 if(!skya_port_bound(e)){std::fprintf(stderr,"Skya Server: FAILED - socket is not bound.\n");skya_destroy(e);return 1;}
 std::printf("Skya Server READY: %s HTTP/%u listening on 0.0.0.0:%u room=%s max-peers=%u\n",skya_status(e),o.http_version,(unsigned)skya_bound_port(e),room.c_str(),o.max_peers);std::fflush(stdout);
 while(!g_stop)std::this_thread::sleep_for(std::chrono::milliseconds(250));
 std::printf("Skya Server: stopping.\n");skya_stop(e);skya_destroy(e);return 0;
}
