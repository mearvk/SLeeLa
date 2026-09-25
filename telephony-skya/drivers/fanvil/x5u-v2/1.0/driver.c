#include "driver.h"
#include "../../../include/skya_product_runtime.h"
static const char *VENDOR="fanvil"; static const char *MODEL="x5u-v2";
static int probe(const skya_driver_device*d){return skya_product_probe(d,VENDOR,MODEL);}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){return skya_product_capabilities(d,VENDOR,MODEL,out);}
static const skya_phone_driver driver={"fanvil_x5u_v2","fanvil",probe,capabilities};
const skya_phone_driver *skya_c_fanvil_x5u_v2_driver(void){return &driver;}
const char *skya_fanvil_x5u_v2_model(void){return MODEL;}
