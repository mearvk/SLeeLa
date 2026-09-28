#pragma once
#include <string>
#include <vector>
namespace sleela::fundamental { class Directory { std::string path_; public: explicit Directory(std::string={}); bool exists()const; bool create()const; std::vector<std::string> entries()const; }; }