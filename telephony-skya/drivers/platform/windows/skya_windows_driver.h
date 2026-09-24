#ifndef SKYA_WINDOWS_DRIVER_H
#define SKYA_WINDOWS_DRIVER_H
#include "../../include/skya_platform_driver.h"
#ifdef __cplusplus
extern "C" {
#endif
int skya_windows_enumerate(skya_platform_device*, size_t);
int skya_windows_load(const skya_platform_device*, const skya_phone_driver**, skya_driver_capabilities*);
#ifdef __cplusplus
}
#endif
#endif
