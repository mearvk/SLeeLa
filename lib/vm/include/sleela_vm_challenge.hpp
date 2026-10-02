#ifndef SLEELA_VM_CHALLENGE_HPP
#define SLEELA_VM_CHALLENGE_HPP
#include "sleela_vm_challenge.h"
namespace sleela { namespace vm { class ChallengeManager { public: explicit ChallengeManager(const sleela_vm_challenge_plan_t& p):plan_(p){} bool validate()const; bool targetAllowed(unsigned)const; bool conditionMatches(const sleela_vm_condition_observed_t&,unsigned)const; private: sleela_vm_challenge_plan_t plan_; }; }}
#endif
