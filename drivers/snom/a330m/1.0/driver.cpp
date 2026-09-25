#include "driver.h"
#include <cstring>
static int probe(const skya_driver_device*d){return d&&d->vendor&&d->model&&std::strcmp(d->vendor,"Snom")==0&&std::strstr(d->model,"A330M")!=nullptr;}
static int capabilities(const skya_driver_device*d,skya_driver_capabilities*out){if(!d||!out)return -1;*out={};out->audio_input=1;out->audio_output=1;out->mute=1;out->volume=1;out->firmware_query=1;out->headset_port=1;if(d->transport==SKYA_TRANSPORT_USB_HID){out->call_answer=1;out->call_end=1;}return 0;}
static const skya_phone_driver driver={"snom_a330m","Snom",probe,capabilities};
extern "C" const skya_phone_driver *skya_snom_a330m_driver(void){return &driver;}
