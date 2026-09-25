#ifndef SLEEELA_MEMORY_MANAGER_H
#define SLEEELA_MEMORY_MANAGER_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sleela_mm sleela_mm;
typedef struct { size_t resident,allocated,freed,allocations,frees,peak; double pressure,voltage,current,flow; uint64_t condition; } sleela_mm_stats;
typedef struct { size_t soft_limit,hard_limit; } sleela_mm_limits;
typedef struct { double voltage_per_byte,current_per_allocation,flow_gain; } sleela_mm_flow_model;
typedef enum { SLEELA_MM_OK=0,SLEELA_MM_INVALID=-1,SLEELA_MM_LIMIT=-2,SLEELA_MM_NOT_FOUND=-3,SLEELA_MM_CORRUPT=-4 } sleela_mm_status;
sleela_mm* sleela_mm_create(void); void sleela_mm_destroy(sleela_mm*);
void* sleela_mm_alloc(sleela_mm*,size_t); void* sleela_mm_calloc(sleela_mm*,size_t,size_t); void* sleela_mm_realloc(sleela_mm*,void*,size_t); sleela_mm_status sleela_mm_free(sleela_mm*,void*);
sleela_mm_status sleela_mm_insert_struct(sleela_mm*,const char*,const void*,size_t,size_t,uint64_t*); sleela_mm_status sleela_mm_remove(sleela_mm*,uint64_t);
sleela_mm_status sleela_mm_leech_attach(sleela_mm*,const char*,uint64_t,uint64_t*); sleela_mm_status sleela_mm_leech_detach(sleela_mm*,uint64_t);
void sleela_mm_set_limits(sleela_mm*,sleela_mm_limits); void sleela_mm_set_flow_model(sleela_mm*,sleela_mm_flow_model); sleela_mm_stats sleela_mm_stats_get(const sleela_mm*); uint64_t sleela_mm_condition(const sleela_mm*); int sleela_mm_validate(const sleela_mm*);
#ifdef __cplusplus
}
#endif
#endif
