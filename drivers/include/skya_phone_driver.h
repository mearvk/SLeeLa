#ifndef SKYA_PHONE_DRIVER_H
#define SKYA_PHONE_DRIVER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum skya_transport {
    SKYA_TRANSPORT_UNKNOWN = 0,
    SKYA_TRANSPORT_USB_HID = 1,
    SKYA_TRANSPORT_SIP = 2,
    SKYA_TRANSPORT_NETWORK = 3,
    SKYA_TRANSPORT_BLUETOOTH = 4,
    SKYA_TRANSPORT_SERIAL = 5
} skya_transport;

typedef struct skya_driver_device {
    const char *vendor;
    const char *model;
    skya_transport transport;
} skya_driver_device;

typedef struct skya_driver_capabilities {
    int audio_input;
    int audio_output;
    int call_answer;
    int call_end;
    int call_hold;
    int mute;
    int volume;
    int dialpad;
    int display;
    int firmware_query;
    int headset_port;
} skya_driver_capabilities;

typedef int (*skya_driver_probe_fn)(const skya_driver_device *);
typedef int (*skya_driver_capabilities_fn)(const skya_driver_device *, skya_driver_capabilities *);

typedef struct skya_phone_driver {
    const char *name;
    const char *vendor;
    skya_driver_probe_fn probe;
    skya_driver_capabilities_fn capabilities;
} skya_phone_driver;

#ifdef __cplusplus
}
#endif
#endif
