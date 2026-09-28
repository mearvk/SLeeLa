#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::security {
void secureZero(void*,std::size_t) noexcept;
std::vector<std::uint8_t> sha256(const std::vector<std::uint8_t>&);
bool constantTimeEqual(const std::vector<std::uint8_t>&,const std::vector<std::uint8_t>&) noexcept;
struct Credential { std::string id; std::vector<std::uint8_t> secret; void clear() noexcept; };
}