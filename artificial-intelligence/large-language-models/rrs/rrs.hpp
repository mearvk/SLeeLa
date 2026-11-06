#ifndef SLEELA_AI_LLM_RRS_HPP
#define SLEELA_AI_LLM_RRS_HPP
#include "rrs.h"
#include <stdexcept>
#include <string>
#include <vector>
namespace sleela::ai {

// A premise entering the RRS feed-forward.
struct RrsPremise {
 std::string text;
 sllmrrs_signal signal = SLLMRRS_AFFIRM;
 bool faith_or_trial = false;     // RRS subtracts faith: such a premise is eliminated
 bool responsibility_met = true;  // stage 2 prelude-to-continue

 sllmrrs_premise to_c() const {
  sllmrrs_premise p;
  p.text = text.c_str();
  p.signal = signal;
  p.faith_or_trial = faith_or_trial ? 1 : 0;
  p.responsibility_met = responsibility_met ? 1 : 0;
  return p;
 }
};

// RRS — Reasonable Responsibility Solidity, a 3-stage congruent feed-forward.
// Stage 1 conjunctive elimination, stage 2 responsibility prelude, stage 3
// solidity/firmament culmination to a graded potestas verdict. Faith/trial is
// subtracted. No weights.
class Rrs {
public:
 struct Outcome {
  sllmrrs_result raw{};
  bool solid() const { return raw.solid != 0; }
  sllmrrs_potestas potestas() const { return raw.potestas; }
  std::string potestas_name() const { return sllmrrs_potestas_name(raw.potestas); }
  std::uint32_t total_power() const { return raw.total_power; }
  std::uint32_t faith_eliminated() const { return raw.faith_eliminated; }
 };

 // Run the three-model feed-forward. Stage 3 may be empty (its culmination
 // reads the accumulated power forward).
 static Outcome feed_forward(const std::vector<RrsPremise>& stage1,
                             const std::vector<RrsPremise>& stage2,
                             const std::vector<RrsPremise>& stage3 = {}) {
  auto conv = [](const std::vector<RrsPremise>& v){
   std::vector<sllmrrs_premise> o; o.reserve(v.size());
   for (const auto& p : v) o.push_back(p.to_c());
   return o;
  };
  std::vector<sllmrrs_premise> a = conv(stage1), b = conv(stage2), c = conv(stage3);
  Outcome out;
  sllmrrs_status st = sllmrrs_feed_forward(
   a.empty()?nullptr:a.data(), a.size(),
   b.empty()?nullptr:b.data(), b.size(),
   c.empty()?nullptr:c.data(), c.size(),
   &out.raw);
  if (st != SLLMRRS_OK)
   throw std::runtime_error(std::string("RRS feed-forward failed: ") + sllmrrs_status_string(st));
  return out;
 }
};

} // namespace sleela::ai
#endif
