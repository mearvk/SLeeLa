#pragma once
#include "Transport.hpp"
#include "NetworkEndpoint.hpp"
namespace sleela::fundamental { class Client { Transport*t_; public: explicit Client(Transport*t=nullptr):t_(t){} bool connect(const NetworkEndpoint&); void close()noexcept; bool connected()const noexcept; }; }