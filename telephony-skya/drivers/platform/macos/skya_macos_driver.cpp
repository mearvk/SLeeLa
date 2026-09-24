#ifdef __APPLE__
#include "skya_macos_driver.h"
#include "../../src/skya_driver_registry.h"
#include <CoreAudio/CoreAudio.h>
#include <cstring>
static void cs(char*d,size_t n,CFStringRef s){if(d&&n&&s){CFStringGetCString(s,d,(CFIndex)n,kCFStringEncodingUTF8);d[n-1]=0;}}
int skya_macos_enumerate(skya_platform_device*d,size_t cap){if(!d||!cap)return-1;AudioObjectPropertyAddress a{kAudioHardwarePropertyDevices,kAudioObjectPropertyScopeGlobal,kAudioObjectPropertyElementMain};UInt32 size=0;if(AudioObjectGetPropertyDataSize(kAudioObjectSystemObject,&a,0,nullptr,&size)!=noErr)return-2;UInt32 n=size/sizeof(AudioDeviceID);if(!n)return 0;AudioDeviceID*ids=new AudioDeviceID[n];if(AudioObjectGetPropertyData(kAudioObjectSystemObject,&a,0,nullptr,&size,ids)!=noErr){delete[]ids;return-3;}size_t out=0;for(UInt32 i=0;i<n&&out<cap;i++){auto&x=d[out++];std::memset(&x,0,sizeof(x));CFStringRef name=nullptr;UInt32 ns=sizeof(name);AudioObjectPropertyAddress na{kAudioObjectPropertyName,kAudioObjectPropertyScopeGlobal,kAudioObjectPropertyElementMain};if(AudioObjectGetPropertyData(ids[i],&na,0,nullptr,&ns,&name)==noErr)cs(x.model,sizeof(x.model),name);std::strncpy(x.family,"Core Audio",sizeof(x.family)-1);x.type=SKYA_DRIVER_USB_HEADSET;x.transport=SKYA_TRANSPORT_USB_AUDIO;x.present=1;x.input_channels=1;x.output_channels=2;}delete[]ids;return(int)out;}
int skya_macos_load(const skya_platform_device*d,const skya_phone_driver**o,skya_driver_capabilities*c){if(!d||!o||!c)return-1;skya_driver_device x{d->vendor[0]?d->vendor:nullptr,d->model,d->family,nullptr,d->type,d->transport};*o=skya_driver_find(&x);if(!*o)return-2;return(*o)->capabilities(&x,c);}
#else
int skya_macos_enumerate(skya_platform_device*,size_t){return-100;}
int skya_macos_load(const skya_platform_device*,const skya_phone_driver**,skya_driver_capabilities*){return-100;}
#endif
int skya_platform_enumerate(skya_platform_device*d,size_t c){return skya_macos_enumerate(d,c);}
int skya_platform_load(const skya_platform_device*d,const skya_phone_driver**o,skya_driver_capabilities*c){return skya_macos_load(d,o,c);}
