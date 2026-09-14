#define _POSIX_C_SOURCE 200809L
#include "sleela_time.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#endif

static char g_country[3]="";
static char g_timezone[64]="";

#ifdef _WIN32
static int64_t filetime_ms(void){FILETIME ft;ULARGE_INTEGER u;GetSystemTimeAsFileTime(&ft);u.LowPart=ft.dwLowDateTime;u.HighPart=ft.dwHighDateTime;return (int64_t)(u.QuadPart/10000ULL-11644473600000ULL);}
static uint64_t qpc_ns(void){static LARGE_INTEGER f;static int ready;LARGE_INTEGER v;if(!ready){QueryPerformanceFrequency(&f);ready=1;}QueryPerformanceCounter(&v);return (uint64_t)((double)v.QuadPart*1000000000.0/(double)f.QuadPart);}
#else
static int64_t clock_realtime_ms(void){struct timespec ts;if(clock_gettime(CLOCK_REALTIME,&ts)!=0)return -1;return (int64_t)ts.tv_sec*1000+ts.tv_nsec/1000000;}
static uint64_t clock_mono_ns(void){struct timespec ts;if(clock_gettime(CLOCK_MONOTONIC,&ts)!=0)return 0;return (uint64_t)ts.tv_sec*1000000000ULL+(uint64_t)ts.tv_nsec;}
#endif

int sltime_init(void){
#ifdef _WIN32
 WSADATA w;return WSAStartup(MAKEWORD(2,2),&w)==0?0:EIO;
#else
 return 0;
#endif
}
int64_t sltime_utc_nanos(void){
#ifdef _WIN32
 { FILETIME ft; ULARGE_INTEGER u; GetSystemTimeAsFileTime(&ft); u.LowPart=ft.dwLowDateTime; u.HighPart=ft.dwHighDateTime; return (int64_t)(u.QuadPart*100ULL-116444736000000000LL); }
#else
 struct timespec ts;if(clock_gettime(CLOCK_REALTIME,&ts)!=0)return -1;return (int64_t)ts.tv_sec*1000000000LL+ts.tv_nsec;
#endif
}
int64_t sltime_utc_millis(void){
#ifdef _WIN32
 return filetime_ms();
#else
 return clock_realtime_ms();
#endif
}
uint64_t sltime_monotonic_nanos(void){
#ifdef _WIN32
 return qpc_ns();
#else
 return clock_mono_ns();
#endif
}
int sltime_set_location(SLTimeLocationMode mode,const char* country,const char* zone){
 (void)mode;g_country[0]=0;g_timezone[0]=0;
 if(country&&strlen(country)>=2){g_country[0]=country[0];g_country[1]=country[1];}
 if(zone&&*zone){strncpy(g_timezone,zone,sizeof(g_timezone)-1);g_timezone[sizeof(g_timezone)-1]=0;}
 return 0;
}
const char* sltime_location_timezone(void){
 if(g_timezone[0])return g_timezone;
#ifdef _WIN32
 return "LOCAL";
#else
 {const char* z=getenv("TZ");if(z&&*z)return z;}
 return "LOCAL";
#endif
}
const char* sltime_location_country(void){return g_country[0]?g_country:"??";}

int sltime_sample(SLTimeSample* s){
 if(!s)return EINVAL;memset(s,0,sizeof(*s));s->utc_ms=sltime_utc_millis();s->monotonic_ns=sltime_monotonic_nanos();
 s->source=SL_TIME_SOURCE_SYSTEM;s->uncertainty_us=1000;strncpy(s->country,sltime_location_country(),2);
 strncpy(s->timezone,sltime_location_timezone(),sizeof(s->timezone)-1);return s->utc_ms<0?EIO:0;
}
static uint64_t ntp64_to_us(uint32_t sec,uint32_t frac){return (uint64_t)sec*1000000ULL+((uint64_t)frac*1000000ULL>>32);}
static void us_to_ntp64(uint64_t unix_us,uint32_t* sec,uint32_t* frac){*sec=(uint32_t)(unix_us/1000000ULL+2208988800ULL);*frac=(uint32_t)(((unix_us%1000000ULL)<<32)/1000000ULL);}

int sltime_query_ntp(const char* host,uint32_t timeout_ms,SLTimeSample* sample){
 if(!host||!*host||!sample)return EINVAL;
#ifdef _WIN32
 SOCKET s=INVALID_SOCKET;
#else
 int s=-1;
#endif
 struct addrinfo hints,*res=NULL,*p;uint8_t packet[48]={0};int result=EIO;uint64_t t1,t4,t3;uint32_t sec,frac;
 memset(&hints,0,sizeof(hints));hints.ai_family=AF_UNSPEC;hints.ai_socktype=SOCK_DGRAM;hints.ai_protocol=IPPROTO_UDP;
 if(getaddrinfo(host,"123",&hints,&res)!=0)return EHOSTUNREACH;
 packet[0]=0x23;t1=(uint64_t)sltime_utc_millis()*1000ULL;us_to_ntp64(t1,&sec,&frac);
 packet[40]=(uint8_t)(sec>>24);packet[41]=(uint8_t)(sec>>16);packet[42]=(uint8_t)(sec>>8);packet[43]=(uint8_t)sec;
 packet[44]=(uint8_t)(frac>>24);packet[45]=(uint8_t)(frac>>16);packet[46]=(uint8_t)(frac>>8);packet[47]=(uint8_t)frac;
 for(p=res;p;p=p->ai_next){
#ifdef _WIN32
  s=socket(p->ai_family,p->ai_socktype,p->ai_protocol);if(s==INVALID_SOCKET)continue;{DWORD tv=timeout_ms;setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv));}
