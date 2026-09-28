#ifndef SLEELA_MEMMGR_GRADE3_H
#define SLEELA_MEMMGR_GRADE3_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SLMM_G3_LINUX=1, SLMM_G3_WINDOWS=2, SLMM_G3_MACOS=3, SLMM_G3_OTHER=255 } SLMMG3Platform;
typedef enum { SLMM_G3_MEMORY=1, SLMM_G3_FILE=2, SLMM_G3_SOCKET=3, SLMM_G3_PROCESS=4, SLMM_G3_THREAD=5, SLMM_G3_LIBRARY=6, SLMM_G3_TERMINAL=7, SLMM_G3_GENERIC=255 } SLMMG3Resource;
typedef struct { uint64_t calls, failures; size_t bytes, peak_bytes; uint64_t live_handles; } SLMMG3Stats;
typedef struct { int initialized; SLMMG3Platform platform; size_t memory_limit, live_memory, peak_memory; uint64_t call_count, failure_count; SLMMG3Stats resource[8]; } SLMMGrade3;
int slmm_g3_init(SLMMGrade3*, size_t memory_limit);
SLMMG3Platform slmm_g3_platform(void);
int slmm_g3_begin(SLMMGrade3*, SLMMG3Resource, size_t bytes, uint64_t handles);
void slmm_g3_end(SLMMGrade3*, SLMMG3Resource, size_t bytes, uint64_t handles);
void slmm_g3_fail(SLMMGrade3*, SLMMG3Resource);
int slmm_g3_stats(const SLMMGrade3*, SLMMG3Resource, SLMMG3Stats*);
int slmm_g3_validate(const SLMMGrade3*);
#ifdef __cplusplus
}
#endif
#endif