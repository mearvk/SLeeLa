#include "driver.h"
#include "../../../include/skya_product_runtime.h"
static const char *VENDOR="grandstream"; static const char *MODEL="grp26xx";
static int probe(const skya_driver_device*d){return skya_product_probe(d,VENDOR,MODEL);}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){return skya_product_capabilities(d,VENDOR,MODEL,out);}
static const skya_phone_driver driver={"grandstream_grp26xx","grandstream",probe,capabilities};
const skya_phone_driver *skya_grandstream_grp26xx_driver(void){return &driver;}
const char *skya_grandstream_grp26xx_model(void){return MODEL;}
