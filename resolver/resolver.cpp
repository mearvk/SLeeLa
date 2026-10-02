#include "resolver.h"
#include <string>
namespace sleela::resolver {
bool event(const std::string& protocol, const std::string& target = {}) {
    return sleela_resolver_event_policy(protocol.c_str(), target.empty() ? nullptr : target.c_str()) != 0;
}
}
