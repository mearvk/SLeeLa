#include "driver.h"
#include "../../../include/skya_product_runtime.h"
static const char *VENDOR="fanvil"; static const char *MODEL="v63";
static int probe(const skya_driver_device*d){return skya_product_probe(d,VENDOR,MODEL);}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){return skya_product_capabilities(d,VENDOR,MODEL,out);}
static const skya_phone_driver driver={"fanvil_v63","fanvil",probe,capabilities};
const skya_phone_driver *skya_c_fanvil_v63_driver(void){return &driver;}
const char *skya_fanvil_v63_model(void){return MODEL;}
