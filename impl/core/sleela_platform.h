#ifndef SLEELA_PLATFORM_H
#define SLEELA_PLATFORM_H
/* Shared host-platform identity and capability contract. */
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SL_PLATFORM_UNKNOWN=0, SL_PLATFORM_LINUX=1, SL_PLATFORM_WINDOWS=2, SL_PLATFORM_MACOS=3 } SLPlatform;
typedef enum { SL_CAP_MEMORY=0, SL_CAP_FILE_IO, SL_CAP_NETWORK, SL_CAP_THREADS, SL_CAP_DYNAMIC_LIBRARY, SL_CAP_TERMINAL, SL_CAP_PATHS, SL_CAP_PROCESS, SL_CAP_IPC, SL_CAP_TIME, SL_CAP_MAX } SLPlatformCapability;
typedef struct { SLPlatform platform; char name[32]; char version[96]; char architecture[32]; char runtime[96]; } SLPlatformInfo;
int slplatform_detect(SLPlatformInfo *out);
SLPlatform slplatform_current(void);
const char *slplatform_name(SLPlatform platform);
int slplatform_capability_available(SLPlatformCapability capability);
int slplatform_capability_for(SLPlatform platform, SLPlatformCapability capability);
int slplatform_is_supported_version(const SLPlatformInfo *info);
int slplatform_format_report(char *buf, size_t cap);
#ifdef __cplusplus
}
#endif
#endif
