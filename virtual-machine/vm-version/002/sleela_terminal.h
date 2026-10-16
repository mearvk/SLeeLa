#ifndef SLEELA_TERMINAL_H
#define SLEELA_TERMINAL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef intptr_t SLTerminalHandle;
typedef ptrdiff_t SLTerminalCount;

typedef enum {
    SL_TERMINAL_AUTO = 0,
    SL_TERMINAL_LINUX = 1,
    SL_TERMINAL_WINDOWS = 2,
    SL_TERMINAL_MACOS = 3
} SLTerminalPlatform;

SLTerminalPlatform slterminal_platform(void);
const char* slterminal_platform_name(void);
int slterminal_platform_is_available(SLTerminalPlatform platform);

/* Start a shell command under a real pseudo-terminal. command may be NULL for
 * an interactive shell. cols/rows specify the initial terminal geometry. */
int slterminal_spawn(SLTerminalHandle* terminal, const char* command,
                     unsigned cols, unsigned rows);

SLTerminalCount slterminal_read(SLTerminalHandle terminal, void* buffer,
                                size_t size);
SLTerminalCount slterminal_write(SLTerminalHandle terminal, const void* buffer,
                                 size_t size);

int slterminal_resize(SLTerminalHandle terminal, unsigned cols, unsigned rows);
int slterminal_close(SLTerminalHandle terminal);
int slterminal_valid(SLTerminalHandle terminal);
int slterminal_last_error(void);

#ifdef __cplusplus
}
#endif

#endif
