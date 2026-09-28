#pragma once
#include "Service.hpp"
#include "Router.hpp"
#include "Listener.hpp"
namespace sleela::fundamental { class Server:public Service { Listener listener_;Router router_; public: Server(std::string,Listener); Router& router()noexcept{return router_;} const Listener& listener()const noexcept{return listener_;} bool start()override; void stop()noexcept override; }; }