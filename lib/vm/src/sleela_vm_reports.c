#include "../include/sleela_vm_reports.h"
int sleela_vm_reports_plan_validate(const sleela_vm_reports_plan_t* p){if(!p||!p->system_code||!p->binary_object_code||!p->capability_code)return 0;if(p->messaging_code&&!p->routing_code)return 0;return 1;}
int sleela_vm_reports_stream_allowed(const sleela_vm_reports_plan_t* p,uint32_t s){if(!sleela_vm_reports_plan_validate(p))return 0;return s==1?p->input_code!=0:s==2?p->output_code!=0:s==3?p->message_code!=0:s==4?p->system_code!=0:0;}
int sleela_vm_reports_record_valid(const sleela_vm_report_record_t* r){return r&&r->system_code&&r->stream_code&&r->object_name_code&&r->sequence&&r->payload;}
