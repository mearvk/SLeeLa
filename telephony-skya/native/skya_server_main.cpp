#include "skya_engine.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
int main(int argc,char**argv){
skya_options_t o{}; o.max_peers=256; o.port=8443; o.http_version=3; std::string room="lobby";
for(int i=1;i<argc;i++){
if(!std::strcmp(argv[i],"--http2")) o.http_version=2;
else if(!std::strcmp(argv[i],"--http3")) o.http_version=3;
else if(!std::strcmp(argv[i],"--port")&&i+1<argc) o.port=(unsigned short)std::strtoul(argv[++i],nullptr,10);
else if(!std::strcmp(argv[i],"--room")&&i+1<argc) room=argv[++i];
else if(!std::strcmp(argv[i],"--max-peers")&&i+1<argc) o.max_peers=(unsigned)std::strtoul(argv[++i],nullptr,10);
else if(!std::strcmp(argv[i],"--help")){std::printf("Skya Server\n  --room <name>\n  --port <port>\n  --max-peers <n>\n  --http2 | --http3\n");return 0;}
}
skya_engine_t*e=skya_create(&o); if(!e)return 1; int rc=skya_start(e,SKYA_SERVER); if(rc==0)rc=skya_join(e,room.c_str());
if(rc==0)std::printf("Skya Server %s HTTP/%u port=%u room=%s max-peers=%u\n",skya_status(e),o.http_version,o.port,room.c_str(),o.max_peers);
skya_destroy(e); return rc?1:0;
