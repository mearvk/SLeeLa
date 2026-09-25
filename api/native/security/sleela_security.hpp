#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::security {
void secureZero(void* data,std::size_t size) noexcept;
std::vector<std::uint8_t> sha256(const std::vector<std::uint8_t>& data);
bool constantTimeEqual(const std::vector<std::uint8_t>& a,const std::vector<std::uint8_t>& b) noexcept;
struct Credential { std::string id; std::vector<std::uint8_t> secret; void clear() noexcept; };
}