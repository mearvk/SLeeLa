#pragma once
#include <string>
#include <vector>
namespace sleela::fundamental { class DnsResolver { public: std::vector<std::string> resolve(const std::string&)const; }; }