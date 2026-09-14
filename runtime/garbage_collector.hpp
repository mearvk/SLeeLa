#ifndef SLEELA_GARBAGE_COLLECTOR_HPP
#define SLEELA_GARBAGE_COLLECTOR_HPP
#include <cstddef>
#include <memory>
#include <vector>
namespace sleela::runtime {
class GarbageCollector {
 std::vector<std::shared_ptr<void>> roots_,objects_; std::size_t threshold_,bytesAllocated_=0,collections_=0;
public:
 explicit GarbageCollector(std::size_t t=1024*1024):threshold_(t?t:1024*1024){}
 std::shared_ptr<void> allocate(std::size_t bytes);
 void addRoot(const std::shared_ptr<void>&);
 std::size_t collect();
 std::size_t liveObjects()const{return objects_.size();}
 std::size_t bytes()const{return bytesAllocated_;}
 std::size_t collections()const{return collections_;}
};
}
#endif