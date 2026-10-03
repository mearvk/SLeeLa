#ifndef SLEELA_VM_GC_HPP
#define SLEELA_VM_GC_HPP
#include <cstddef>
#include <cstdint>
extern "C" {
#include "../../../runtime/garbage_collector.h"
}
namespace sleela::vm {
class GarbageCollector {
public:
    explicit GarbageCollector(std::size_t threshold = 1024u * 1024u) { gc_init(&gc_, threshold); }
    ~GarbageCollector() { gc_free(&gc_); }
    GarbageCollector(const GarbageCollector&) = delete;
    GarbageCollector& operator=(const GarbageCollector&) = delete;
    void safepoint(std::size_t budget = 32) { gc_safepoint(&gc_, budget); }
    std::size_t collectYoung() { return gc_collect_young(&gc_); }
    std::size_t collectFull() { return gc_collect_full(&gc_); }
    std::size_t liveObjects() const { return gc_live_objects(&gc_); }
    std::size_t bytes() const { return gc_bytes(&gc_); }
    std::size_t promoted() const { return gc_promoted_objects(&gc_); }
    ::GarbageCollector* native() { return &gc_; }
private:
    ::GarbageCollector gc_{};
};
}
#endif
