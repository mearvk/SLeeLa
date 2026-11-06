#ifndef SLEELA_AI_LLM_REASONING_HPP
#define SLEELA_AI_LLM_REASONING_HPP
#include "large_language_models_reasoning.h"
#include "large_language_models.hpp"
#include "large_language_models_capacity.h"
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>
namespace sleela::ai {

// A single reasoning step, copied out of the C result so it owns its text.
struct ReasoningStep {
 std::uint32_t index = 0, depth = 0;
 sllmr_step_kind kind = SLLMR_STEP_DEDUCTION;
 std::string text;
 sllmr_verdict verdict = SLLMR_VERDICT_UNKNOWN;
 double confidence = 0.0;
};

struct ReasoningOutcome {
 std::string answer;
 std::string summary;
 std::vector<ReasoningStep> steps;
 double confidence = 0.0;
 bool verified = false;
 bool tentative = false;
 std::uint32_t backtracks = 0;
};

struct ReasoningOptions {
 sllmr_subject subject = SLLMR_SUBJECT_GENERAL;
 std::uint32_t max_steps = 12;
 std::uint32_t max_backtracks = 3;
 std::uint32_t reflection_every = 3;
 double min_confidence = 0.55;
 bool require_verified = false;
 std::uint64_t seed = 0;

 // Scale the step/backtrack/reflection budget to a user-declared model-capacity
 // budget (from a CapacityProfile, 2..16 GB envelope). Leaves subject and the
 // honesty knobs (min_confidence, require_verified) alone. Returns *this for
 // chaining; the user may still override any field afterwards.
 ReasoningOptions& apply(const sllmc_budget& b) {
  max_steps = b.max_steps;
  max_backtracks = b.max_backtracks;
  reflection_every = b.reflection_every;
  return *this;
 }
};

// Optional verifier: a callback that judges a candidate step. Returning
// SLLMR_VERDICT_UNKNOWN is always honest when you cannot decide.
using StepVerifier = std::function<sllmr_verdict(
 sllmr_subject subject,
 const std::string& question,
 const std::vector<ReasoningStep>& prior,
 const ReasoningStep& candidate,
 std::string& note_out)>;

// A neuro-symbolic reasoning engine over a loaded LanguageModel. If a verifier
// is supplied it provides soundness; otherwise the engine still reflects and
// backtracks on self-critique but reports lower confidence.
class ReasoningModel {
public:
 // The LanguageModel must outlive this object and be loaded.
 explicit ReasoningModel(LanguageModel& llm, StepVerifier verifier = nullptr);
 ~ReasoningModel();
 ReasoningModel(const ReasoningModel&) = delete;
 ReasoningModel& operator=(const ReasoningModel&) = delete;
 ReasoningModel(ReasoningModel&&) noexcept;
 ReasoningModel& operator=(ReasoningModel&&) noexcept;

 // Observe steps as they form; return false to cancel.
 using StepObserver = std::function<bool(const ReasoningStep&)>;

 ReasoningOutcome reason(const std::string& question,
                         const ReasoningOptions& options,
                         StepObserver on_step = nullptr);

 std::string verifier_name() const;

private:
 struct Impl;
 Impl* impl_ = nullptr;
};

} // namespace sleela::ai
#endif
