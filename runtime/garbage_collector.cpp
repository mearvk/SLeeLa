#include "garbage_collector.hpp"
namespace sleela::runtime {
std::shared_ptr<void> GarbageCollector::allocate(std::size_t b){auto t=std::shared_ptr<std::byte[]>(new std::byte[b?b:1]());std::shared_ptr<void>o(t,static_cast<void*>(t.get()));objects_.push_back(o);bytesAllocated_+=b;return o;}
void GarbageCollector::addRoot(const std::shared_ptr<void>&o){if(o)roots_.push_back(o);}
std::size_t GarbageCollector::collect(){std::vector<std::shared_ptr<void>>k;for(auto&o:objects_){bool r=false;for(auto&x:roots_)if(x.get()==o.get()){r=true;break;}if(r)k.push_back(o);}objects_.swap(k);roots_.clear();++collections_;return 0;}
}