#ifndef SLEELA_SLVM_CAPABILITY_HPP
#define SLEELA_SLVM_CAPABILITY_HPP
#include <cstdint>
namespace sleela::vm {
using capability_id_t = std::uint64_t;
enum class capability_domain : std::uint32_t { files=1,directories,processes,threads,network,dns,time,ipc,signals,memory,dynamic_libraries,devices,security,system,terminal,random,crypto,gui_media };
struct capability { capability_id_t id{}; capability_domain domain{}; std::uint64_t rights{}; std::uint64_t resource_limit{}; };
}
#endif
