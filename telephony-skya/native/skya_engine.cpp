#include "skya_engine.h"
#include <mutex>
#include <string>
#include <vector>
struct skya_engine { skya_options_t o{}; skya_role_t role{SKYA_BOTH}; bool running{}; std::string room; std::string status{"created"}; std::vector<skya_peer_t> peers; mutable std::mutex m; };
extern "C" skya_engine_t *skya_create(const skya_options_t *o){ auto *e=new skya_engine; if(o)e->o=*o; if(!e->o.max_peers)e->o.max_peers=256; if(!e->o.port)e->o.port=8443; if(!e->o.http_version)e->o.http_version=3; return e; }
extern "C" void skya_destroy(skya_engine_t*e){delete e;}
extern "C" int skya_start(skya_engine_t*e,skya_role_t r){if(!e)return -1;std::lock_guard<std::mutex>l(e->m);e->role=r;e->running=true;e->status="running";return 0;}
extern "C" void skya_stop(skya_engine_t*e){if(!e)return;std::lock_guard<std::mutex>l(e->m);e->running=false;e->peers.clear();e->status="stopped";}
extern "C" int skya_join(skya_engine_t*e,const char*r){if(!e||!r||!*r)return -1;std::lock_guard<std::mutex>l(e->m);if(!e->running)return -2;e->room=r;return 0;}
extern "C" int skya_message(skya_engine_t*e,const char*r,const char*t){if(!e||!r||!t)return -1;std::lock_guard<std::mutex>l(e->m);return e->running&&e->room==r?0:-2;}
extern "C" size_t skya_peer_count(const skya_engine_t*e){if(!e)return 0;std::lock_guard<std::mutex>l(e->m);return e->peers.size();}
extern "C" int skya_peer_at(const skya_engine_t*e,size_t i,skya_peer_t*out){if(!e||!out)return -1;std::lock_guard<std::mutex>l(e->m);if(i>=e->peers.size())return -2;*out=e->peers[i];return 0;}
extern "C" const char *skya_status(const skya_engine_t*e){return e?e->status.c_str():"invalid";}
