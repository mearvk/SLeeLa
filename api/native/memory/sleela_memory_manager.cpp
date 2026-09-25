#include "sleela_memory_manager.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <mutex>
#include <unordered_map>
#include <string>
#include <vector>

static constexpr uint64_t CANARY=0x534C45454C414D4DULL;
struct Header { uint64_t canary; size_t size; size_t alignment; };
struct Entry { void* user; void* raw; size_t size; size_t alignment; };
struct Object { uint64_t id; uint64_t target; bool leech; std::string name; };

struct sleela_mm {
  mutable std::mutex m;
  std::unordered_map<void*,Entry> blocks;
  std::unordered_map<uint64_t,Object> objects;
  uint64_t next=1;
  size_t resident=0, allocated=0, freed=0, allocations=0, frees=0, peak=0;
  sleela_mm_limits limits{0,(size_t)-1};
  sleela_mm_flow_model flow{1.0,1.0,1.0};
  sleela_mm_thresholds thresholds{0.70,0.90};
  uint64_t condition=SLEELA_MM_CONDITION_NORMAL;
};

static void* raw_alloc(size_t bytes,size_t alignment){
  if(alignment<alignof(void*)) alignment=alignof(void*);
  if((alignment&(alignment-1))!=0) alignment=alignof(void*);
  if(bytes>std::numeric_limits<size_t>::max()-sizeof(Header)-sizeof(uint64_t)-alignment)return nullptr; const size_t total=sizeof(Header)+bytes+sizeof(uint64_t)+alignment;
  void* raw=std::malloc(total);
  if(!raw)return nullptr;
  uintptr_t base=reinterpret_cast<uintptr_t>(raw)+sizeof(Header);
  uintptr_t user=(base+alignment-1)&~(uintptr_t)(alignment-1);
  auto* h=reinterpret_cast<Header*>(user-sizeof(Header));
  h->canary=CANARY; h->size=bytes; h->alignment=alignment;
  *reinterpret_cast<uint64_t*>(user+bytes)=CANARY;
  return reinterpret_cast<void*>(user);
}
static Header* header(void*p){return p?reinterpret_cast<Header*>(reinterpret_cast<uintptr_t>(p)-sizeof(Header)):nullptr;}
static bool valid_block(void*p){auto*h=header(p);return h&&h->canary==CANARY&&*reinterpret_cast<uint64_t*>(reinterpret_cast<uintptr_t>(p)+h->size)==CANARY;}

extern "C" sleela_mm* sleela_mm_create(){return new sleela_mm;}
extern "C" void sleela_mm_destroy(sleela_mm*x){if(!x)return;std::lock_guard<std::mutex>g(x->m);for(auto&kv:x->blocks)std::free(kv.second.raw);x->blocks.clear();x->objects.clear();delete x;}

extern "C" void* sleela_mm_alloc(sleela_mm*x,size_t n){
  if(!x||!n)return nullptr;std::lock_guard<std::mutex>g(x->m);
  if(n>x->limits.hard_limit-x->resident)return nullptr;
  void* user=raw_alloc(n,alignof(std::max_align_t));if(!user)return nullptr;
  x->blocks[user]={user,header(user),n,header(user)->alignment};
  x->resident+=n;x->allocated+=n;x->allocations++;x->peak=std::max(x->peak,x->resident);return user;
}
extern "C" void* sleela_mm_calloc(sleela_mm*x,size_t c,size_t n){if(n&&c>SIZE_MAX/n)return nullptr;void*p=sleela_mm_alloc(x,c*n);if(p)std::memset(p,0,c*n);return p;}

