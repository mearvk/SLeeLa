#include "../include/skya_phone_driver.h"
#include "../src/skya_driver_registry.h"

#include <cstring>

static int vendor_probe(const skya_driver_device *d, const char *vendor) {
    return d && d->vendor && vendor && std::strcmp(d->vendor, vendor) == 0;
}

static int audio_phone_caps(const skya_driver_device *d, skya_driver_capabilities *c) {
    if (!d || !c) return -1;
    *c = {};
    c->audio_input = 1;
    c->audio_output = 1;
    c->mute = 1;
    c->volume = 1;
    c->firmware_query = 1;
    return 0;
}

static int standard_audio_probe(const skya_driver_device *d) {
    return d && (d->transport == SKYA_TRANSPORT_USB_AUDIO ||
                 d->transport == SKYA_TRANSPORT_BLUETOOTH);
}
static int standard_audio_caps(const skya_driver_device *d, skya_driver_capabilities *c) {
    if (!d || !c) return -1;
    *c = {};
    c->audio_input = 1;
    c->audio_output = 1;
    c->mute = 1;
    c->volume = 1;
    return 0;
}

static int yealink_probe(const skya_driver_device *d) { return vendor_probe(d, "Yealink"); }
static int poly_probe(const skya_driver_device *d) { return vendor_probe(d, "Poly"); }
static int jabra_probe(const skya_driver_device *d) { return vendor_probe(d, "Jabra"); }
static int grandstream_probe(const skya_driver_device *d) { return vendor_probe(d, "Grandstream"); }
static int epos_probe(const skya_driver_device *d) { return vendor_probe(d, "EPOS"); }
static int logitech_probe(const skya_driver_device *d) { return vendor_probe(d, "Logitech"); }
static int fanvil_probe(const skya_driver_device *d) { return vendor_probe(d, "Fanvil"); }
static int snom_probe(const skya_driver_device *d) { return vendor_probe(d, "Snom"); }
static int cisco_probe(const skya_driver_device *d) { return vendor_probe(d, "Cisco"); }

static const skya_phone_driver drivers[] = {
    {"standard-audio", "Standard", standard_audio_probe, standard_audio_caps},
    {"yealink", "Yealink", yealink_probe, audio_phone_caps},
    {"poly", "Poly", poly_probe, audio_phone_caps},
    {"jabra", "Jabra", jabra_probe, audio_phone_caps},
    {"grandstream", "Grandstream", grandstream_probe, audio_phone_caps},
    {"epos", "EPOS", epos_probe, audio_phone_caps},
    {"logitech", "Logitech", logitech_probe, audio_phone_caps},
    {"fanvil", "Fanvil", fanvil_probe, audio_phone_caps},
    {"snom", "Snom", snom_probe, audio_phone_caps},
    {"cisco", "Cisco", cisco_probe, audio_phone_caps}
};

extern "C" int skya_register_builtin_drivers(void) {
    int registered = 0;
    for (const auto &driver : drivers) {
        if (skya_driver_register(&driver) == 0) ++registered;
    }
    return registered;
}
