#include "driver.h"
#include "../../../include/skya_product_runtime.h"
static const char *VENDOR="epos"; static const char *MODEL="sdw-5000";
static int probe(const skya_driver_device*d){return skya_product_probe(d,VENDOR,MODEL);}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){return skya_product_capabilities(d,VENDOR,MODEL,out);}
static const skya_phone_driver driver={"epos_sdw_5000","epos",probe,capabilities};
const skya_phone_driver *skya_c_epos_sdw_5000_driver(void){return &driver;}
const char *skya_epos_sdw_5000_model(void){return MODEL;}