extern "C" void* sleela_mm_realloc(sleela_mm*x,void*p,size_t n){
  if(!p)return sleela_mm_alloc(x,n);if(!x||!n)return nullptr;std::lock_guard<std::mutex>g(x->m);
  auto it=x->blocks.find(p);if(it==x->blocks.end()||!valid_block(p))return nullptr;
  const size_t old=it->second.size;if(n>old&&n-old>x->limits.hard_limit-x->resident)return nullptr;
  void*q=raw_alloc(n,it->second.alignment);if(!q)return nullptr;std::memcpy(q,p,std::min(old,n));
  std::free(it->second.raw);x->blocks.erase(it);x->blocks[q]={q,header(q),n,header(q)->alignment};
  x->resident=x->resident-old+n;x->allocated+=n;x->freed+=old;x->peak=std::max(x->peak,x->resident);return q;
}
extern "C" sleela_mm_status sleela_mm_free(sleela_mm*x,void*p){
  if(!x||!p)return SLEELA_MM_INVALID;std::lock_guard<std::mutex>g(x->m);
  auto it=x->blocks.find(p);if(it==x->blocks.end())return SLEELA_MM_NOT_FOUND;
  if(!valid_block(p)){x->condition=SLEELA_MM_CONDITION_CORRUPT;return SLEELA_MM_CORRUPT;}
  const size_t n=it->second.size;x->resident-=n;x->freed+=n;x->frees++;
  for(auto oi=x->objects.begin();oi!=x->objects.end();) { if(!oi->second.leech && oi->second.target==reinterpret_cast<uint64_t>(p)) oi=x->objects.erase(oi); else ++oi; }
  std::free(it->second.raw);x->blocks.erase(it);return SLEELA_MM_OK;
}
extern "C" sleela_mm_status sleela_mm_insert_struct(sleela_mm*x,const char*n,const void*s,size_t z,size_t alignment,uint64_t*out){
  if(!x||!s||!z||!out)return SLEELA_MM_INVALID;void*p=raw_alloc(z,alignment);if(!p)return SLEELA_MM_LIMIT;
  {std::lock_guard<std::mutex>g(x->m);if(z>x->limits.hard_limit-x->resident){return SLEELA_MM_LIMIT;}
  std::memcpy(p,s,z);x->blocks[p]={p,header(p),z,header(p)->alignment};x->resident+=z;x->allocated+=z;x->allocations++;x->peak=std::max(x->peak,x->resident);
  const uint64_t id=x->next++;x->objects[id]={id,reinterpret_cast<uint64_t>(p),false,n?n:"struct"};*out=id;}return SLEELA_MM_OK;
}
extern "C" sleela_mm_status sleela_mm_remove(sleela_mm*x,uint64_t id){
  if(!x)return SLEELA_MM_INVALID;std::lock_guard<std::mutex>g(x->m);auto it=x->objects.find(id);if(it==x->objects.end()||it->second.leech)return SLEELA_MM_NOT_FOUND;
  void*p=reinterpret_cast<void*>(it->second.target);auto b=x->blocks.find(p);if(b==x->blocks.end()){x->condition=SLEELA_MM_CONDITION_CORRUPT;return SLEELA_MM_CORRUPT;}
  const size_t n=b->second.size;x->resident-=n;x->freed+=n;x->frees++;std::free(b->second.raw);x->blocks.erase(b);x->objects.erase(it);
  for(auto oi=x->objects.begin();oi!=x->objects.end();) {if(oi->second.leech&&oi->second.target==reinterpret_cast<uint64_t>(p))oi=x->objects.erase(oi);else ++oi;}return SLEELA_MM_OK;
}
extern "C" sleela_mm_status sleela_mm_leech_attach(sleela_mm*x,const char*n,uint64_t target,uint64_t*out){
  if(!x||!out)return SLEELA_MM_INVALID;std::lock_guard<std::mutex>g(x->m);auto it=x->objects.find(target);if(it==x->objects.end()||it->second.leech)return SLEELA_MM_NOT_FOUND;
  const uint64_t id=x->next++;x->objects[id]={id,it->second.target,true,n?n:"leech"};*out=id;return SLEELA_MM_OK;
}
extern "C" sleela_mm_status sleela_mm_leech_detach(sleela_mm*x,uint64_t id){if(!x)return SLEELA_MM_INVALID;std::lock_guard<std::mutex>g(x->m);auto it=x->objects.find(id);if(it==x->objects.end()||!it->second.leech)return SLEELA_MM_NOT_FOUND;x->objects.erase(it);return SLEELA_MM_OK;}
extern "C" void sleela_mm_set_limits(sleela_mm*x,sleela_mm_limits l){if(x){std::lock_guard<std::mutex>g(x->m);if(l.hard_limit<l.soft_limit)l.soft_limit=l.hard_limit;x->limits=l;}}
extern "C" void sleela_mm_set_flow_model(sleela_mm*x,sleela_mm_flow_model f){if(x){std::lock_guard<std::mutex>g(x->m);x->flow=f;}}
extern "C" void sleela_mm_set_thresholds(sleela_mm*x,sleela_mm_thresholds t){if(x){std::lock_guard<std::mutex>g(x->m);x->thresholds=t;}}
extern "C" sleela_mm_stats sleela_mm_stats_get(const sleela_mm*x){sleela_mm_stats s{};if(!x)return s;std::lock_guard<std::mutex>g(x->m);s.resident=x->resident;s.allocated=x->allocated;s.freed=x->freed;s.allocations=x->allocations;s.frees=x->frees;s.peak=x->peak;s.pressure=x->limits.hard_limit?double(x->resident)/double(x->limits.hard_limit):0.0;s.voltage=s.pressure*x->flow.voltage_per_byte;s.current=double(x->allocations-x->frees)*x->flow.current_per_allocation;s.flow=(s.voltage+s.current)*x->flow.flow_gain;s.condition=x->condition;if(s.condition!=SLEELA_MM_CONDITION_CORRUPT)s.condition=s.pressure>=x->thresholds.pressure_critical?SLEELA_MM_CONDITION_LIMIT:(s.pressure>=x->thresholds.pressure_warning?SLEELA_MM_CONDITION_PRESSURE:SLEELA_MM_CONDITION_NORMAL);return s;}
extern "C" uint64_t sleela_mm_condition(const sleela_mm*x){return sleela_mm_stats_get(x).condition;}
extern "C" int sleela_mm_validate(const sleela_mm*x){if(!x)return 0;std::lock_guard<std::mutex>g(x->m);for(const auto&kv:x->blocks)if(!valid_block(kv.first)||kv.second.user!=kv.first)return 0;return 1;}
