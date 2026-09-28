#include "driver.h"
#include <string.h>
static int probe(const skya_driver_device*d){return d&&d->vendor&&d->model&&strcmp(d->vendor,"EPOS")==0&&strstr(d->model,"IMPACT SC 600")!=0;}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){if(!d||!out)return -1;memset(out,0,sizeof(*out));out->audio_input=1;out->audio_output=1;out->mute=1;out->volume=1;out->firmware_query=1;out->headset_port=1;if(d->transport==SKYA_TRANSPORT_USB_HID){out->call_answer=1;out->call_end=1;}return 0;}
static const skya_phone_driver driver={"c-epos_impact_sc600","EPOS",probe,capabilities};
const skya_phone_driver *skya_c_epos_impact_sc600_driver(void){return &driver;}
