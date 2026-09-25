#include "driver.h"
#include "../../../include/skya_product_runtime.h"
static const char *VENDOR="yealink"; static const char *MODEL="uh42";
static int probe(const skya_driver_device*d){return skya_product_probe(d,VENDOR,MODEL);}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){return skya_product_capabilities(d,VENDOR,MODEL,out);}
static const skya_phone_driver driver={"yealink_uh42","yealink",probe,capabilities};
const skya_phone_driver *skya_yealink_uh42_driver(void){return &driver;}
const char *skya_yealink_uh42_model(void){return MODEL;}
