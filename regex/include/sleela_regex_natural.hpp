#pragma once
#include <string>
#include <string_view>
namespace sleela::regex::natural { enum class Status{ok,null_source,unknown_word,unbalanced,invalid_quantity}; struct Diagnostic{Status status;std::size_t offset;std::string message;}; Status validate(std::string_view,Diagnostic* =nullptr); std::string canonical_symbol(std::string_view); }
