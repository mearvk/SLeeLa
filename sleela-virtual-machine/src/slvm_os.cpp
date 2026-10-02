#include "slvm_os.hpp"
namespace sleela::vm { os_result unsupported(const char* operation){ return {os_status::unsupported,0,operation?operation:""}; } }
