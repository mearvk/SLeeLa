#include "../include/skya_phone_driver.h"
#include "../include/skya_vendor_drivers.h"
#include "../src/skya_driver_registry.h"

static int standard_audio_probe(const skya_driver_device *d) {
    return d && (d->transport == SKYA_TRANSPORT_USB_AUDIO ||
                 d->transport == SKYA_TRANSPORT_BLUETOOTH);
}

static int standard_audio_caps(const skya_driver_device *d,
                               skya_driver_capabilities *c) {
    if (!d || !c) return -1;
    *c = {};
    c->audio_input = 1;
    c->audio_output = 1;
    c->mute = 1;
    c->volume = 1;
    return 0;
}

static const skya_phone_driver standard_audio_driver = {
    "standard-audio", "Standard", standard_audio_probe, standard_audio_caps
};

extern "C" int skya_register_builtin_drivers(void) {
    const skya_phone_driver *drivers[] = {
        skya_yealink_driver(), skya_poly_driver(), skya_jabra_driver(),
        skya_grandstream_driver(), skya_epos_driver(), skya_logitech_driver(),
        skya_fanvil_driver(), skya_snom_driver(), skya_cisco_driver(),
        &standard_audio_driver
    };

    int registered = 0;
    for (const skya_phone_driver *driver : drivers) {
        if (driver && skya_driver_register(driver) == 0) ++registered;
    }
    return registered;
}
