#ifndef SLEELA_MEMMGR_GRADE2_H
#define SLEELA_MEMMGR_GRADE2_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLMM_G2_MAX_PROCESSES 3
typedef struct { int active; uint64_t id; size_t limit_bytes, live_bytes, peak_bytes; uint64_t allocations, frees, refused; } SLMMProcessStats;
typedef struct { int initialized; size_t aggregate_limit, aggregate_live, aggregate_peak; unsigned active_processes; SLMMProcessStats process[SLMM_G2_MAX_PROCESSES]; } SLMMGrade2;
int slmm_g2_init(SLMMGrade2*, size_t aggregate_limit);
int slmm_g2_spawn(SLMMGrade2*, uint64_t process_id, size_t process_limit, unsigned *slot);
int slmm_g2_reserve(SLMMGrade2*, unsigned slot, size_t bytes);
void slmm_g2_release(SLMMGrade2*, unsigned slot, size_t bytes);
void slmm_g2_close(SLMMGrade2*, unsigned slot);
int slmm_g2_stats(const SLMMGrade2*, unsigned slot, SLMMProcessStats*);
int slmm_g2_validate(const SLMMGrade2*);
#ifdef __cplusplus
}
#endif
#endif