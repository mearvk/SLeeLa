#ifndef SKYA_PLATFORM_DRIVER_H
#define SKYA_PLATFORM_DRIVER_H
#include "skya_phone_driver.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { char vendor[96]; char model[128]; char family[96]; char firmware[96]; char device_id[192]; skya_driver_device_type type; skya_driver_transport transport; unsigned int input_channels; unsigned int output_channels; unsigned int sample_rate; int present; } skya_platform_device;
int skya_platform_enumerate(skya_platform_device*, size_t);
int skya_platform_load(const skya_platform_device*, const skya_phone_driver**, skya_driver_capabilities*);
#ifdef __cplusplus
}
#endif
#endif
