#include "slvm_capability.hpp"
#include <unordered_map>
namespace sleela::vm {
class capability_broker {
 std::unordered_map<capability_id_t, capability> table_;
 public:
 bool grant(capability c){ return c.id != 0 && table_.emplace(c.id,c).second; }
 bool revoke(capability_id_t id){ return table_.erase(id)==1; }
 const capability* get(capability_id_t id) const { auto i=table_.find(id); return i==table_.end()?nullptr:&i->second; }
 bool permits(capability_id_t id,std::uint64_t rights) const { const auto* c=get(id); return c && (c->rights & rights)==rights; }
};
}
