#include "../include/sleela_vm_reports.hpp"
namespace sleela { namespace vm { bool ReportsManager::validate()const{return sleela_vm_reports_plan_validate(&plan_)!=0;} bool ReportsManager::streamAllowed(unsigned s)const{return sleela_vm_reports_stream_allowed(&plan_,s)!=0;} bool ReportsManager::recordValid(const sleela_vm_report_record_t& r)const{return sleela_vm_reports_record_valid(&r)!=0;} }}
