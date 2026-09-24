#include "skya_sleela_bridge.h"
#include "skya_engine.h"
extern "C" {
#include "../../../impl/core/sleela_core.h"
}
#include "../../../api/http-multiplexing/sleela_http_api.h"
#include "../../../api/server/nat_aware.h"
#include <cstdio>
#include <cstring>
#include <string>
int skya_sleela_runtime_probe(void){SLVM*vm=slvm_new();if(!vm)return -1;int safe=slvm_memory_safe_mode(vm);slvm_free(vm);return safe?0:-2;}
int skya_sleela_initial_policy(skya_policy_t*p){if(skya_sleela_runtime_probe()!=0)return -1;return skya_policy_initial(p);}
int skya_sleela_command(int argc,char**argv){skya_policy_t p{};if(skya_sleela_initial_policy(&p)!=0){std::fprintf(stderr,"Skya: unable to establish SLeeLa runtime/policy\n");return 1;}if(argc>=2&&!std::strcmp(argv[1],"plan")){std::printf("Skya initial policy: %s\n",p.summary);return 0;}skya_options_t o{};o.max_peers=256;o.port=p.port;o.http_version=p.http_version;o.relay=p.relay;skya_role_t role=SKYA_BOTH;std::string room="lobby";for(int i=1;i<argc;i++){if(!std::strcmp(argv[i],"--server"))role=SKYA_SERVER;else if(!std::strcmp(argv[i],"--client"))role=SKYA_CLIENT;else if(!std::strcmp(argv[i],"--both"))role=SKYA_BOTH;else if(!std::strcmp(argv[i],"--room")&&i+1<argc)room=argv[++i];}skya_engine_t*e=skya_create(&o);if(!e)return 1;int rc=skya_start(e,role);if(!rc)rc=skya_join(e,room.c_str());std::printf("Skya %s | %s | room=%s\n",skya_status(e),p.summary,room.c_str());skya_destroy(e);return rc?1:0;}
