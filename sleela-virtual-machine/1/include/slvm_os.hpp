#ifndef SLEELA_SLVM_OS_HPP
#define SLEELA_SLVM_OS_HPP
#include "slvm_capability.hpp"
#include <cstdint>
#include <string>
namespace sleela::vm {
enum class os_status : std::int32_t { ok=0, denied=-100, invalid_argument=-101, not_found=-102, resource_exhausted=-103, unsupported=-104, platform_error=-105 };
struct os_result { os_status status{os_status::ok}; std::int32_t native_error{}; std::string operation; };
class os_adapter { public: virtual ~os_adapter()=default; virtual const char* name() const noexcept=0; virtual os_result probe() const=0; };
}
#endif
