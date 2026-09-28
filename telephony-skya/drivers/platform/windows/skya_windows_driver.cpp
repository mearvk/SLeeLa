#ifdef _WIN32
#include "skya_windows_driver.h"
#include "../../src/skya_driver_registry.h"
#include <windows.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <wrl/client.h>
#include <cstring>
using Microsoft::WRL::ComPtr;
static void cw(char*d,size_t n,const wchar_t*s){if(d&&n&&s){WideCharToMultiByte(CP_UTF8,0,s,-1,d,(int)n,nullptr,nullptr);d[n-1]=0;}}
int skya_windows_enumerate(skya_platform_device*d,size_t cap){if(!d||!cap)return-1;HRESULT h=CoInitializeEx(nullptr,COINIT_MULTITHREADED);bool u=SUCCEEDED(h);if(FAILED(h)&&h!=RPC_E_CHANGED_MODE)return-2;ComPtr<IMMDeviceEnumerator>e;h=CoCreateInstance(__uuidof(MMDeviceEnumerator),nullptr,CLSCTX_ALL,IID_PPV_ARGS(&e));if(FAILED(h)){if(u)CoUninitialize();return-3;}ComPtr<IMMDeviceCollection>col;h=e->EnumAudioEndpoints(eAll,DEVICE_STATE_ACTIVE,&col);if(FAILED(h)){if(u)CoUninitialize();return-4;}UINT n=0;col->GetCount(&n);size_t out=0;for(UINT i=0;i<n&&out<cap;i++){ComPtr<IMMDevice>x;if(FAILED(col->Item(i,&x)))continue;LPWSTR id=nullptr;if(FAILED(x->GetId(&id)))continue;auto&z=d[out++];std::memset(&z,0,sizeof(z));cw(z.device_id,sizeof(z.device_id),id);CoTaskMemFree(id);ComPtr<IPropertyStore>ps;if(SUCCEEDED(x->OpenPropertyStore(STGM_READ,&ps))){PROPVARIANT v;PropVariantInit(&v);if(SUCCEEDED(ps->GetValue(PKEY_Device_FriendlyName,&v))&&v.vt==VT_LPWSTR)cw(z.model,sizeof(z.model),v.pwszVal);PropVariantClear(&v);}std::strncpy(z.family,"Windows Audio",sizeof(z.family)-1);z.type=SKYA_DRIVER_USB_HEADSET;z.transport=SKYA_TRANSPORT_USB_AUDIO;z.present=1;z.input_channels=1;z.output_channels=2;}if(u)CoUninitialize();return(int)out;}
int skya_windows_load(const skya_platform_device*d,const skya_phone_driver**o,skya_driver_capabilities*c){if(!d||!o||!c)return-1;skya_driver_device x{d->vendor[0]?d->vendor:nullptr,d->model,d->family,nullptr,d->type,d->transport};*o=skya_driver_find(&x);if(!*o)return-2;return(*o)->capabilities(&x,c);}
#else
int skya_windows_enumerate(skya_platform_device*,size_t){return-100;}
int skya_windows_load(const skya_platform_device*,const skya_phone_driver**,skya_driver_capabilities*){return-100;}
#endif
int skya_platform_enumerate(skya_platform_device*d,size_t c){return skya_windows_enumerate(d,c);}
int skya_platform_load(const skya_platform_device*d,const skya_phone_driver**o,skya_driver_capabilities*c){return skya_windows_load(d,o,c);}
