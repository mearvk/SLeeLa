#ifndef SLEELA_NET_H
#define SLEELA_NET_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SL_NET_AUTO = 0,
    SL_NET_LINUX = 1,
    SL_NET_WINDOWS = 2
} SLNetPlatform;

typedef intptr_t SLNetHandle;
typedef ptrdiff_t SLNetCount;
#define SL_NET_INVALID ((SLNetHandle)-1)

SLNetPlatform slnet_platform(void);
const char* slnet_platform_name(void);
int slnet_platform_is_available(SLNetPlatform platform);

/* TCP lifecycle. Handles are SLeeLa-owned opaque values, never raw SOCKETs. */
int slnet_startup(void);
void slnet_shutdown(void);
SLNetHandle slnet_listen(uint16_t port, int backlog);
SLNetHandle slnet_accept(SLNetHandle listener);
SLNetHandle slnet_connect(const char* host, uint16_t port);
SLNetCount slnet_read(SLNetHandle handle, char* buffer, size_t capacity);
SLNetCount slnet_write(SLNetHandle handle, const char* data, size_t length);
int slnet_close(SLNetHandle handle);

/* Socket-handle validity and error normalization. */
int slnet_valid(SLNetHandle handle);
int slnet_last_error(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_NET_H */
