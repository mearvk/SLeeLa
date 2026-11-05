/* SLeeLa Registry 3.0 record API. */
#ifndef SLEELA_REGISTRY_V3_H
#define SLEELA_REGISTRY_V3_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLR3_ID_HEX 32
#define SLR3_DIGEST_HEX 64
#define SLR3_MAX_ALIASES 16
#define SLR3_MAX_RELATIONS 32
#define SLR3_MIN_REGION_BYTES (UINT64_C(256) * 1024 * 1024)
#define SLR3_MAX_REGION_BYTES (UINT64_C(64) * 1024 * 1024 * 1024)
typedef enum slr3_status {
 SLR3_OK=0, SLR3_INVALID=-1, SLR3_NOT_FOUND=-2, SLR3_EXISTS=-3, SLR3_LIMIT=-4,
 SLR3_MALFORMED=-5, SLR3_IO=-6, SLR3_CONFLICT=-7, SLR3_BAD_TRANSITION=-8, SLR3_INTEGRITY=-9
} slr3_status;
typedef struct slr3_relationship { char type[32]; char target_id[SLR3_ID_HEX+1]; } slr3_relationship;
typedef struct slr3_record {
 char id[SLR3_ID_HEX+1], name_space[128], kind[48], schema_version[24], name[256], state[24];
 char locator[512], version[64], content_digest[SLR3_DIGEST_HEX+1];
 uint64_t revision;
 char payload_kind[16], payload[1024];
 uint64_t capacity_bytes;
 char owner_label[128], format_version[24], device_seal[SLR3_DIGEST_HEX+1], integrity_status[24];
 char provenance_name[256], provenance_date[32], provenance_author[128], provenance_version[64];
 char provenance_hex_id[17], provenance_hash[65], provenance_seal[65];
 size_t alias_count; char aliases[SLR3_MAX_ALIASES][128];
 size_t relationship_count; slr3_relationship relationships[SLR3_MAX_RELATIONS];
} slr3_record;
typedef struct slr3_registry slr3_registry;
slr3_status slr3_id_from_seed(const char *seed,char out_id[SLR3_ID_HEX+1]);
slr3_registry *slr3_registry_create(const char *registry_id,size_t max_records);
void slr3_registry_destroy(slr3_registry *registry);
void slr3_record_init(slr3_record *record,const char *id,const char *name_space,const char *kind,const char *name);
slr3_status slr3_registry_add(slr3_registry *registry,const slr3_record *record);
const slr3_record *slr3_registry_get(const slr3_registry *registry,const char *id);
/* Namespace-scoped name/alias lookup; returns NULL when not present. */
const slr3_record *slr3_registry_identify(const slr3_registry *registry,const char *name_space,const char *name_or_alias);
size_t slr3_registry_count(const slr3_registry *registry);
const char *slr3_registry_id(const slr3_registry *registry);
slr3_status slr3_registry_add_alias(slr3_registry *registry,const char *id,const char *alias);
slr3_status slr3_registry_relate(slr3_registry *registry,const char *source_id,const char *relationship_type,const char *target_id);
slr3_status slr3_registry_rename(slr3_registry *registry,const char *id,const char *new_name,uint64_t expected_revision);
slr3_status slr3_registry_set_locator(slr3_registry *registry,const char *id,const char *locator,uint64_t expected_revision);
slr3_status slr3_registry_set_integrity(slr3_registry *registry,const char *id,const char *integrity_status,uint64_t expected_revision);
slr3_status slr3_registry_transition(slr3_registry *registry,const char *id,const char *next_state,uint64_t expected_revision);
/* Catalogs an inspected region only. No discovery, claim, mount, format, resize, read, or write. */
slr3_status slr3_modium_region_register(slr3_registry *registry,const char *id,const char *name,const char *region_kind,const char *locator,uint64_t capacity_bytes,const char *owner_label,const char *descriptor_version,const char *device_seal);
slr3_status slr3_registry_save_v2(const slr3_registry *registry,const char *path);
slr3_status slr3_registry_load_v2(slr3_registry *registry,const char *path);
slr3_status slr3_registry_migrate_v1(const char *input_path,const char *output_path,const char *new_registry_id,size_t max_records);
slr3_status slr3_registry_review(const slr3_registry *registry,char *buffer,size_t capacity);
const char *slr3_status_name(slr3_status status);
#ifdef __cplusplus
}
#endif
#endif
