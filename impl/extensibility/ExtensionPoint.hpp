#pragma once
#include <string>
namespace sleela::extensibility { class ExtensionPoint { std::string id_; public: explicit ExtensionPoint(std::string id):id_(std::move(id)){} const std::string& id()const noexcept{return id_;} }; }