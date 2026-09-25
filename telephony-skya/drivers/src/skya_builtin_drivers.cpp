#include "../include/skya_phone_driver.h"
#include "../include/skya_model_drivers.h"
#include "../include/skya_vendor_drivers.h"
#include "../include/skya_remaining_model_drivers.h"
#include "../src/skya_driver_registry.h"

static int standard_audio_probe(const skya_driver_device *d) {
    return d && (d->transport == SKYA_TRANSPORT_USB_AUDIO ||
                 d->transport == SKYA_TRANSPORT_BLUETOOTH);
}
static int standard_audio_caps(const skya_driver_device *d, skya_driver_capabilities *c) {
    if (!d || !c) return -1;
    *c = {}; c->audio_input=1; c->audio_output=1; c->mute=1; c->volume=1;
    return 0;
}
static const skya_phone_driver standard_audio_driver={
    "standard-audio","Standard",standard_audio_probe,standard_audio_caps
};

extern "C" int skya_register_builtin_drivers(void) {
    const skya_phone_driver *drivers[]={
        skya_yealink_uh42_driver(),
        skya_yealink_uh44_driver(),
        skya_yealink_wh64_driver(),
        skya_yealink_wh68_driver(),
        skya_yealink_uh46_driver(),
        skya_poly_blackwire_3320_driver(),
        skya_poly_savi_8200_driver(),
        skya_poly_savi_8400_driver(),
        skya_poly_voyager_4320_driver(),
        skya_poly_voyager_5200_driver(),
        skya_jabra_biz1500_driver(),
        skya_jabra_evolve3_65_driver(),
        skya_jabra_evolve3_75_driver(),
        skya_jabra_speak2_55_driver(),
        skya_jabra_speak2_75_driver(),
        skya_grandstream_guv3005_driver(),
        skya_grandstream_gxp21xx_driver(),
        skya_grandstream_grp26xx_driver(),
        skya_epos_impact_sc200_driver(),
        skya_epos_impact_1000_driver(),
        skya_epos_sdw5000_driver(),
        skya_logitech_zone_wireless2_driver(),
        skya_logitech_zone_vibe_driver(),
        skya_logitech_h570e_driver(),
        skya_logitech_zone305_driver(),
        skya_fanvil_x5uv2_driver(),
        skya_fanvil_x6uv2_driver(),
        skya_fanvil_v63_driver(),
        skya_fanvil_v64_driver(),
        skya_fanvil_v65_driver(),
        skya_fanvil_v66_driver(),
        skya_fanvil_x210iv2_driver(),
        skya_snom_a330m_driver(),
        skya_cisco_322_driver(),
        skya_yealink_mp45_driver(), skya_yealink_mp50_driver(),
        skya_poly_blackwire_5220_driver(), skya_jabra_evolve2_40_driver(),
        skya_grandstream_guv3000_driver(), skya_epos_impact_sc600_driver(),
        skya_logitech_zone_wired2_driver(), skya_fanvil_x4uv2_driver(),
        skya_snom_a330d_driver(), skya_cisco_321_driver(),
        skya_yealink_driver(), skya_poly_driver(), skya_jabra_driver(),
        skya_grandstream_driver(), skya_epos_driver(), skya_logitech_driver(),
        skya_fanvil_driver(), skya_snom_driver(), skya_cisco_driver(),
        &standard_audio_driver
    };
    int registered=0;
    for(const skya_phone_driver *driver:drivers)
        if(driver&&skya_driver_register(driver)==0) ++registered;
    return registered;
}
