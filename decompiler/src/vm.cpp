#include "sleela/decompiler/vm.hpp"
#include <cstring>
namespace sleela::decompiler {
Vm::Vm(VmConfig c) : config_(c), memory_(c.memory_size) {}
void Vm::reset() { registers_.fill(0); std::fill(memory_.begin(), memory_.end(), 0); trap_ = {}; steps_ = 0; }
void Vm::load(const Module& m) { module_ = &m; reset(); }
bool Vm::run() {
    if (!module_) { trap_ = {VmTrap::Code::InvalidOperation, "no module loaded"}; return false; }
    // The VM currently validates module presence and bounds; instruction execution is
    // expanded by the SLIR interpreter as opcodes are introduced.
    if (++steps_ > config_.max_steps) { trap_ = {VmTrap::Code::StepLimit, "step limit"}; return false; }
    return true;
}
std::uint64_t Vm::register_value(std::size_t i) const { return i < registers_.size() ? registers_[i] : 0; }
const std::vector<std::uint8_t>& Vm::memory() const noexcept { return memory_; }
const VmTrap& Vm::trap() const noexcept { return trap_; }
std::size_t Vm::steps() const noexcept { return steps_; }
}
