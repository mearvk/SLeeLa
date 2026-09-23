#include "sleela_memmgr_grade3.h"
#include <string.h>
#if defined(_WIN32)
#define G3P SLMM_G3_WINDOWS
#elif defined(__APPLE__)
#define G3P SLMM_G3_MACOS
#elif defined(__linux__)
#define G3P SLMM_G3_LINUX
#else
#define G3P SLMM_G3_OTHER
#endif
static int okr(SLMMG3Resource r){return r>=SLMM_G3_MEMORY&&r<=SLMM_G3_TERMINAL;}
int slmm_g3_init(SLMMGrade3*m,size_t limit){if(!m||!limit)return -1;memset(m,0,sizeof(*m));m->initialized=1;m->platform=G3P;m->memory_limit=limit;return 0;}
SLMMG3Platform slmm_g3_platform(void){return G3P;}
int slmm_g3_begin(SLMMGrade3*m,SLMMG3Resource r,size_t b,uint64_t h){if(!m||!m->initialized||!okr(r))return -1;m->call_count++;if(b>SIZE_MAX-m->live_memory||m->live_memory+b>m->memory_limit){m->failure_count++;m->resource[r].failures++;return -1;}m->live_memory+=b;if(m->live_memory>m->peak_memory)m->peak_memory=m->live_memory;SLMMG3Stats*s=&m->resource[r];s->calls++;if(SIZE_MAX-s->bytes<b){m->failure_count++;s->failures++;return -1;}s->bytes+=b;if(b>s->peak_bytes)s->peak_bytes=b;if(h>UINT64_MAX-s->live_handles){m->failure_count++;s->failures++;return -1;}s->live_handles+=h;return 0;}
void slmm_g3_end(SLMMGrade3*m,SLMMG3Resource r,size_t b,uint64_t h){if(!m||!m->initialized||!okr(r))return;if(b>m->live_memory)b=m->live_memory;m->live_memory-=b;SLMMG3Stats*s=&m->resource[r];if(h>s->live_handles)s->live_handles=0;else s->live_handles-=h;}
void slmm_g3_fail(SLMMGrade3*m,SLMMG3Resource r){if(!m||!m->initialized||!okr(r))return;m->call_count++;m->failure_count++;m->resource[r].failures++;}
int slmm_g3_stats(const SLMMGrade3*m,SLMMG3Resource r,SLMMG3Stats*out){if(!m||!m->initialized||!okr(r)||!out)return -1;*out=m->resource[r];return 0;}
int slmm_g3_validate(const SLMMGrade3*m){return m&&m->initialized&&m->live_memory<=m->memory_limit;}