#pragma once
#include "slir.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace sleela::decompiler {

struct VmConfig {
    std::size_t memory_size = 1024 * 1024;
    std::size_t max_steps = 1'000'000;
    bool deterministic = true;
};

struct VmTrap {
    enum class Code { None, StepLimit, MemoryFault, DivisionByZero, InvalidOperation };
    Code code{Code::None};
    std::string message;
};

class Vm {
public:
    explicit Vm(VmConfig config = {});
    void reset();
    void load(const Module&);
    bool run();
    std::uint64_t register_value(std::size_t index) const;
    const std::vector<std::uint8_t>& memory() const noexcept;
    const VmTrap& trap() const noexcept;
    std::size_t steps() const noexcept;
private:
    VmConfig config_;
    std::array<std::uint64_t, 32> registers_{};
    std::vector<std::uint8_t> memory_;
    const Module* module_{};
    VmTrap trap_{};
    std::size_t steps_{};
};

} // namespace sleela::decompiler
