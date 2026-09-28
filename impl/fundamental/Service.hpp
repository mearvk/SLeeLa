#pragma once
#include <string>
namespace sleela::fundamental { class Service { std::string name_;bool running_{false}; public: explicit Service(std::string={}); virtual ~Service()=default; virtual bool start(); virtual void stop()noexcept; bool running()const noexcept; const std::string& name()const noexcept; }; }