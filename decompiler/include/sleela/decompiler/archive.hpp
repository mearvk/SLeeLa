#pragma once
#include "decompiler.hpp"
#include <span>
namespace sleela::decompiler {
void analyze_archive(std::span<const std::uint8_t> bytes, NativeInterfaces& interfaces);
}
