#ifndef SKYA_VENDOR_DRIVERS_H
#define SKYA_VENDOR_DRIVERS_H

#include "skya_phone_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

const skya_phone_driver *skya_yealink_driver(void);
const skya_phone_driver *skya_poly_driver(void);
const skya_phone_driver *skya_jabra_driver(void);
const skya_phone_driver *skya_grandstream_driver(void);
const skya_phone_driver *skya_epos_driver(void);
const skya_phone_driver *skya_logitech_driver(void);
const skya_phone_driver *skya_fanvil_driver(void);
const skya_phone_driver *skya_snom_driver(void);
const skya_phone_driver *skya_cisco_driver(void);

#ifdef __cplusplus
}
#endif

#endif
