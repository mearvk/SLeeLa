#ifndef SLEELA_DEBUG_ENGINE_C_H
#define SLEELA_DEBUG_ENGINE_C_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sleela_debug_engine sleela_debug_engine_t;
typedef struct { const char* file; uint32_t line; uint32_t column; } sleela_debug_engine_location_t;
typedef struct { uint64_t id; int kind; sleela_debug_engine_location_t location; const char* function; const char* condition; int enabled; uint64_t hit_count; } sleela_debug_engine_breakpoint_t;
sleela_debug_engine_t* sleela_debug_engine_create(void);
void sleela_debug_engine_destroy(sleela_debug_engine_t*);
uint64_t sleela_debug_engine_add_breakpoint(sleela_debug_engine_t*,int,const char*,uint32_t,uint32_t,const char*,int);
int sleela_debug_engine_remove_breakpoint(sleela_debug_engine_t*,uint64_t);
int sleela_debug_engine_hit_breakpoint(sleela_debug_engine_t*,uint64_t);
int sleela_debug_engine_capability(sleela_debug_engine_t*,const char*);
#ifdef __cplusplus
}
#endif
#endif