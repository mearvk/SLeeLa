#include "driver.h"
#include "../../../include/skya_product_runtime.h"
static const char *VENDOR="logitech"; static const char *MODEL="h570e";
static int probe(const skya_driver_device*d){return skya_product_probe(d,VENDOR,MODEL);}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){return skya_product_capabilities(d,VENDOR,MODEL,out);}
static const skya_phone_driver driver={"logitech_h570e","logitech",probe,capabilities};
const skya_phone_driver *skya_logitech_h570e_driver(void){return &driver;}
const char *skya_logitech_h570e_model(void){return MODEL;}
