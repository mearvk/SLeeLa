#include "skya_linux_driver.h"
#include "../../src/skya_driver_registry.h"
#include <dirent.h>
#include <cstring>
#include <string>
static void cp(char*d,size_t n,const std::string&s){if(d&&n){std::strncpy(d,s.c_str(),n-1);d[n-1]=0;}}
int skya_linux_enumerate(skya_platform_device*d,size_t cap){if(!d||!cap)return-1;DIR*p=opendir("/proc/asound");if(!p)return 0;size_t n=0;while(dirent*e=readdir(p)){if(n>=cap)break;if(e->d_name[0]=='.'||!std::strcmp(e->d_name,"cards"))continue;auto&x=d[n++];std::memset(&x,0,sizeof(x));cp(x.device_id,sizeof(x.device_id),std::string("/proc/asound/")+e->d_name);cp(x.model,sizeof(x.model),e->d_name);cp(x.family,sizeof(x.family),"ALSA");x.type=SKYA_DRIVER_USB_HEADSET;x.transport=SKYA_TRANSPORT_USB_AUDIO;x.present=1;x.input_channels=1;x.output_channels=2;}closedir(p);return(int)n;}
int skya_linux_load(const skya_platform_device*d,const skya_phone_driver**o,skya_driver_capabilities*c){if(!d||!o||!c)return-1;skya_driver_device x{d->vendor[0]?d->vendor:nullptr,d->model,d->family,d->firmware[0]?d->firmware:nullptr,d->type,d->transport};*o=skya_driver_find(&x);if(!*o)return-2;return(*o)->capabilities(&x,c);}
int skya_platform_enumerate(skya_platform_device*d,size_t c){return skya_linux_enumerate(d,c);}
int skya_platform_load(const skya_platform_device*d,const skya_phone_driver**o,skya_driver_capabilities*c){return skya_linux_load(d,o,c);}
