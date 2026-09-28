#pragma once
#include "Event.hpp"
#include <functional>
#include <unordered_map>
#include <vector>
namespace sleela::fundamental { class EventBus { std::unordered_map<std::string,std::vector<std::function<void(const Event&)>>> h_; public: void subscribe(const std::string&,std::function<void(const Event&)>); void publish(const Event&)const; }; }