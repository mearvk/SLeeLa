#include "preferred_router.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
static void trim(char *s){size_t n;if(!s)return;while(isspace((unsigned char)*s))memmove(s,s+1,strlen(s));n=strlen(s);while(n&&isspace((unsigned char)s[n-1]))s[--n]='\0';}
static int kv(const char *line,const char *key,char *out,size_t cap){size_t k=strlen(key);if(strncmp(line,key,k)!=0||line[k]!='=')return 0;snprintf(out,cap,"%s",line+k+1);trim(out);return 1;}
static int score_for_role(const char *role){if(!strcmp(role,"national_backbone")||!strcmp(role,"national_ixp"))return 5;if(!strcmp(role,"regional_carrier")||!strcmp(role,"international_dc"))return 4;return 1;}
static int route_generation(const char *p){if(!p||strncmp(p,"HTTP/",5))return 0;int g=p[5]-'0';return g>=1&&g<=9?g:0;}
static const char *route_name(int g){static const char*n[]={"Core","Foundation","Operator","Specialist","Supervisor","Manager","Director","Administrator","Executive","Authority","Principal","Sovereign"};return g>=0&&g<=11?n[g]:"Core";}
static const char *route_path(int g){static const char*p[]={"/impl","/1","/2","/3","/4","/5","/6","/7","/8","/9","/10","/11"};return g>=0&&g<=11?p[g]:"/impl";}
int sleela_preferred_router_select(const char *config_path,const char *protocol,sleela_preferred_router *out){
 FILE *f=NULL;char line[512],country[16]="US",role[64]="national_backbone",candidate[256]="";int min_score=4,international=1;
 if(!out)return -1;memset(out,0,sizeof(*out));
 const char *cfg=config_path;if(!cfg||!*cfg)cfg=getenv("SLEELA_PREFERRED_ROUTER_CONFIG");
 if(cfg&&*cfg)f=fopen(cfg,"r");if(f){while(fgets(line,sizeof line,f)){char v[256];trim(line);if(!line[0]||line[0]=='#')continue;if(kv(line,"country",v,sizeof v))snprintf(country,sizeof country,"%s",v);else if(kv(line,"preferred_role",v,sizeof v))snprintf(role,sizeof role,"%s",v);else if(kv(line,"min_score",v,sizeof v))min_score=atoi(v);else if(kv(line,"international",v,sizeof v))international=atoi(v)!=0;else if(kv(line,"candidate",v,sizeof v))snprintf(candidate,sizeof candidate,"%s",v);}fclose(f);}
 if(min_score<1)min_score=1;if(min_score>5)min_score=5;
 if(score_for_role(role)<min_score)snprintf(role,sizeof role,"%s",min_score<=4?"regional_carrier":"national_backbone");
 if(!candidate[0])snprintf(candidate,sizeof candidate,"%s — configured %s anchor",country,role);
 snprintf(out->country,sizeof out->country,"%s",country);snprintf(out->role,sizeof out->role,"%s",role);snprintf(out->candidate,sizeof out->candidate,"%s",candidate);out->routing_preference=score_for_role(role);
 out->vm_generation=route_generation(protocol);snprintf(out->vm_name,sizeof out->vm_name,"%s",route_name(out->vm_generation));snprintf(out->vm_path,sizeof out->vm_path,"%s",route_path(out->vm_generation));snprintf(out->protocol,sizeof out->protocol,"%s",protocol?protocol:"HTTP");snprintf(out->server_surface,sizeof out->server_surface,"%s",protocol?protocol:"server-edition");
 (void)international;return 0;
}
int sleela_preferred_router_startup(const char *config_path,const char *protocol){sleela_preferred_router r;int rc=sleela_preferred_router_select(config_path,protocol,&r);if(rc!=0)return rc;fprintf(stderr,"SLeeLa preferred-router protocol=%s vm=%d/%s route=%s country=%s role=%s preference=%d candidate=%s\n",r.protocol,r.vm_generation,r.vm_name,r.vm_path,r.country,r.role,r.routing_preference,r.candidate);return 0;}
int sleela_preferred_router_packet_policy(const char *protocol,int minimum_preference){sleela_preferred_router r;if(minimum_preference<1)minimum_preference=1;if(minimum_preference>5)minimum_preference=5;return sleela_preferred_router_select(NULL,protocol,&r)==0&&r.routing_preference>=minimum_preference;}
