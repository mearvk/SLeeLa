#pragma once
#include "Transport.hpp"
#include <string>
namespace sleela::fundamental { class SecureChannel { Transport* transport_;std::string peer_; public: explicit SecureChannel(Transport*t=nullptr):transport_(t){} void set_peer_name(std::string); const std::string& peer_name()const noexcept; bool ready()const noexcept; }; }