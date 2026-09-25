#include "sleela_memory_manager.h"
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include <unordered_map>
#include <string>
#include <algorithm>
struct B{void*p;size_t n;}; struct E{uint64_t id;std::string name;void*p;size_t n;bool leech;}; struct sleela_mm{mutable std::mutex m;std::unordered_map<void*,B>b;std::unordered_map<uint64_t,E>e;uint64_t next=1,cond=0;size_t resident=0,allocated=0,freed=0,allocs=0,frees=0,peak=0; sleela_mm_limits lim{0,(size_t)-1};sleela_mm_flow_model fm{1,1,1};};
extern "C" sleela_mm* sleela_mm_create(){return new sleela_mm;}
extern "C" void sleela_mm_destroy(sleela_mm*x){if(!x)return;std::lock_guard<std::mutex>g(x->m);for(auto&q:x->b)free(q.second.p);delete x;}
extern "C" void* sleela_mm_alloc(sleela_mm*x,size_t n){if(!x||!n)return nullptr;std::lock_guard<std::mutex>g(x->m);if(n>x->lim.hard_limit-x->resident)return nullptr;void*p=malloc(n);if(!p)return nullptr;x->b[p]={p,n};x->resident+=n;x->allocated+=n;x->allocs++;x->peak=std::max(x->peak,x->resident);return p;}
extern "C" void* sleela_mm_calloc(sleela_mm*x,size_t c,size_t n){if(n&&c>((size_t)-1)/n)return nullptr;void*p=sleela_mm_alloc(x,c*n);if(p)memset(p,0,c*n);return p;}
extern "C" void* sleela_mm_realloc(sleela_mm*x,void*p,size_t n){if(!p)return sleela_mm_alloc(x,n);if(!x)return nullptr;std::lock_guard<std::mutex>g(x->m);auto i=x->b.find(p);if(i==x->b.end())return nullptr;size_t old=i->second.n;if(n>old&&n-old>x->lim.hard_limit-x->resident)return nullptr;void*q=realloc(p,n);if(!q)return nullptr;x->b.erase(i);x->b[q]={q,n};x->resident=x->resident-old+n;x->allocated+=n;x->freed+=old;x->peak=std::max(x->peak,x->resident);return q;}
extern "C" sleela_mm_status sleela_mm_free(sleela_mm*x,void*p){if(!x||!p)return SLEELA_MM_INVALID;std::lock_guard<std::mutex>g(x->m);auto i=x->b.find(p);if(i==x->b.end())return SLEELA_MM_NOT_FOUND;x->resident-=i->second.n;x->freed+=i->second.n;x->frees++;free(p);x->b.erase(i);return SLEELA_MM_OK;}
extern "C" sleela_mm_status sleela_mm_insert_struct(sleela_mm*x,const char*n,const void*s,size_t z,size_t,uint64_t*out){if(!x||!s||!z||!out)return SLEELA_MM_INVALID;void*p=sleela_mm_alloc(x,z);if(!p)return SLEELA_MM_LIMIT;memcpy(p,s,z);std::lock_guard<std::mutex>g(x->m);uint64_t id=x->next++;x->e[id]={id,n?n:"struct",p,z,false};*out=id;return SLEELA_MM_OK;}
extern "C" sleela_mm_status sleela_mm_remove(sleela_mm*x,uint64_t id){if(!x)return SLEELA_MM_INVALID;std::lock_guard<std::mutex>g(x->m);auto i=x->e.find(id);if(i==x->e.end()||i->second.leech)return SLEELA_MM_NOT_FOUND;auto b=x->b.find(i->second.p);if(b!=x->b.end()){x->resident-=b->second.n;x->freed+=b->second.n;free(b->second.p);x->b.erase(b);}x->e.erase(i);return SLEELA_MM_OK;}
extern "C" sleela_mm_status sleela_mm_leech_attach(sleela_mm*x,const char*n,uint64_t target,uint64_t*out){if(!x||!out)return SLEELA_MM_INVALID;std::lock_guard<std::mutex>g(x->m);auto i=x->e.find(target);if(i==x->e.end())return SLEELA_MM_NOT_FOUND;uint64_t id=x->next++;x->e[id]={id,n?n:"leech",i->second.p,i->second.n,true};*out=id;return SLEELA_MM_OK;}
extern "C" sleela_mm_status sleela_mm_leech_detach(sleela_mm*x,uint64_t id){if(!x)return SLEELA_MM_INVALID;std::lock_guard<std::mutex>g(x->m);auto i=x->e.find(id);if(i==x->e.end()||!i->second.leech)return SLEELA_MM_NOT_FOUND;x->e.erase(i);return SLEELA_MM_OK;}
extern "C" void sleela_mm_set_limits(sleela_mm*x,sleela_mm_limits l){if(x){std::lock_guard<std::mutex>g(x->m);x->lim=l;}}
extern "C" void sleela_mm_set_flow_model(sleela_mm*x,sleela_mm_flow_model f){if(x){std::lock_guard<std::mutex>g(x->m);x->fm=f;}}
extern "C" sleela_mm_stats sleela_mm_stats_get(const sleela_mm*x){sleela_mm_stats s{};if(!x)return s;std::lock_guard<std::mutex>g(x->m);s.resident=x->resident;s.allocated=x->allocated;s.freed=x->freed;s.allocations=x->allocs;s.frees=x->frees;s.peak=x->peak;s.pressure=x->lim.hard_limit?double(x->resident)/x->lim.hard_limit:0;s.voltage=s.pressure*x->fm.voltage_per_byte;s.current=double(x->allocs-x->frees)*x->fm.current_per_allocation;s.flow=(s.voltage+s.current)*x->fm.flow_gain;s.condition=x->cond;return s;}
extern "C" uint64_t sleela_mm_condition(const sleela_mm*x){return sleela_mm_stats_get(x).condition;} extern "C" int sleela_mm_validate(const sleela_mm*x){return x?1:0;}
