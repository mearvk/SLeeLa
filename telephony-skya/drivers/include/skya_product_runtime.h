#ifndef SKYA_PRODUCT_RUNTIME_H
#define SKYA_PRODUCT_RUNTIME_H
#include "skya_phone_driver.h"
#include "skya_hardware_profile.h"
#include "skya_data_plane.h"
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SKYA_PRODUCT_OP_ANSWER=1,SKYA_PRODUCT_OP_END,SKYA_PRODUCT_OP_HOLD,SKYA_PRODUCT_OP_MUTE,SKYA_PRODUCT_OP_VOLUME,SKYA_PRODUCT_OP_DIAL,SKYA_PRODUCT_OP_BUSYLIGHT,SKYA_PRODUCT_OP_FIRMWARE_QUERY,SKYA_PRODUCT_OP_DESCRIPTOR,SKYA_PRODUCT_OP_RESET } skya_product_operation;
typedef enum { SKYA_HOST_LINUX=1,SKYA_HOST_WINDOWS_10=2,SKYA_HOST_MACOS=3 } skya_product_host;
typedef struct { int (*control)(void*,skya_product_operation,int32_t); int (*query_firmware)(void*,char*,size_t); int (*read_descriptor)(void*,uint8_t*,size_t,size_t*); int (*reset)(void*); int (*hotplug)(void*,int); void *ctx; } skya_product_transport;
typedef struct { skya_driver_device device; skya_driver_capabilities capabilities; skya_product_transport transport; skya_data_plane *data_plane; skya_product_host host; int present,muted,volume,call_active,on_hold,busylight; uint64_t operations; } skya_product_runtime;
int skya_product_runtime_init(skya_product_runtime*,const skya_driver_device*,skya_product_host,const skya_product_transport*);
int skya_product_runtime_control(skya_product_runtime*,skya_product_operation,int32_t);
int skya_product_runtime_firmware(skya_product_runtime*,char*,size_t);
int skya_product_runtime_descriptor(skya_product_runtime*,uint8_t*,size_t,size_t*);
int skya_product_runtime_hotplug(skya_product_runtime*,int);
int skya_product_runtime_reset(skya_product_runtime*);
int skya_product_probe(const skya_driver_device*,const char*,const char*);
int skya_product_capabilities(const skya_driver_device*,const char*,const char*,skya_driver_capabilities*);
#ifdef __cplusplus
}
#endif
#endif
