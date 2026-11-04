#include "osls.h"
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
#  include <io.h>
#  include <sys/stat.h>
#else
#  include <dirent.h>
#  include <unistd.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <fcntl.h>
#  include <limits.h>
#  include <pwd.h>
#  include <ifaddrs.h>
#  include <arpa/inet.h>
#  include <net/if.h>
#  if defined(__linux__)
#    include <sys/sysinfo.h>
#    include <sys/utsname.h>
#  elif defined(__APPLE__)
#    include <sys/sysctl.h>
#    include <mach-o/dyld.h>
#  endif
#endif
static int osls_copy(char*d,size_t c,const char*s){size_t n;if(!d||!c||!s)return 0;n=strlen(s);if(n>=c)return 0;memcpy(d,s,n+1);return 1;}
static uint64_t self_pid(void){
#if defined(_WIN32)
return (uint64_t)_getpid();
#else
return (uint64_t)getpid();
#endif
}
static osls_status fill_self(osls_process_info*o){char cwd[OSLS_PATH_MAX];if(!o)return OSLS_INVALID_ARGUMENT;memset(o,0,sizeof(*o));o->pid=self_pid();
#if defined(_WIN32)
{DWORD n=(DWORD)sizeof(o->user_name);if(!_getcwd(cwd,(int)sizeof(cwd))||!osls_copy(o->current_directory,sizeof(o->current_directory),cwd))return OSLS_SYSTEM_ERROR;if(!GetUserNameA(o->user_name,&n))o->user_name[0]=0;if(!GetModuleFileNameA(NULL,o->executable_path,(DWORD)sizeof(o->executable_path)))return OSLS_SYSTEM_ERROR;osls_copy(o->command_line,sizeof(o->command_line),GetCommandLineA());osls_copy(o->state,sizeof(o->state),"running");}
#else
if(!getcwd(cwd,sizeof(cwd))||!osls_copy(o->current_directory,sizeof(o->current_directory),cwd))return OSLS_SYSTEM_ERROR;{struct passwd*pw=getpwuid(geteuid());if(pw&&pw->pw_name)osls_copy(o->user_name,sizeof(o->user_name),pw->pw_name);}osls_copy(o->state,sizeof(o->state),"running");
# if defined(__linux__)
{char p[64];ssize_t n;FILE*f;char line[256];snprintf(p,sizeof(p),"/proc/%llu/exe",(unsigned long long)o->pid);n=readlink(p,o->executable_path,sizeof(o->executable_path)-1);if(n>=0)o->executable_path[n]=0;snprintf(p,sizeof(p),"/proc/%llu/cmdline",(unsigned long long)o->pid);{int fd=open(p,O_RDONLY);if(fd>=0){n=read(fd,o->command_line,sizeof(o->command_line)-1);close(fd);if(n>0){ssize_t i;for(i=0;i<n;++i)if(!o->command_line[i])o->command_line[i]=' ';while(n>0&&o->command_line[n-1]==' ')--n;o->command_line[n]=0;}}}snprintf(p,sizeof(p),"/proc/%llu/status",(unsigned long long)o->pid);f=fopen(p,"r");if(f){while(fgets(line,sizeof(line),f)){unsigned long long v;if(sscanf(line,"Threads: %llu",&v)==1)o->thread_count=v;if(sscanf(line,"PPid: %llu",&v)==1)o->parent_pid=v;}fclose(f);}}
# elif defined(__APPLE__)
{uint32_t n=(uint32_t)sizeof(o->executable_path);if(_NSGetExecutablePath(o->executable_path,&n)!=0)o->executable_path[0]=0;}
# endif
#endif
o->start_epoch_seconds=(int64_t)time(NULL);return OSLS_OK;}
osls_status osls_get_process_info(uint64_t pid,osls_process_info*out){if(!out)return OSLS_INVALID_ARGUMENT;if(pid&&pid!=self_pid())return OSLS_UNSUPPORTED;return fill_self(out);}
osls_status osls_enumerate_processes(osls_process_info*b,size_t cap,size_t*out){if(!out)return OSLS_INVALID_ARGUMENT;*out=1;if(!b||cap<1)return OSLS_BUFFER_TOO_SMALL;return fill_self(b);}
osls_status osls_get_environment_value(const char*n,char*b,size_t cap){const char*v;if(!n||!*n||!b||!cap)return OSLS_INVALID_ARGUMENT;v=getenv(n);if(!v)return OSLS_NOT_FOUND;return osls_copy(b,cap,v)?OSLS_OK:OSLS_BUFFER_TOO_SMALL;}
osls_status osls_enumerate_environment(osls_environment_entry*b,size_t cap,size_t*out){
#if defined(_WIN32)
LPCH block,item;size_t count=0;if(!out)return OSLS_INVALID_ARGUMENT;block=GetEnvironmentStringsA();if(!block)return OSLS_SYSTEM_ERROR;for(item=block;*item;item+=strlen(item)+1)++count;*out=count;if(cap<count||(count&&!b)){FreeEnvironmentStringsA(block);return OSLS_BUFFER_TOO_SMALL;}count=0;for(item=block;*item;item+=strlen(item)+1){char*eq=strchr(item,'=');size_t k;if(!eq||item[0]=='=')continue;k=(size_t)(eq-item);if(k>=sizeof(b[count].name)||strlen(eq+1)>=sizeof(b[count].value)){FreeEnvironmentStringsA(block);return OSLS_BUFFER_TOO_SMALL;}memcpy(b[count].name,item,k);b[count].name[k]=0;osls_copy(b[count].value,sizeof(b[count].value),eq+1);++count;}*out=count;FreeEnvironmentStringsA(block);return OSLS_OK;
#else
extern char **environ;size_t n=0,i;if(!out)return OSLS_INVALID_ARGUMENT;while(environ[n])++n;*out=n;if(cap<n||(n&&!b))return OSLS_BUFFER_TOO_SMALL;for(i=0;i<n;++i){char*eq=strchr(environ[i],'=');size_t k;if(!eq)continue;k=(size_t)(eq-environ[i]);if(k>=sizeof(b[i].name)||strlen(eq+1)>=sizeof(b[i].value))return OSLS_BUFFER_TOO_SMALL;memcpy(b[i].name,environ[i],k);b[i].name[k]=0;osls_copy(b[i].value,sizeof(b[i].value),eq+1);}return OSLS_OK;
#endif
}
osls_status osls_get_file_info(const char*p,osls_file_info*o){
#if defined(_WIN32)
struct _stat64 st;
#else
struct stat st;
#endif
if(!p||!*p||!o)return OSLS_INVALID_ARGUMENT;memset(o,0,sizeof(*o));if(!osls_copy(o->path,sizeof(o->path),p))return OSLS_BUFFER_TOO_SMALL;
#if defined(_WIN32)
if(_stat64(p,&st)!=0)return errno==ENOENT?OSLS_NOT_FOUND:OSLS_SYSTEM_ERROR;o->is_directory=(st.st_mode&_S_IFDIR)!=0;o->is_regular_file=(st.st_mode&_S_IFREG)!=0;o->size_bytes=(uint64_t)st.st_size;o->modified_epoch_seconds=st.st_mtime;o->created_epoch_seconds=st.st_ctime;o->is_readable=_access(p,4)==0;o->is_writable=_access(p,2)==0;o->is_executable=0;
#else
if(stat(p,&st)!=0)return errno==ENOENT?OSLS_NOT_FOUND:OSLS_SYSTEM_ERROR;o->is_directory=S_ISDIR(st.st_mode)!=0;o->is_regular_file=S_ISREG(st.st_mode)!=0;o->size_bytes=(uint64_t)st.st_size;o->modified_epoch_seconds=st.st_mtime;o->created_epoch_seconds=st.st_ctime;o->is_readable=access(p,R_OK)==0;o->is_writable=access(p,W_OK)==0;o->is_executable=access(p,X_OK)==0;
#endif
return OSLS_OK;}
osls_status osls_list_directory(const char*p,char*b,size_t cap,size_t*out){size_t used=0;if(!p||!*p||!out||(!b&&cap))return OSLS_INVALID_ARGUMENT;
#if defined(_WIN32)
{WIN32_FIND_DATAA d;char pattern[OSLS_PATH_MAX];HANDLE h;size_t n=strlen(p);if(n+3>=sizeof(pattern))return OSLS_BUFFER_TOO_SMALL;memcpy(pattern,p,n);if(n&&p[n-1]!='\\'&&p[n-1]!='/')pattern[n++]='\\';memcpy(pattern+n,"*",2);h=FindFirstFileA(pattern,&d);if(h==INVALID_HANDLE_VALUE)return OSLS_NOT_FOUND;do{size_t z=strlen(d.cFileName)+1;if(used+z>cap){FindClose(h);*out=used;return OSLS_BUFFER_TOO_SMALL;}memcpy(b+used,d.cFileName,z);used+=z;}while(FindNextFileA(h,&d));FindClose(h);}
#else
{DIR*d=opendir(p);struct dirent*e;if(!d)return errno==ENOENT?OSLS_NOT_FOUND:OSLS_SYSTEM_ERROR;while((e=readdir(d))!=NULL){size_t z=strlen(e->d_name)+1;if(used+z>cap){closedir(d);*out=used;return OSLS_BUFFER_TOO_SMALL;}memcpy(b+used,e->d_name,z);used+=z;}closedir(d);}
#endif
*out=used;return OSLS_OK;}
osls_status osls_get_cpu_info(osls_cpu_info*o){if(!o)return OSLS_INVALID_ARGUMENT;memset(o,0,sizeof(*o));
#if defined(_WIN32)
{SYSTEM_INFO s;MEMORYSTATUSEX m;GetSystemInfo(&s);o->logical_cores=s.dwNumberOfProcessors;o->physical_cores=o->logical_cores;m.dwLength=sizeof(m);if(GlobalMemoryStatusEx(&m))o->total_memory_bytes=m.ullTotalPhys;osls_copy(o->vendor,sizeof(o->vendor),"unknown");osls_copy(o->model,sizeof(o->model),"unknown");}
#elif defined(__linux__)
{long n=sysconf(_SC_NPROCESSORS_ONLN);struct sysinfo s;struct utsname u;o->logical_cores=n>0?(uint32_t)n:0;o->physical_cores=o->logical_cores;if(sysinfo(&s)==0)o->total_memory_bytes=(uint64_t)s.totalram*s.mem_unit;if(uname(&u)==0)osls_copy(o->model,sizeof(o->model),u.machine);osls_copy(o->vendor,sizeof(o->vendor),"unknown");}
#elif defined(__APPLE__)
{int n=0;uint64_t m=0;size_t z=sizeof(n);if(sysctlbyname("hw.logicalcpu",&n,&z,NULL,0)==0&&n>0)o->logical_cores=n;z=sizeof(n);if(sysctlbyname("hw.physicalcpu",&n,&z,NULL,0)==0&&n>0)o->physical_cores=n;z=sizeof(m);if(sysctlbyname("hw.memsize",&m,&z,NULL,0)==0)o->total_memory_bytes=m;z=sizeof(o->model);if(sysctlbyname("machdep.cpu.brand_string",o->model,&z,NULL,0)!=0)osls_copy(o->model,sizeof(o->model),"unknown");osls_copy(o->vendor,sizeof(o->vendor),"unknown");}
#else
return OSLS_UNSUPPORTED;
#endif
return OSLS_OK;}
osls_status osls_get_timezone_info(osls_timezone_info*o){time_t now;struct tm lt;if(!o)return OSLS_INVALID_ARGUMENT;memset(o,0,sizeof(*o));now=time(NULL);
#if defined(_WIN32)
if(localtime_s(&lt,&now)!=0)return OSLS_SYSTEM_ERROR;o->offset_minutes_from_utc=(int)(-_get_timezone/60);osls_copy(o->id,sizeof(o->id),"local");
#else
if(!localtime_r(&now,&lt))return OSLS_SYSTEM_ERROR;
# if defined(__APPLE__) || defined(__linux__)
o->offset_minutes_from_utc=(int)(lt.tm_gmtoff/60);
# else
return OSLS_UNSUPPORTED;
# endif
{const char*tz=getenv("TZ");osls_copy(o->id,sizeof(o->id),tz&&*tz?tz:"local");}
#endif
#if defined(__APPLE__) || defined(__linux__)
osls_copy(o->abbreviation,sizeof(o->abbreviation),lt.tm_zone?lt.tm_zone:"local");
#else
osls_copy(o->abbreviation,sizeof(o->abbreviation),"local");
#endif
o->is_dst=lt.tm_isdst>0;return OSLS_OK;}
osls_status osls_get_network_interfaces(osls_network_interface*b,size_t cap,size_t*out){
#if defined(_WIN32)
(void)b;(void)cap;if(!out)return OSLS_INVALID_ARGUMENT;*out=0;return OSLS_UNSUPPORTED;
#else
struct ifaddrs*l,*i;size_t n=0;if(!out)return OSLS_INVALID_ARGUMENT;if(getifaddrs(&l)!=0)return OSLS_SYSTEM_ERROR;for(i=l;i;i=i->ifa_next)if(i->ifa_name&&i->ifa_addr&&(i->ifa_addr->sa_family==AF_INET||i->ifa_addr->sa_family==AF_INET6))++n;*out=n;if(n>cap||(n&&!b)){freeifaddrs(l);return OSLS_BUFFER_TOO_SMALL;}if(n)memset(b,0,n*sizeof(*b));n=0;for(i=l;i;i=i->ifa_next){char addr[INET6_ADDRSTRLEN];osls_network_interface*e;if(!i->ifa_name||!i->ifa_addr||(i->ifa_addr->sa_family!=AF_INET&&i->ifa_addr->sa_family!=AF_INET6))continue;e=&b[n++];osls_copy(e->name,sizeof(e->name),i->ifa_name);e->is_up=(i->ifa_flags&IFF_UP)!=0;e->is_loopback=(i->ifa_flags&IFF_LOOPBACK)!=0;if(inet_ntop(i->ifa_addr->sa_family,i->ifa_addr->sa_family==AF_INET?(void*)&((struct sockaddr_in*)i->ifa_addr)->sin_addr:(void*)&((struct sockaddr_in6*)i->ifa_addr)->sin6_addr,addr,sizeof(addr))){if(i->ifa_addr->sa_family==AF_INET)osls_copy(e->ipv4,sizeof(e->ipv4),addr);else osls_copy(e->ipv6,sizeof(e->ipv6),addr);}}freeifaddrs(l);return OSLS_OK;
#endif
}
