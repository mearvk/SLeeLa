#include "skya_engine.h"
#include <cstdio>
#include <cstring>
#include <string>
int main(int argc,char**argv){skya_options_t o{};o.max_peers=256;o.port=8443;o.http_version=3;skya_role_t role=SKYA_BOTH;std::string room="lobby";for(int i=1;i<argc;i++){if(!std::strcmp(argv[i],"--server"))role=SKYA_SERVER;else if(!std::strcmp(argv[i],"--client"))role=SKYA_CLIENT;else if(!std::strcmp(argv[i],"--both"))role=SKYA_BOTH;else if(!std::strcmp(argv[i],"--http2"))o.http_version=2;else if(!std::strcmp(argv[i],"--http3"))o.http_version=3;else if(!std::strcmp(argv[i],"--room")&&i+1<argc)room=argv[++i];}auto*e=skya_create(&o);if(!e)return 1;int rc=skya_start(e,role);if(rc==0)rc=skya_join(e,room.c_str());std::printf("Skya %s HTTP/%u port=%u room=%s\n",skya_status(e),o.http_version,o.port,room.c_str());skya_destroy(e);return rc?1:0;}
