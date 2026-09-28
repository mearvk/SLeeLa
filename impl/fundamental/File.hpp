#pragma once
#include <string>
#include <cstdint>
namespace sleela::fundamental { class File { std::string path_; public: explicit File(std::string={}); bool exists()const; bool write(const std::string&); std::string read()const; std::uintmax_t size()const; const std::string& path()const noexcept; }; }