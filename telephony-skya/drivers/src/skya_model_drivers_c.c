#include "../include/skya_model_drivers_c.h"
#include <string.h>

#define C_USB(name, label, vendor, model) \
static int name##_probe(const skya_driver_device *d){return d&&d->vendor&&d->model&&strcmp(d->vendor,vendor)==0&&strstr(d->model,model)!=0;} \
static int name##_caps(const skya_driver_device *d,skya_driver_capabilities *o){if(!d||!o)return -1;memset(o,0,sizeof(*o));o->audio_input=1;o->audio_output=1;o->mute=1;o->volume=1;o->firmware_query=1;o->headset_port=1;if(d->transport==SKYA_TRANSPORT_USB_HID){o->call_answer=1;o->call_end=1;}return 0;} \
static const skya_phone_driver name##_driver={label,vendor,name##_probe,name##_caps}; \
const skya_phone_driver *skya_c_##name##_driver(void){return &name##_driver;}

#define C_SIP(name, label, vendor, model) \
static int name##_probe(const skya_driver_device *d){return d&&d->vendor&&d->model&&strcmp(d->vendor,vendor)==0&&strstr(d->model,model)!=0;} \
static int name##_caps(const skya_driver_device *d,skya_driver_capabilities *o){if(!d||!o)return -1;memset(o,0,sizeof(*o));if(d->transport!=SKYA_TRANSPORT_SIP&&d->transport!=SKYA_TRANSPORT_NETWORK)return 0;o->audio_input=1;o->audio_output=1;o->call_answer=1;o->call_end=1;o->call_hold=1;o->mute=1;o->volume=1;o->dialpad=1;o->display=1;o->firmware_query=1;return 0;} \
static const skya_phone_driver name##_driver={label,vendor,name##_probe,name##_caps}; \
const skya_phone_driver *skya_c_##name##_driver(void){return &name##_driver;}

C_USB(yealink_uh42,"c-yealink-uh42","Yealink","UH42")
C_USB(yealink_uh44,"c-yealink-uh44","Yealink","UH44")
C_USB(yealink_wh64,"c-yealink-wh64","Yealink","WH64")
C_USB(yealink_wh68,"c-yealink-wh68","Yealink","WH68")
C_USB(yealink_uh46,"c-yealink-uh46","Yealink","UH46")
C_USB(poly_blackwire_3320,"c-poly-blackwire-3320","Poly","Blackwire 3320")
C_USB(poly_savi_8200,"c-poly-savi-8200","Poly","Savi 8200")
C_USB(poly_savi_8400,"c-poly-savi-8400","Poly","Savi 8400")
C_USB(poly_voyager_4320,"c-poly-voyager-4320","Poly","Voyager 4320")
C_USB(poly_voyager_5200,"c-poly-voyager-5200","Poly","Voyager 5200")
C_USB(jabra_biz1500,"c-jabra-biz-1500","Jabra","Biz 1500")
C_USB(jabra_evolve3_65,"c-jabra-evolve3-65","Jabra","Evolve3 65")
C_USB(jabra_evolve3_75,"c-jabra-evolve3-75","Jabra","Evolve3 75")
C_USB(jabra_speak2_55,"c-jabra-speak2-55","Jabra","SPEAK2 55")
C_USB(jabra_speak2_75,"c-jabra-speak2-75","Jabra","SPEAK2 75")
C_USB(grandstream_guv3005,"c-grandstream-guv3005","Grandstream","GUV3005")
C_SIP(grandstream_gxp21xx,"c-grandstream-gxp21xx","Grandstream","GXP21")
C_SIP(grandstream_grp26xx,"c-grandstream-grp26xx","Grandstream","GRP26")
C_USB(epos_impact_sc200,"c-epos-impact-sc200","EPOS","IMPACT SC 200")
C_USB(epos_impact_1000,"c-epos-impact-1000","EPOS","IMPACT 1000")
C_USB(epos_sdw5000,"c-epos-sdw-5000","EPOS","SDW 5000")
C_USB(logitech_zone_wireless2,"c-logitech-zone-wireless-2","Logitech","Zone Wireless 2")
C_USB(logitech_zone_vibe,"c-logitech-zone-vibe","Logitech","Zone Vibe")
C_USB(logitech_h570e,"c-logitech-h570e","Logitech","H570e")
C_USB(logitech_zone305,"c-logitech-zone-305","Logitech","Zone 305")
C_SIP(fanvil_x5uv2,"c-fanvil-x5u-v2","Fanvil","X5U-V2")
C_SIP(fanvil_x6uv2,"c-fanvil-x6u-v2","Fanvil","X6U-V2")
C_SIP(fanvil_v63,"c-fanvil-v63","Fanvil","V63")
C_SIP(fanvil_v64,"c-fanvil-v64","Fanvil","V64")
C_SIP(fanvil_v65,"c-fanvil-v65","Fanvil","V65")
C_SIP(fanvil_v66,"c-fanvil-v66","Fanvil","V66")
C_SIP(fanvil_x210iv2,"c-fanvil-x210i-v2","Fanvil","X210i-V2")
C_USB(snom_a330m,"c-snom-a330m","Snom","A330M")
C_USB(cisco_322,"c-cisco-322","Cisco","322")

C_USB(yealink_mp45,"c-yealink-mp45","Yealink","MP45")
C_USB(yealink_mp50,"c-yealink-mp50","Yealink","MP50")
C_USB(poly_blackwire_5220,"c-poly-blackwire-5220","Poly","Blackwire 5220")
C_USB(jabra_evolve2_40,"c-jabra-evolve2-40","Jabra","Evolve2 40")
C_USB(grandstream_guv3000,"c-grandstream-guv3000","Grandstream","GUV3000")
C_USB(epos_impact_sc600,"c-epos-impact-sc600","EPOS","IMPACT SC 600")
C_USB(logitech_zone_wired2,"c-logitech-zone-wired-2","Logitech","Zone Wired 2")
C_SIP(fanvil_x4uv2,"c-fanvil-x4u-v2","Fanvil","X4U-V2")
C_USB(snom_a330d,"c-snom-a330d","Snom","A330D")
C_USB(cisco_321,"c-cisco-321","Cisco","321")
