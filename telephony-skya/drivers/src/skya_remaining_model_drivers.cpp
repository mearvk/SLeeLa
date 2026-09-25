#include "../include/skya_remaining_model_drivers.h"
#include <cstring>
namespace {
bool match(const skya_driver_device*d,const char*v,const char*m){return d&&d->vendor&&d->model&&std::strcmp(d->vendor,v)==0&&std::strstr(d->model,m)!=nullptr;}
int usb_caps(const skya_driver_device*d,skya_driver_capabilities*o){if(!d||!o)return -1;*o={};o->audio_input=1;o->audio_output=1;o->mute=1;o->volume=1;o->firmware_query=1;o->headset_port=1; if(d->transport==SKYA_TRANSPORT_USB_HID){o->call_answer=1;o->call_end=1;}return 0;}
int sip_caps(const skya_driver_device*d,skya_driver_capabilities*o){if(!d||!o)return -1;*o={};if(d->transport!=SKYA_TRANSPORT_SIP&&d->transport!=SKYA_TRANSPORT_NETWORK)return 0;o->audio_input=1;o->audio_output=1;o->call_answer=1;o->call_end=1;o->call_hold=1;o->mute=1;o->volume=1;o->dialpad=1;o->display=1;o->firmware_query=1;return 0;}
#define D(S,N,V,M,C) static int S##_p(const skya_driver_device*d){return match(d,V,M)?1:0;} static int S##_c(const skya_driver_device*d,skya_driver_capabilities*o){return C(d,o);} static const skya_phone_driver S##_d={N,V,S##_p,S##_c};
uh42_x
uh44_x
wh64_x
wh68_x
uh46_x
blackwire_3320_x
savi_8200_x
savi_8400_x
voyager_4320_x
voyager_5200_x
biz_1500_x
evolve3_65_x
evolve3_75_x
speak2_55_x
speak2_75_x
guv3005_x
gxp21_x
grp26_x
impact_sc_200_x
impact_1000_x
sdw_5000_x
zone_wireless_2_x
zone_vibe_x
h570e_x
zone_305_x
x5u_v2_x
x6u_v2_x
v63_x
v64_x
v65_x
v66_x
x210i_v2_x
a330m_x
322_x
int c0(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c1(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c2(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c3(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c4(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c5(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c6(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c7(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c8(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c9(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c10(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c11(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c12(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c13(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c14(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c15(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c16(const skya_driver_device*d,skya_driver_capabilities*o){return sip_caps(d,o);}
int c17(const skya_driver_device*d,skya_driver_capabilities*o){return sip_caps(d,o);}
int c18(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c19(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c20(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c21(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c22(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c23(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c24(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c25(const skya_driver_device*d,skya_driver_capabilities*o){return sip_caps(d,o);}
int c26(const skya_driver_device*d,skya_driver_capabilities*o){return sip_caps(d,o);}
int c27(const skya_driver_device*d,skya_driver_capabilities*o){return sip_caps(d,o);}
int c28(const skya_driver_device*d,skya_driver_capabilities*o){return sip_caps(d,o);}
int c29(const skya_driver_device*d,skya_driver_capabilities*o){return sip_caps(d,o);}
int c30(const skya_driver_device*d,skya_driver_capabilities*o){return sip_caps(d,o);}
int c31(const skya_driver_device*d,skya_driver_capabilities*o){return sip_caps(d,o);}
int c32(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
int c33(const skya_driver_device*d,skya_driver_capabilities*o){return usb_caps(d,o);}
D(m0,"yealink-uh42","Yealink","UH42",c0)
D(m1,"yealink-uh44","Yealink","UH44",c1)
D(m2,"yealink-wh64","Yealink","WH64",c2)
D(m3,"yealink-wh68","Yealink","WH68",c3)
D(m4,"yealink-uh46","Yealink","UH46",c4)
D(m5,"poly-blackwire-3320","Poly","Blackwire 3320",c5)
D(m6,"poly-savi-8200","Poly","Savi 8200",c6)
D(m7,"poly-savi-8400","Poly","Savi 8400",c7)
D(m8,"poly-voyager-4320","Poly","Voyager 4320",c8)
D(m9,"poly-voyager-5200","Poly","Voyager 5200",c9)
D(m10,"jabra-biz-1500","Jabra","Biz 1500",c10)
D(m11,"jabra-evolve3-65","Jabra","Evolve3 65",c11)
D(m12,"jabra-evolve3-75","Jabra","Evolve3 75",c12)
D(m13,"jabra-speak2-55","Jabra","SPEAK2 55",c13)
D(m14,"jabra-speak2-75","Jabra","SPEAK2 75",c14)
D(m15,"grandstream-guv3005","Grandstream","GUV3005",c15)
D(m16,"grandstream-gxp21xx","Grandstream","GXP21",c16)
D(m17,"grandstream-grp26xx","Grandstream","GRP26",c17)
D(m18,"epos-impact-sc200","EPOS","IMPACT SC 200",c18)
D(m19,"epos-impact-1000","EPOS","IMPACT 1000",c19)
D(m20,"epos-sdw-5000","EPOS","SDW 5000",c20)
D(m21,"logitech-zone-wireless-2","Logitech","Zone Wireless 2",c21)
D(m22,"logitech-zone-vibe","Logitech","Zone Vibe",c22)
D(m23,"logitech-h570e","Logitech","H570e",c23)
D(m24,"logitech-zone-305","Logitech","Zone 305",c24)
D(m25,"fanvil-x5u-v2","Fanvil","X5U-V2",c25)
D(m26,"fanvil-x6u-v2","Fanvil","X6U-V2",c26)
D(m27,"fanvil-v63","Fanvil","V63",c27)
D(m28,"fanvil-v64","Fanvil","V64",c28)
D(m29,"fanvil-v65","Fanvil","V65",c29)
D(m30,"fanvil-v66","Fanvil","V66",c30)
D(m31,"fanvil-x210i-v2","Fanvil","X210i-V2",c31)
D(m32,"snom-a330m","Snom","A330M",c32)
D(m33,"cisco-322","Cisco","322",c33)
}
extern "C" const skya_phone_driver *skya_yealink_uh42_driver(){return &m0_d;}
extern "C" const skya_phone_driver *skya_yealink_uh44_driver(){return &m1_d;}
extern "C" const skya_phone_driver *skya_yealink_wh64_driver(){return &m2_d;}
extern "C" const skya_phone_driver *skya_yealink_wh68_driver(){return &m3_d;}
extern "C" const skya_phone_driver *skya_yealink_uh46_driver(){return &m4_d;}
extern "C" const skya_phone_driver *skya_poly_blackwire_3320_driver(){return &m5_d;}
extern "C" const skya_phone_driver *skya_poly_savi_8200_driver(){return &m6_d;}
extern "C" const skya_phone_driver *skya_poly_savi_8400_driver(){return &m7_d;}
extern "C" const skya_phone_driver *skya_poly_voyager_4320_driver(){return &m8_d;}
extern "C" const skya_phone_driver *skya_poly_voyager_5200_driver(){return &m9_d;}
extern "C" const skya_phone_driver *skya_jabra_biz1500_driver(){return &m10_d;}
extern "C" const skya_phone_driver *skya_jabra_evolve3_65_driver(){return &m11_d;}
extern "C" const skya_phone_driver *skya_jabra_evolve3_75_driver(){return &m12_d;}
extern "C" const skya_phone_driver *skya_jabra_speak2_55_driver(){return &m13_d;}
extern "C" const skya_phone_driver *skya_jabra_speak2_75_driver(){return &m14_d;}
extern "C" const skya_phone_driver *skya_grandstream_guv3005_driver(){return &m15_d;}
extern "C" const skya_phone_driver *skya_grandstream_gxp21xx_driver(){return &m16_d;}
extern "C" const skya_phone_driver *skya_grandstream_grp26xx_driver(){return &m17_d;}
extern "C" const skya_phone_driver *skya_epos_impact_sc200_driver(){return &m18_d;}
extern "C" const skya_phone_driver *skya_epos_impact_1000_driver(){return &m19_d;}
extern "C" const skya_phone_driver *skya_epos_sdw5000_driver(){return &m20_d;}
extern "C" const skya_phone_driver *skya_logitech_zone_wireless2_driver(){return &m21_d;}
extern "C" const skya_phone_driver *skya_logitech_zone_vibe_driver(){return &m22_d;}
extern "C" const skya_phone_driver *skya_logitech_h570e_driver(){return &m23_d;}
extern "C" const skya_phone_driver *skya_logitech_zone305_driver(){return &m24_d;}
extern "C" const skya_phone_driver *skya_fanvil_x5uv2_driver(){return &m25_d;}
extern "C" const skya_phone_driver *skya_fanvil_x6uv2_driver(){return &m26_d;}
extern "C" const skya_phone_driver *skya_fanvil_v63_driver(){return &m27_d;}
extern "C" const skya_phone_driver *skya_fanvil_v64_driver(){return &m28_d;}
extern "C" const skya_phone_driver *skya_fanvil_v65_driver(){return &m29_d;}
extern "C" const skya_phone_driver *skya_fanvil_v66_driver(){return &m30_d;}
extern "C" const skya_phone_driver *skya_fanvil_x210iv2_driver(){return &m31_d;}
extern "C" const skya_phone_driver *skya_snom_a330m_driver(){return &m32_d;}
extern "C" const skya_phone_driver *skya_cisco_322_driver(){return &m33_d;}
