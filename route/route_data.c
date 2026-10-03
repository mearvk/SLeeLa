#include "route_data.h"
#include <string.h>
typedef struct { int gen; const char *name; const char *path; } vm_entry;
static const vm_entry VMS[] = {
 {0,"Core","/impl"},{1,"Foundation","/1"},{2,"Operator","/2"},
 {3,"Specialist","/3"},{4,"Supervisor","/4"},{5,"Manager","/5"},
 {6,"Director","/6"},{7,"Administrator","/7"},{8,"Executive","/8"},
 {9,"Authority","/9"},{10,"Principal","/10"},{11,"Sovereign","/11"}
};
const char *sleela_route_data_vm_name(int g){return g>=0&&g<=11?VMS[g].name:0;}
const char *sleela_route_data_vm_path(int g){return g>=0&&g<=11?VMS[g].path:0;}
int sleela_route_data_required_vm(const char *p){
 if(!p)return -1;
 if(!strncmp(p,"HTTP/",5)){int g=p[5]-'0';if(g>=1&&g<=9)return g;}
 return 0;
}
int sleela_route_data_for_protocol(const char *p,sleela_route_data *o){
 int g=sleela_route_data_required_vm(p);
 if(g<0||!o||!sleela_route_data_vm_name(g))return -1;
 o->vm_generation=g;o->vm_name=VMS[g].name;o->vm_path=VMS[g].path;o->protocol=p;
 o->server_surface=(g==0?"server-edition":p);return 0;
}
int sleela_route_data_validate(const sleela_route_data *d){
 return d&&d->vm_generation>=0&&d->vm_generation<=11&&d->vm_name&&d->vm_path&&d->protocol&&d->server_surface?0:-1;
}
