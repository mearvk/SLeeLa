#ifndef SLEELA_SECURITY_SUPERVISOR_H
#define SLEELA_SECURITY_SUPERVISOR_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLS_MAX_CLASSES 128
#define SLS_MAX_ROLES 16
#define SLS_MAX_CLASS_NAME 96
typedef enum { SLS_ROLE_USER, SLS_ROLE_BUSINESS, SLS_ROLE_CORPORATION, SLS_ROLE_LLC, SLS_ROLE_FIDUCIARY, SLS_ROLE_CONTRACT, SLS_ROLE_ATTORNEY, SLS_ROLE_SENATOR, SLS_ROLE_DEMOCRAT, SLS_ROLE_CUSTOM } SLSRole;
typedef struct { char name[SLS_MAX_CLASS_NAME]; int allowed; size_t max_instances,max_bytes,instances,bytes; } SLSClassPolicy;
typedef struct { SLSRole role; size_t max_total_instances,max_total_bytes; } SLSRolePolicy;
typedef struct { SLSClassPolicy classes[SLS_MAX_CLASSES]; size_t class_count; SLSRolePolicy roles[SLS_MAX_ROLES]; size_t role_count; SLSRole active_role; size_t total_instances,total_bytes; } SecuritySupervisor;
void security_supervisor_init(SecuritySupervisor*,SLSRole);
int security_supervisor_set_class(SecuritySupervisor*,const char*,int,size_t,size_t);
int security_supervisor_set_role_limit(SecuritySupervisor*,SLSRole,size_t,size_t);
int security_supervisor_authorize_class(const SecuritySupervisor*,const char*);
int security_supervisor_reserve(SecuritySupervisor*,const char*,size_t);
void security_supervisor_release(SecuritySupervisor*,const char*,size_t);
int security_supervisor_set_active_role(SecuritySupervisor*,SLSRole);
#ifdef __cplusplus
}
#endif
#endif