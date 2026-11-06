#ifndef SLEELA_AI_LLM_CODE_HPP
#define SLEELA_AI_LLM_CODE_HPP
#include "large_language_models_code.h"
#include <cstdint>
#include <string>
namespace sleela::ai {

// A user-owned Code LLM (coding-assistant / program-aided) profile. SLeeLa ships
// no code model; this records the caller's choice and the tasks/verification it
// supports. Pairs with SLLMR_SUBJECT_CODE in the reasoning layer and with a
// CapacityProfile (2..16 GB) for budget.
struct CodeModelProfile {
 std::string model_id;        // the caller's code model
 std::string base_family;     // informational, e.g. "code-llama", "deepseek-coder"
 sllmcx_language primary_language = SLLMCX_LANG_C;
 std::string other_language;  // used when primary_language == OTHER
 std::uint32_t tasks =
   SLLMCX_TASK_SUMMARIZE | SLLMCX_TASK_DRY_RUN | SLLMCX_TASK_REFACTOR | SLLMCX_TASK_TRANSLATE;
 sllmcx_check_mode check_mode = SLLMCX_CHECK_TRACE;
 bool fill_in_middle = false;

 bool has_task(sllmcx_task t) const { return (tasks & static_cast<std::uint32_t>(t)) != 0; }

 std::string language_label() const {
  sllmcx_profile p = to_c();
  return sllmcx_profile_language_label(&p);
 }

 sllmcx_profile to_c() const {
  sllmcx_profile p; sllmcx_profile_init(&p);
  p.model_id = model_id.empty() ? nullptr : model_id.c_str();
  p.base_family = base_family.empty() ? nullptr : base_family.c_str();
  p.primary_language = primary_language;
  p.other_language = other_language.empty() ? nullptr : other_language.c_str();
  p.tasks = tasks;
  p.check_mode = check_mode;
  p.fill_in_middle = fill_in_middle ? 1 : 0;
  return p;
 }
};

} // namespace sleela::ai
#endif
