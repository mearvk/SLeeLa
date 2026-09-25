#include "driver.h"
#include <string.h>
static int probe(const skya_driver_device*d){return d&&d->vendor&&d->model&&strcmp(d->vendor,"Fanvil")==0&&strstr(d->model,"X210i-V2")!=0;}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){if(!d||!out)return -1;memset(out,0,sizeof(*out));if(d->transport!=SKYA_TRANSPORT_SIP&&d->transport!=SKYA_TRANSPORT_NETWORK)return 0;out->audio_input=1;out->audio_output=1;out->call_answer=1;out->call_end=1;out->call_hold=1;out->mute=1;out->volume=1;out->dialpad=1;out->display=1;out->firmware_query=1;return 0;}
static const skya_phone_driver driver={"c-fanvil_x210i_v2","Fanvil",probe,capabilities};
const skya_phone_driver *skya_c_fanvil_x210i_v2_driver(void){return &driver;}
