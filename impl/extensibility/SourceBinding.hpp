#pragma once
#include <string>
namespace sleela::extensibility { class SourceBinding { std::string route_,source_; public: SourceBinding(std::string r,std::string s):route_(std::move(r)),source_(std::move(s)){} const std::string& route()const noexcept{return route_;} const std::string& source()const noexcept{return source_;} }; }