#include "driver.h"
#include "../../../include/skya_product_runtime.h"
static const char *VENDOR="poly"; static const char *MODEL="savi-8400";
static int probe(const skya_driver_device*d){return skya_product_probe(d,VENDOR,MODEL);}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){return skya_product_capabilities(d,VENDOR,MODEL,out);}
static const skya_phone_driver driver={"poly_savi_8400","poly",probe,capabilities};
const skya_phone_driver *skya_poly_savi_8400_driver(void){return &driver;}
const char *skya_poly_savi_8400_model(void){return MODEL;}
