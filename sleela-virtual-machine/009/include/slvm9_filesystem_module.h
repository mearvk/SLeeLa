#ifndef SLVM9_FILESYSTEM_MODULE_H
#define SLVM9_FILESYSTEM_MODULE_H
#include <stdint.h>
#define SLVM9_FS_MODULE_SCHEMA "sleela.filesystem.module/v1"
#define SLVM9_FS_MODULE_TAC3_ID "tac3"
typedef struct { const char *module_id; const char *schema; const char *name; uint16_t format_major, format_minor; uint32_t block_size; uint8_t verified, read_only_reconstruction, durable_write_support, boot_recovery, fat_optional; } slvm9_filesystem_module_t;
int slvm9_filesystem_module_validate(const slvm9_filesystem_module_t *);
int slvm9_filesystem_module_is_tac3(const slvm9_filesystem_module_t *);
#endif
