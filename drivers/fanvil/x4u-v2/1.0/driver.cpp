#include "driver.h"
#include <cstring>
static int probe(const skya_driver_device*d){return d&&d->vendor&&d->model&&std::strcmp(d->vendor,"Fanvil")==0&&std::strstr(d->model,"X4U-V2")!=nullptr;}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){if(!d||!out)return -1;*out={};if(d->transport!=SKYA_TRANSPORT_SIP&&d->transport!=SKYA_TRANSPORT_NETWORK)return 0;out->audio_input=1;out->audio_output=1;out->call_answer=1;out->call_end=1;out->call_hold=1;out->mute=1;out->volume=1;out->dialpad=1;out->display=1;out->firmware_query=1;return 0;}
static const skya_phone_driver driver={"fanvil_x4u_v2","Fanvil",probe,capabilities};
extern "C" const skya_phone_driver *skya_fanvil_x4u_v2_driver(void){return &driver;}
