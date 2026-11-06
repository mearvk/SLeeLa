#ifndef SLEELA_AI_LLM_CAPACITY_HPP
#define SLEELA_AI_LLM_CAPACITY_HPP
#include "large_language_models_capacity.h"
#include <cstdint>
#include <string>
namespace sleela::ai {

// A user-owned description of the model the caller intends to run, within the
// supported 2 GB .. 16 GB weight envelope. SLeeLa ships no weights and selects
// no model; this only records the user's choice and derives a reasoning budget.
struct CapacityProfile {
 std::string model_id;        // the caller's identifier for THEIR model
 std::string weight_format;   // informational, e.g. "gguf-q4_k_m"
 std::uint64_t weight_bytes = 4ULL * 1024 * 1024 * 1024; // default STANDARD (4 GB)
 std::uint32_t context_tokens = 0;      // 0 => derive from tier
 std::uint64_t ram_budget_bytes = 0;    // 0 => unspecified
 bool accelerated = false;

 sllmc_tier tier() const { return sllmc_tier_for_bytes(weight_bytes); }
 std::string tier_name() const { return sllmc_tier_name(tier()); }

 // Advisory envelope check: true when weight_bytes is within 2..16 GB (or 0).
 bool within_envelope() const { return to_c_status() == SLLMC_OK; }
 sllmc_status to_c_status() const {
  sllmc_profile p = to_c();
  return sllmc_profile_check(&p);
 }

 sllmc_profile to_c() const {
  sllmc_profile p; sllmc_profile_init(&p);
  p.model_id = model_id.empty() ? nullptr : model_id.c_str();
  p.weight_format = weight_format.empty() ? nullptr : weight_format.c_str();
  p.weight_bytes = weight_bytes;
  p.context_tokens = context_tokens;
  p.ram_budget_bytes = ram_budget_bytes;
  p.accelerated = accelerated ? 1 : 0;
  return p;
 }

 sllmc_budget budget() const {
  sllmc_profile p = to_c();
  sllmc_budget b{};
  sllmc_derive_budget(&p, &b);
  return b;
 }
};

} // namespace sleela::ai
#endif
