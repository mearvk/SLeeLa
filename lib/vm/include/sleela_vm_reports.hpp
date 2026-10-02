#ifndef SLEELA_VM_REPORTS_HPP
#define SLEELA_VM_REPORTS_HPP
#include "sleela_vm_reports.h"
namespace sleela { namespace vm { class ReportsManager { public: explicit ReportsManager(const sleela_vm_reports_plan_t& p):plan_(p){} bool validate()const; bool streamAllowed(unsigned)const; bool recordValid(const sleela_vm_report_record_t&)const; private: sleela_vm_reports_plan_t plan_; }; }}
#endif
