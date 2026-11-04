#include "osls.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <process.h>
#  include <direct.h>
#else
#  include <sys/types.h>
#  include <unistd.h>
#  include <limits.h>
#  if defined(__APPLE__)
#    include <mach/mach_time.h>
#  endif
#endif

typedef struct cron_field { uint64_t values; int minimum; int maximum; int wildcard; } cron_field;
static int copy_text(char *d,size_t c,const char *s){size_t n;if(!d||!c||!s)return 0;n=strlen(s);if(n>=c)return 0;memcpy(d,s,n+1);return 1;}
static void copy_env(char*d,size_t c,const char*n){const char*v=getenv(n);if(v)(void)copy_text(d,c,v);}

osls_status osls_get_system_info(osls_system_info *o){if(!o)return OSLS_INVALID_ARGUMENT;memset(o,0,sizeof(*o));
#if defined(_WIN32)
{DWORD hl=(DWORD)sizeof(o->hostname),ul=(DWORD)sizeof(o->user_name);LARGE_INTEGER q,f;char cwd[sizeof(o->working_directory)];if(!GetComputerNameA(o->hostname,&hl))return OSLS_SYSTEM_ERROR;if(!GetUserNameA(o->user_name,&ul))copy_env(o->user_name,sizeof(o->user_name),"USERNAME");if(!_getcwd(cwd,(int)sizeof(cwd))||!copy_text(o->working_directory,sizeof(o->working_directory),cwd))return OSLS_SYSTEM_ERROR;o->platform=OSLS_PLATFORM_WINDOWS;o->process_id=(uint64_t)_getpid();o->unix_time_seconds=(int64_t)time(NULL);if(!QueryPerformanceFrequency(&f)||!QueryPerformanceCounter(&q)||f.QuadPart<=0)return OSLS_SYSTEM_ERROR;o->monotonic_time_nanoseconds=(uint64_t)((q.QuadPart/f.QuadPart)*1000000000ULL+((q.QuadPart%f.QuadPart)*1000000000ULL)/f.QuadPart);}
#elif defined(__APPLE__) || defined(__linux__)
{struct timespec ts;char cwd[sizeof(o->working_directory)];if(gethostname(o->hostname,sizeof(o->hostname)-1)!=0)return OSLS_SYSTEM_ERROR;o->hostname[sizeof(o->hostname)-1]=0;if(!getcwd(cwd,sizeof(cwd))||!copy_text(o->working_directory,sizeof(o->working_directory),cwd))return OSLS_SYSTEM_ERROR;copy_env(o->user_name,sizeof(o->user_name),"USER");if(!o->user_name[0])copy_env(o->user_name,sizeof(o->user_name),"LOGNAME");
# if defined(__APPLE__)
o->platform=OSLS_PLATFORM_MACOS;
# else
o->platform=OSLS_PLATFORM_LINUX;
# endif
o->process_id=(uint64_t)getpid();o->unix_time_seconds=(int64_t)time(NULL);
# if defined(__APPLE__)
{mach_timebase_info_data_t i;uint64_t t=mach_absolute_time();if(mach_timebase_info(&i)!=KERN_SUCCESS||!i.denom)return OSLS_SYSTEM_ERROR;o->monotonic_time_nanoseconds=(t/i.denom)*i.numer+((t%i.denom)*i.numer)/i.denom;}
# else
if(clock_gettime(CLOCK_MONOTONIC,&ts)!=0)return OSLS_SYSTEM_ERROR;o->monotonic_time_nanoseconds=(uint64_t)ts.tv_sec*1000000000ULL+(uint64_t)ts.tv_nsec;
# endif
}
#else
return OSLS_UNSUPPORTED;
#endif
return OSLS_OK;}
static int parse_uint(const char**c,int*out){const char*p=*c;int v=0;if(!isdigit((unsigned char)*p))return 0;while(isdigit((unsigned char)*p)){int d=*p-'0';if(v>(INT_MAX-d)/10)return 0;v=v*10+d;++p;}*c=p;*out=v;return 1;}
static int parse_field(const char*t,int lo,int hi,int sunday,cron_field*out){const char*p=t;int wild,bits_dummy=0;uint64_t bits=0;if(!t||!*t||!out)return 0;wild=t[0]=='*';while(*p){int a,b,step=1,v;if(*p=='*'){a=lo;b=hi;++p;}else{if(!parse_uint(&p,&a))return 0;b=a;if(*p=='-'){++p;if(!parse_uint(&p,&b))return 0;}}if(*p=='/'){++p;if(!parse_uint(&p,&step)||step<=0)return 0;}if(a<lo||b>hi||a>b)return 0;for(v=a;v<=b;v+=step){int x=(sunday&&v==7)?0:v;bits|=(uint64_t)1<<x;if(b-v<step)break;}if(!*p)break;if(*p++!=','||!*p)return 0;}bits_dummy=(bits!=0);if(!bits_dummy)return 0;out->values=bits;out->minimum=lo;out->maximum=hi;out->wildcard=wild;return 1;}
static int parse_expression(const char*e,cron_field f[5]){char b[256],*cur,*tok;int n=0;if(!e||strlen(e)>=sizeof(b))return 0;strcpy(b,e);cur=b;while((tok=strtok(cur," \t\r\n"))!=NULL){static const int lo[5]={0,0,1,1,0},hi[5]={59,23,31,12,7};cur=NULL;if(n>=5||!parse_field(tok,lo[n],hi[n],n==4,&f[n]))return 0;if(n==3&&(f[n].values&1u))return 0;++n;}return n==5;}
osls_status osls_cron_validate(const char*e){cron_field f[5];return parse_expression(e,f)?OSLS_OK:OSLS_INVALID_ARGUMENT;}
static int local_time(time_t v,struct tm*o){
#if defined(_WIN32)
return localtime_s(o,&v)==0;
#else
return localtime_r(&v,o)!=NULL;
#endif
}
static int field_has(const cron_field*f,int v){return v>=0&&v<64&&((f->values>>v)&1u)!=0;}
osls_status osls_cron_next_after(const char*e,int64_t after,int64_t*out){cron_field f[5];int64_t c,i;const int64_t limit=(int64_t)5*366*24*60;if(!out||!parse_expression(e,f))return OSLS_INVALID_ARGUMENT;c=after-(after%60)+60;if(after<0&&after%60)c=after-(after%60);for(i=0;i<limit;++i,c+=60){struct tm tmv;time_t instant=(time_t)c;int dm,wm;if((int64_t)instant!=c||!local_time(instant,&tmv))return OSLS_SYSTEM_ERROR;if(!field_has(&f[0],tmv.tm_min)||!field_has(&f[1],tmv.tm_hour)||!field_has(&f[3],tmv.tm_mon+1))continue;dm=field_has(&f[2],tmv.tm_mday);wm=field_has(&f[4],tmv.tm_wday);if(f[2].wildcard||f[4].wildcard){if(!dm||!wm)continue;}else if(!dm&&!wm)continue;*out=c;return OSLS_OK;}return OSLS_NOT_FOUND;}