#else
  s=socket(p->ai_family,p->ai_socktype,p->ai_protocol);if(s<0)continue;{struct timeval tv={(long)(timeout_ms/1000),(long)((timeout_ms%1000)*1000)};setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));}
#endif
  if(sendto(s,(const char*)packet,48,0,p->ai_addr,(int)p->ai_addrlen)>=0){
   int n=recvfrom(s,(char*)packet,48,0,NULL,NULL);t4=(uint64_t)sltime_utc_millis()*1000ULL;
   if(n>=48){uint32_t sec2=((uint32_t)packet[32]<<24)|((uint32_t)packet[33]<<16)|((uint32_t)packet[34]<<8)|packet[35];
    uint32_t frac2=((uint32_t)packet[36]<<24)|((uint32_t)packet[37]<<16)|((uint32_t)packet[38]<<8)|packet[39];
    sec=((uint32_t)packet[40]<<24)|((uint32_t)packet[41]<<16)|((uint32_t)packet[42]<<8)|packet[43];
    frac=((uint32_t)packet[44]<<24)|((uint32_t)packet[45]<<16)|((uint32_t)packet[46]<<8)|packet[47];
    t3=ntp64_to_us(sec,frac);uint64_t t2=ntp64_to_us(sec2,frac2);
    {int64_t theta=((int64_t)t2-(int64_t)t1+(int64_t)t3-(int64_t)t4)/2;
     int64_t delay=(int64_t)(t4-t1)-((int64_t)t3-(int64_t)t2);memset(sample,0,sizeof(*sample));
     sample->utc_ms=(int64_t)(t4/1000ULL)+theta/1000;sample->monotonic_ns=sltime_monotonic_nanos();sample->utc_offset_ms=theta/1000;
     sample->uncertainty_us=(uint64_t)(delay>0?delay/2:0);sample->source=SL_TIME_SOURCE_NTP;sample->stratum=packet[1];
     strncpy(sample->source_host,host,sizeof(sample->source_host)-1);strncpy(sample->country,sltime_location_country(),2);
     strncpy(sample->timezone,sltime_location_timezone(),sizeof(sample->timezone)-1);result=0;}}
  }
#ifdef _WIN32
  closesocket(s);
#else
  close(s);
#endif
  if(result==0)break;
 }
 freeaddrinfo(res);return result;
}
static int utc_tm(int64_t ms,struct tm* t){time_t sec=(time_t)(ms/1000);
#ifdef _WIN32
 return gmtime_s(t,&sec)==0;
#else
 return gmtime_r(&sec,t)!=NULL;
#endif
}
int sltime_format_iso8601(int64_t ms,char* out,size_t n){struct tm t;long m=(long)(ms>=0?ms%1000:(1000+ms%1000)%1000);if(!out||n<32||!utc_tm(ms,&t))return EINVAL;
 return snprintf(out,n,"%04d-%02d-%02dT%02d:%02d:%02d.%03ldZ",t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec,m)<0?EIO:0;}
int sltime_http_date(int64_t ms,char* out,size_t n){static const char* wd[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};static const char* mo[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};struct tm t;
 if(!out||n<30||!utc_tm(ms,&t))return EINVAL;snprintf(out,n,"%s, %02d %s %04d %02d:%02d:%02d GMT",wd[t.tm_wday],t.tm_mday,mo[t.tm_mon],t.tm_year+1900,t.tm_hour,t.tm_min,t.tm_sec);return 0;}
int sltime_json(const SLTimeSample* s,char* out,size_t n){if(!s||!out||n<64)return EINVAL;return snprintf(out,n,"{\"utc_ms\":%lld,\"monotonic_ns\":%llu,\"offset_ms\":%lld,\"uncertainty_us\":%llu,\"source\":%u,\"stratum\":%u,\"country\":\"%s\",\"timezone\":\"%s\",\"source_host\":\"%s\"}",(long long)s->utc_ms,(unsigned long long)s->monotonic_ns,(long long)s->utc_offset_ms,(unsigned long long)s->uncertainty_us,s->source,s->stratum,s->country,s->timezone,s->source_host)<0?EIO:0;}
int sltime_rmi_record(const SLTimeSample* s,char* out,size_t n){return sltime_json(s,out,n);}
size_t sltime_bodi_record(const SLTimeSample* s,uint8_t* out,size_t n){const size_t need=32;if(!s||!out||n<need)return 0;uint64_t u=(uint64_t)s->utc_ms,m=s->monotonic_ns,e=s->uncertainty_us;uint32_t a=s->source,b=s->stratum;int i;for(i=0;i<8;i++)out[i]=(uint8_t)(u>>(56-8*i));for(i=0;i<8;i++)out[8+i]=(uint8_t)(m>>(56-8*i));for(i=0;i<8;i++)out[16+i]=(uint8_t)(e>>(56-8*i));out[24]=a>>24;out[25]=a>>16;out[26]=a>>8;out[27]=a;out[28]=b>>24;out[29]=b>>16;out[30]=b>>8;out[31]=b;return need;}
