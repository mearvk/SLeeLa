#include <stdio.h>
#include "sleela_platform.h"
#include "sleela_system_monitor.h"
int main(void){
    SLPlatformInfo p; SLSystemMonitor m; SLHSMStats s; char report[1024];
    if(slplatform_detect(&p)!=0||!slplatform_is_supported_version(&p))return 1;
    slhsm_init(&m);
    if(slhsm_check_capability(&m,SL_CAP_MEMORY)!=SL_HSM_OK)return 2;
    if(slhsm_unknown_operation(&m,"custom.os.operation")<SL_HSM_REVIEW)return 3;
    if(slhsm_native_escape(&m,"native.provider")<SL_HSM_REVIEW)return 4;
    slhsm_stats(&m,&s);
    if(s.unknown_operations!=1||s.native_escapes!=1)return 5;
    if(slplatform_format_report(report,sizeof(report))<=0)return 6;
    if(slhsm_format_report(&m,report,sizeof(report))<=0)return 7;
    printf("platform=%s version=%s\n",p.name,p.version);
    printf("system-monitor: PASS\n");
    return 0;
}