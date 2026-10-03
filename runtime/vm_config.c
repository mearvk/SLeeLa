#include "vm_config.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char *trim(char *s){while(*s&&isspace((unsigned char)*s))++s;char*e=s+strlen(s);while(e>s&&isspace((unsigned char)e[-1]))--e;*e='\0';return s;}
static int ok(int v){return v>=SLEELA_VM_MIN_VERSION&&v<=SLEELA_VM_MAX_VERSION;}
void sleela_vm_config_defaults(SLEELA_VM_CONFIG*c){if(!c)return;c->version=11;c->strict=1;strncpy(c->profile,"user",15);c->profile[15]='\0';}
int sleela_vm_config_set_version(SLEELA_VM_CONFIG*c,int v){if(!c||!ok(v))return -1;c->version=v;return 0;}
int sleela_vm_config_load(const char*path,SLEELA_VM_CONFIG*c){if(!path||!c)return -1;FILE*f=fopen(path,"rb");if(!f)return -2;char line[256];while(fgets(line,sizeof(line),f)){char*p=trim(line);if(!*p||*p=='#')continue;char*eq=strchr(p,'=');if(!eq){fclose(f);return -3;}*eq='\0';char*k=trim(p),*v=trim(eq+1);if(!strcmp(k,"vm.version")){char*e=0;long n=strtol(v,&e,10);if(!ok((int)n)||*trim(e)){fclose(f);return -4;}c->version=(int)n;}else if(!strcmp(k,"vm.strict")){if(!strcmp(v,"true")||!strcmp(v,"1"))c->strict=1;else if(!strcmp(v,"false")||!strcmp(v,"0"))c->strict=0;else{fclose(f);return -5;}}else if(!strcmp(k,"vm.profile")){if(strcmp(v,"user")&&strcmp(v,"administrator")&&strcmp(v,"developer")){fclose(f);return -6;}strncpy(c->profile,v,15);c->profile[15]='\0';}}fclose(f);return sleela_vm_config_validate(c);}
int sleela_vm_config_apply_environment(SLEELA_VM_CONFIG*c){if(!c)return -1;const char*v=getenv("SLEELA_VM_VERSION");if(!v||!*v)return 0;char*e=0;long n=strtol(v,&e,10);if(!ok((int)n)||*e)return -2;c->version=(int)n;return 0;}
int sleela_vm_config_validate(const SLEELA_VM_CONFIG*c){if(!c||!ok(c->version))return -1;if(strcmp(c->profile,"user")&&strcmp(c->profile,"administrator")&&strcmp(c->profile,"developer"))return -2;return(c->strict==0||c->strict==1)?0:-3;}
