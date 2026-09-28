#ifndef SKYA_DRIVER_REGISTRY_H
#define SKYA_DRIVER_REGISTRY_H

#include "../include/skya_phone_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

int skya_driver_register(const skya_phone_driver *driver);
size_t skya_driver_count(void);
const skya_phone_driver *skya_driver_at(size_t index);
const skya_phone_driver *skya_driver_find(const skya_driver_device *device);

#ifdef __cplusplus
}
#endif

#endif
