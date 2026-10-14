/* ========================================================================
 * sleela_io.h -- operating-system-aware file and pipe abstraction.
 *
 * The SLeeLa language exposes one I/O API while the host implementation
 * selects Linux/POSIX pipes and FIFOs or Windows anonymous/named pipes.
 * ======================================================================== */
#ifndef SLEELA_IO_H
#define SLEELA_IO_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SL_IO_AUTO = 0,
    SL_IO_LINUX = 1,
    SL_IO_WINDOWS = 2,
    SL_IO_MACOS = 3
} SLIOPlatform;

/* Host platform selected at compile time; may be overridden by
 * SLEELA_IO_PLATFORM only when the requested platform is the native host. */
SLIOPlatform slio_platform(void);
const char* slio_platform_name(void);
int slio_platform_is_available(SLIOPlatform platform);

/* Opaque VM-local file/pipe handles. */
typedef intptr_t SLIOHandle;
#define SLIO_INVALID_HANDLE ((SLIOHandle)-1)

/* mode strings use r/w/a/rw/r+/w+/a+ plus optional n (nonblocking). */
int slio_pipe(SLIOHandle* read_handle, SLIOHandle* write_handle);
int slio_named_pipe_create(const char* path, unsigned mode);
SLIOHandle slio_open(const char* path, const char* mode);
int slio_read(SLIOHandle handle, char* buffer, size_t capacity);
int slio_write(SLIOHandle handle, const char* data, size_t length);
int slio_close(SLIOHandle handle);
int slio_unlink(const char* path);
int slio_peer(SLIOHandle handle, SLIOHandle* peer);

/* The API calls the Windows named-pipe namespace "NPFS" and the Linux
 * filesystem FIFO namespace "FIFO". The same SLeeLa source operation can
 * therefore select either transport without exposing host handles. */
int slio_create_pipe_endpoint(const char* path, unsigned mode);

#ifdef __cplusplus
}
#endif
#endif /* SLEELA_IO_H */
