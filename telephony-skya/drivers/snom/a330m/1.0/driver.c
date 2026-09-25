#include "driver.h"
#include "../../../include/skya_product_runtime.h"
static const char *VENDOR="snom"; static const char *MODEL="a330m";
static int probe(const skya_driver_device*d){return skya_product_probe(d,VENDOR,MODEL);}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){return skya_product_capabilities(d,VENDOR,MODEL,out);}
static const skya_phone_driver driver={"snom_a330m","snom",probe,capabilities};
const skya_phone_driver *skya_snom_a330m_driver(void){return &driver;}
const char *skya_snom_a330m_model(void){return MODEL;}
