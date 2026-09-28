#pragma once
#include <string>
#include <vector>
namespace sleela::debugger::engine {
enum class CapabilityState { Unavailable, Declared, Implemented, Tested };
struct Capability { std::string name; CapabilityState state{CapabilityState::Unavailable}; std::string mechanism; std::string platform; };
struct CapabilityReport { std::vector<Capability> entries; void add(std::string,std::string); void add(const std::string&,CapabilityState,std::string={},std::string={}); bool supports(const std::string&) const; std::string text() const; };
const char* capabilityStateName(CapabilityState) noexcept;
}