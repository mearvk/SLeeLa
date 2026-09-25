#include "driver.h"
#include "../../../include/skya_product_runtime.h"
static const char *VENDOR="grandstream"; static const char *MODEL="gxp21xx";
static int probe(const skya_driver_device*d){return skya_product_probe(d,VENDOR,MODEL);}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){return skya_product_capabilities(d,VENDOR,MODEL,out);}
static const skya_phone_driver driver={"grandstream_gxp21xx","grandstream",probe,capabilities};
const skya_phone_driver *skya_c_grandstream_gxp21xx_driver(void){return &driver;}
const char *skya_grandstream_gxp21xx_model(void){return MODEL;}
