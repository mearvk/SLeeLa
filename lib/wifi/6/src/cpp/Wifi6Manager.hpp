#ifndef SLEELA_WIFI_6_NATIVE_MANAGER_HPP
#define SLEELA_WIFI_6_NATIVE_MANAGER_HPP
#include "../../../../../wifi-common/include/sleela_wifi/wifi_manager_cpp.hpp"

namespace sleela::wifi::v6 {
using Manager = ::sleela::wifi::Manager;
using Network = ::sleela::wifi::Network;
using Credentials = ::sleela::wifi::Credentials;
const char *generation() noexcept;
}
#endif
