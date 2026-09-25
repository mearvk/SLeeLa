#include "driver.h"
#include "../../../include/skya_product_runtime.h"
static const char *VENDOR="jabra"; static const char *MODEL="evolve2-40";
static int probe(const skya_driver_device*d){return skya_product_probe(d,VENDOR,MODEL);}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){return skya_product_capabilities(d,VENDOR,MODEL,out);}
static const skya_phone_driver driver={"jabra_evolve2_40","jabra",probe,capabilities};
const skya_phone_driver *skya_c_jabra_evolve2_40_driver(void){return &driver;}
const char *skya_jabra_evolve2_40_model(void){return MODEL;}
