#ifndef SKYA_EPOS_IMPACT_1000_DRIVER_H
#define SKYA_EPOS_IMPACT_1000_DRIVER_H
#include "../../../include/skya_phone_driver.h"
#include "../../../include/skya_hardware_profile.h"
#include "../../../include/skya_data_plane.h"
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SKYA_EPOS_CTL_ANSWER=1,SKYA_EPOS_CTL_END,SKYA_EPOS_CTL_HOLD,SKYA_EPOS_CTL_MUTE,SKYA_EPOS_CTL_VOLUME,SKYA_EPOS_CTL_BUSYLIGHT } skya_epos_control;
typedef struct { int (*send_control)(void*,skya_epos_control,int32_t); int (*query_firmware)(void*,char*,size_t); int (*read_descriptor)(void*,uint8_t*,size_t,size_t*); void *ctx; } skya_epos_transport;
typedef struct { skya_driver_device device; skya_driver_capabilities capabilities; skya_hardware_profile hardware; skya_epos_transport transport; skya_data_plane *data_plane; int muted,volume,call_active,on_hold,busylight; } skya_epos_impact_1000;
int skya_epos_impact_1000_init(skya_epos_impact_1000*,const skya_driver_device*,const skya_epos_transport*);
int skya_epos_impact_1000_set(skya_epos_impact_1000*,skya_epos_control,int32_t);
int skya_epos_impact_1000_firmware(skya_epos_impact_1000*,char*,size_t);
int skya_epos_impact_1000_descriptor(skya_epos_impact_1000*,uint8_t*,size_t,size_t*);
const skya_phone_driver *skya_epos_impact_1000_driver(void);
const skya_phone_driver *skya_c_epos_impact_1000_driver(void);
#ifdef __cplusplus
}
#endif
#endif