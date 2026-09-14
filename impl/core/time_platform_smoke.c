#include "sleela_time.h"
#include <stdio.h>
#include <string.h>
int main(void){
    SLTimeSample s; char iso[64],http[64],json[2048],rmi[2048]; uint8_t bodi[32];
    if(sltime_init()!=0)return 1;
    if(sltime_sample(&s)!=0||s.utc_ms<=0||s.monotonic_ns==0)return 2;
    if(sltime_format_iso8601(s.utc_ms,iso,sizeof(iso))!=0)return 3;
    if(sltime_http_date(s.utc_ms,http,sizeof(http))!=0)return 4;
    if(sltime_json(&s,json,sizeof(json))!=0)return 5;
    if(sltime_rmi_record(&s,rmi,sizeof(rmi))!=0)return 6;
    if(sltime_bodi_record(&s,bodi,sizeof(bodi))!=32)return 7;
    if(sltime_set_location(SL_TIME_LOCATION_COUNTRY,"DE","Europe/Berlin")!=0)return 8;
    if(strcmp(sltime_location_country(),"DE")!=0)return 9;
    printf("time platform smoke: PASS\n%s\n%s\n",iso,http);
    return 0;
}
