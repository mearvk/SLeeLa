#include "driver.h"
#include "../../../include/skya_product_runtime.h"
static const char *VENDOR="poly"; static const char *MODEL="blackwire-3320";
static int probe(const skya_driver_device*d){return skya_product_probe(d,VENDOR,MODEL);}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){return skya_product_capabilities(d,VENDOR,MODEL,out);}
static const skya_phone_driver driver={"poly_blackwire_3320","poly",probe,capabilities};
const skya_phone_driver *skya_c_poly_blackwire_3320_driver(void){return &driver;}
const char *skya_poly_blackwire_3320_model(void){return MODEL;}
