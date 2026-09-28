#pragma once
#include <string>
namespace sleela::fundamental { class Path { std::string value_; public: explicit Path(std::string={}); std::string normalized()const; std::string filename()const; std::string parent()const; std::string extension()const; }; }