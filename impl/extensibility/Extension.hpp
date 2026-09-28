#pragma once
#include <string>
#include "ExtensionPoint.hpp"
namespace sleela::extensibility { class Extension { std::string id_; ExtensionPoint point_; public: Extension(std::string id,ExtensionPoint p):id_(std::move(id)),point_(std::move(p)){} const std::string& id()const noexcept{return id_;} const ExtensionPoint& point()const noexcept{return point_;} }; }