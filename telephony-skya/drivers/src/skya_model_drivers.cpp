#include "../include/skya_model_drivers.h"
#include <cstring>

namespace {
bool has(const char *v,const char *n){return v&&n&&std::strstr(v,n)!=nullptr;}
bool model(const skya_driver_device*d,const char*v,const char*m){
 return d&&d->vendor&&d->model&&std::strcmp(d->vendor,v)==0&&has(d->model,m);
}
int usb(const skya_driver_device*d,skya_driver_capabilities*o,bool ctl){
 if(!d||!o)return -1; *o={}; o->audio_input=1;o->audio_output=1;o->mute=1;o->volume=1;o->firmware_query=1;
 if(ctl&&d->transport==SKYA_TRANSPORT_USB_HID){o->call_answer=1;o->call_end=1;}
 return 0;
}
int sip(const skya_driver_device*d,skya_driver_capabilities*o){
 if(!d||!o)return -1; *o={};
 if(d->transport!=SKYA_TRANSPORT_SIP&&d->transport!=SKYA_TRANSPORT_NETWORK)return 0;
 o->audio_input=1;o->audio_output=1;o->call_answer=1;o->call_end=1;o->call_hold=1;
 o->mute=1;o->volume=1;o->dialpad=1;o->display=1;o->firmware_query=1; return 0;
}
#define MD(s,p,v,m,c) static int s##_p(const skya_driver_device*d){return model(d,v,m)?1:0;} static int s##_c(const skya_driver_device*d,skya_driver_capabilities*o){return c(d,o);} static const skya_phone_driver s##_d={p,v,s##_p,s##_c};
int a(const skya_driver_device*d,skya_driver_capabilities*o){return usb(d,o,true);}
int b(const skya_driver_device*d,skya_driver_capabilities*o){return usb(d,o,true);}
int c(const skya_driver_device*d,skya_driver_capabilities*o){return usb(d,o,true);}
int d(const skya_driver_device*d,skya_driver_capabilities*o){return usb(d,o,true);}
int e(const skya_driver_device*d,skya_driver_capabilities*o){return usb(d,o,true);}
int f(const skya_driver_device*d,skya_driver_capabilities*o){return usb(d,o,true);}
int g(const skya_driver_device*d,skya_driver_capabilities*o){return usb(d,o,true);}
int h(const skya_driver_device*d,skya_driver_capabilities*o){return sip(d,o);}
int i(const skya_driver_device*d,skya_driver_capabilities*o){return usb(d,o,true);}
int j(const skya_driver_device*d,skya_driver_capabilities*o){return usb(d,o,true);}
MD(a,"yealink-mp45","Yealink","MP45",a)
MD(b,"yealink-mp50","Yealink","MP50",b)
MD(c,"poly-blackwire-5220","Poly","Blackwire 5220",c)
MD(d,"jabra-evolve2-40","Jabra","Evolve2 40",d)
MD(e,"grandstream-guv3000","Grandstream","GUV3000",e)
MD(f,"epos-impact-sc600","EPOS","IMPACT SC 600",f)
MD(g,"logitech-zone-wired-2","Logitech","Zone Wired 2",g)
MD(h,"fanvil-x4u-v2","Fanvil","X4U-V2",h)
MD(i,"snom-a330d","Snom","A330D",i)
MD(j,"cisco-321","Cisco","321",j)
}
extern "C" const skya_phone_driver *skya_yealink_mp45_driver(){return &a_d;}
extern "C" const skya_phone_driver *skya_yealink_mp50_driver(){return &b_d;}
extern "C" const skya_phone_driver *skya_poly_blackwire_5220_driver(){return &c_d;}
extern "C" const skya_phone_driver *skya_jabra_evolve2_40_driver(){return &d_d;}
extern "C" const skya_phone_driver *skya_grandstream_guv3000_driver(){return &e_d;}
extern "C" const skya_phone_driver *skya_epos_impact_sc600_driver(){return &f_d;}
extern "C" const skya_phone_driver *skya_logitech_zone_wired2_driver(){return &g_d;}
extern "C" const skya_phone_driver *skya_fanvil_x4uv2_driver(){return &h_d;}
extern "C" const skya_phone_driver *skya_snom_a330d_driver(){return &i_d;}
extern "C" const skya_phone_driver *skya_cisco_321_driver(){return &j_d;}
