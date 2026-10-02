#ifndef SLEELA_VM_REPORTS_H
#define SLEELA_VM_REPORTS_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint32_t input_code, output_code, message_code, system_code; uint32_t binary_object_code, iq_code, routing_code, messaging_code; uint32_t capability_code, audit_code; } sleela_vm_reports_plan_t;
typedef struct { uint32_t system_code, stream_code, object_name_code, destination_code; uint64_t sequence; const void* payload; uint64_t payload_size; } sleela_vm_report_record_t;
int sleela_vm_reports_plan_validate(const sleela_vm_reports_plan_t*);
int sleela_vm_reports_stream_allowed(const sleela_vm_reports_plan_t*, uint32_t);
int sleela_vm_reports_record_valid(const sleela_vm_report_record_t*);
#ifdef __cplusplus
}
#endif
#endif
