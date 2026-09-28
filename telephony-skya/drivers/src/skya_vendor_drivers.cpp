#include "../include/skya_vendor_drivers.h"

#include <cstring>

namespace {

bool vendor_is(const skya_driver_device *d, const char *name) {
    return d && d->vendor && std::strcmp(d->vendor, name) == 0;
}

int common_usb_audio_caps(const skya_driver_device *d,
                          skya_driver_capabilities *out) {
    if (!d || !out) return -1;
    *out = {};
    out->audio_input = 1;
    out->audio_output = 1;
    out->mute = 1;
    out->volume = 1;
    out->firmware_query = 1;

    if (d->transport == SKYA_TRANSPORT_USB_HID) {
        out->call_answer = 1;
        out->call_end = 1;
        out->call_hold = 1;
    } else if (d->transport == SKYA_TRANSPORT_SIP ||
               d->transport == SKYA_TRANSPORT_NETWORK) {
        if (d->type == SKYA_DRIVER_VOIP_PHONE) {
            out->call_answer = 1;
            out->call_end = 1;
            out->call_hold = 1;
            out->dialpad = 1;
            out->display = 1;
        }
    }
    return 0;
}

#define DEFINE_VENDOR_DRIVER(symbol, public_name, vendor_name)     static int symbol##_probe(const skya_driver_device *d) {         return vendor_is(d, vendor_name) ? 1 : 0;     }     static int symbol##_caps(const skya_driver_device *d,                              skya_driver_capabilities *out) {         return common_usb_audio_caps(d, out);     }     static const skya_phone_driver symbol##_driver = {         public_name, vendor_name, symbol##_probe, symbol##_caps     };

DEFINE_VENDOR_DRIVER(yealink, "yealink", "Yealink")
DEFINE_VENDOR_DRIVER(poly, "poly", "Poly")
DEFINE_VENDOR_DRIVER(jabra, "jabra", "Jabra")
DEFINE_VENDOR_DRIVER(grandstream, "grandstream", "Grandstream")
DEFINE_VENDOR_DRIVER(epos, "epos", "EPOS")
DEFINE_VENDOR_DRIVER(logitech, "logitech", "Logitech")
DEFINE_VENDOR_DRIVER(fanvil, "fanvil", "Fanvil")
DEFINE_VENDOR_DRIVER(snom, "snom", "Snom")
DEFINE_VENDOR_DRIVER(cisco, "cisco", "Cisco")

} // namespace

extern "C" const skya_phone_driver *skya_yealink_driver(void) { return &yealink_driver; }
extern "C" const skya_phone_driver *skya_poly_driver(void) { return &poly_driver; }
extern "C" const skya_phone_driver *skya_jabra_driver(void) { return &jabra_driver; }
extern "C" const skya_phone_driver *skya_grandstream_driver(void) { return &grandstream_driver; }
extern "C" const skya_phone_driver *skya_epos_driver(void) { return &epos_driver; }
extern "C" const skya_phone_driver *skya_logitech_driver(void) { return &logitech_driver; }
extern "C" const skya_phone_driver *skya_fanvil_driver(void) { return &fanvil_driver; }
extern "C" const skya_phone_driver *skya_snom_driver(void) { return &snom_driver; }
extern "C" const skya_phone_driver *skya_cisco_driver(void) { return &cisco_driver; }
