#ifndef SKYA_MODEL_DRIVERS_H
#define SKYA_MODEL_DRIVERS_H
#include "skya_phone_driver.h"
#ifdef __cplusplus
extern "C" {
#endif
const skya_phone_driver *skya_yealink_mp45_driver(void);
const skya_phone_driver *skya_yealink_mp50_driver(void);
const skya_phone_driver *skya_poly_blackwire_5220_driver(void);
const skya_phone_driver *skya_jabra_evolve2_40_driver(void);
const skya_phone_driver *skya_grandstream_guv3000_driver(void);
const skya_phone_driver *skya_epos_impact_sc600_driver(void);
const skya_phone_driver *skya_logitech_zone_wired2_driver(void);
const skya_phone_driver *skya_fanvil_x4uv2_driver(void);
const skya_phone_driver *skya_snom_a330d_driver(void);
const skya_phone_driver *skya_cisco_321_driver(void);
#ifdef __cplusplus
}
#endif
#endif
