#pragma once
#include "Transport.hpp"
#include "NetworkEndpoint.hpp"
namespace sleela::fundamental { class Connection { Transport*t_;NetworkEndpoint endpoint_; public: Connection(Transport*t=nullptr,NetworkEndpoint e={}):t_(t),endpoint_(std::move(e)){} bool open(); void close()noexcept; bool active()const noexcept; const NetworkEndpoint& endpoint()const noexcept{return endpoint_;} }; }