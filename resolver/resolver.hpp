#ifndef SLEELA_RESOLVER_HPP
#define SLEELA_RESOLVER_HPP
#include <string>
namespace sleela::resolver {
bool event(const std::string& protocol, const std::string& target = {});
}
#endif
