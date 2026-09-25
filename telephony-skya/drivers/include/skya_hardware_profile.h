#ifndef SKYA_HARDWARE_PROFILE_H
#define SKYA_HARDWARE_PROFILE_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SKYA_EVIDENCE_UNKNOWN=0, SKYA_EVIDENCE_DOCUMENTED, SKYA_EVIDENCE_ENUMERATED, SKYA_EVIDENCE_MEASURED, SKYA_EVIDENCE_TESTED, SKYA_EVIDENCE_CERTIFIED } skya_evidence_level;
typedef enum { SKYA_PIN_UNSPECIFIED=0, SKYA_PIN_POWER_RAIL, SKYA_PIN_GROUND, SKYA_PIN_CONTROL, SKYA_PIN_INPUT, SKYA_PIN_OUTPUT, SKYA_PIN_BIDIRECTIONAL, SKYA_PIN_RESET, SKYA_PIN_CLOCK, SKYA_PIN_DATA, SKYA_PIN_LED, SKYA_PIN_BUTTON, SKYA_PIN_AUDIO } skya_pin_role;
typedef enum { SKYA_PIN_INPUT_ONLY=0, SKYA_PIN_OUTPUT_ONLY, SKYA_PIN_INOUT } skya_pin_direction;
typedef enum { SKYA_PULL_NONE=0, SKYA_PULL_UP, SKYA_PULL_DOWN } skya_pin_pull;
typedef enum { SKYA_CACHE_NONE=0, SKYA_CACHE_WRITE_THROUGH, SKYA_CACHE_WRITE_BACK, SKYA_CACHE_DEVICE, SKYA_CACHE_COHERENT } skya_cache_policy;
typedef enum { SKYA_MEMORY_UNKNOWN=0, SKYA_MEMORY_REGISTER, SKYA_MEMORY_RAM, SKYA_MEMORY_FLASH, SKYA_MEMORY_EEPROM, SKYA_MEMORY_NVRAM, SKYA_MEMORY_DMA } skya_memory_type;
typedef struct { const char *name,*number; skya_pin_role role; skya_pin_direction direction; skya_pin_pull pull; int active_high; uint32_t voltage_min_mv,voltage_nominal_mv,voltage_max_mv,max_current_ma,flags; skya_evidence_level evidence; } skya_pin_spec;
typedef struct { const char *name; skya_memory_type type; uint64_t address,size; uint32_t alignment; skya_cache_policy cache; int readable,writable,executable,volatile_storage; skya_evidence_level evidence; } skya_memory_region;
typedef struct { uint32_t cache_line_bytes,read_ahead_bytes,write_buffer_bytes; skya_cache_policy default_policy; int dma_coherent,requires_flush,requires_invalidate; } skya_cache_spec;
typedef struct { const char *vendor,*model,*family,*hardware_revision,*firmware_version,*interface_revision; skya_evidence_level identity_evidence; const skya_pin_spec *pins; size_t pin_count; const skya_memory_region *memory; size_t memory_count; skya_cache_spec cache; uint32_t flags; } skya_hardware_profile;
typedef struct { int (*set_pin)(void*,const skya_pin_spec*,int); int (*read_pin)(void*,const skya_pin_spec*,int*); int (*set_voltage)(void*,const skya_pin_spec*,uint32_t); int (*read_memory)(void*,const skya_memory_region*,uint64_t,void*,size_t); int (*write_memory)(void*,const skya_memory_region*,uint64_t,const void*,size_t); int (*flush_cache)(void*,const skya_memory_region*); int (*invalidate_cache)(void*,const skya_memory_region*); void *ctx; } skya_hardware_control;
int skya_hardware_profile_validate(const skya_hardware_profile*);
int skya_hardware_pin_set(skya_hardware_control*,const skya_pin_spec*,int);
int skya_hardware_pin_read(skya_hardware_control*,const skya_pin_spec*,int*);
int skya_hardware_voltage_set(skya_hardware_control*,const skya_pin_spec*,uint32_t);
int skya_hardware_memory_read(skya_hardware_control*,const skya_memory_region*,uint64_t,void*,size_t);
int skya_hardware_memory_write(skya_hardware_control*,const skya_memory_region*,uint64_t,const void*,size_t);
int skya_hardware_cache_flush(skya_hardware_control*,const skya_memory_region*);
int skya_hardware_cache_invalidate(skya_hardware_control*,const skya_memory_region*);
#ifdef __cplusplus
}
#endif
#endif
