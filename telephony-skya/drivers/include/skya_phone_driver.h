#ifndef SKYA_PHONE_DRIVER_H
#define SKYA_PHONE_DRIVER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SKYA_DRIVER_UNKNOWN = 0,
    SKYA_DRIVER_VOIP_PHONE,
    SKYA_DRIVER_USB_PHONE,
    SKYA_DRIVER_USB_HEADSET,
    SKYA_DRIVER_BLUETOOTH_HEADSET,
    SKYA_DRIVER_SPEAKERPHONE
} skya_driver_device_type;

typedef enum {
    SKYA_TRANSPORT_UNKNOWN = 0,
    SKYA_TRANSPORT_SIP,
    SKYA_TRANSPORT_USB_AUDIO,
    SKYA_TRANSPORT_USB_HID,
    SKYA_TRANSPORT_BLUETOOTH,
    SKYA_TRANSPORT_NETWORK
} skya_driver_transport;

typedef struct {
    const char *vendor;
    const char *model;
    const char *family;
    const char *firmware;
    skya_driver_device_type type;
    skya_driver_transport transport;
} skya_driver_device;

typedef struct {
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
} skya_driver_capabilities;

typedef struct {
    const char *name;
    const char *vendor;
    int (*probe)(const skya_driver_device *device);
    int (*capabilities)(const skya_driver_device *device, skya_driver_capabilities *out);
} skya_phone_driver;

#ifdef __cplusplus
}
#endif

#endif
