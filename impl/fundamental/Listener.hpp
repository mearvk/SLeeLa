#pragma once
#include "NetworkEndpoint.hpp"
namespace sleela::fundamental { class Listener { NetworkEndpoint endpoint_;bool listening_{false}; public: explicit Listener(NetworkEndpoint e={}):endpoint_(std::move(e)){} const NetworkEndpoint& endpoint()const noexcept{return endpoint_;} bool listening()const noexcept{return listening_;} void set_listening(bool)noexcept; }; }