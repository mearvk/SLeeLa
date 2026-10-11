#ifndef SLEELA_CONNECTOR_REGISTRY_H
#define SLEELA_CONNECTOR_REGISTRY_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sleela_connector_registry sleela_connector_registry;
typedef struct sleela_connector_view { const char *reference; const char *kind; void *handle; } sleela_connector_view;
sleela_connector_registry *sleela_connector_registry_create(void);
void sleela_connector_registry_destroy(sleela_connector_registry *registry);
/* Duplicate references are hard errors. The registry does not own connector handles. */
int sleela_connector_register(sleela_connector_registry *registry,const char *reference,const char *kind,void *handle,char *error,size_t error_size);
int sleela_connector_resolve(sleela_connector_registry *registry,const char *reference,sleela_connector_view *out,char *error,size_t error_size);
/* Unnamed resolution succeeds only when exactly one connector is registered. */
int sleela_connector_resolve_unnamed(sleela_connector_registry *registry,sleela_connector_view *out,char *error,size_t error_size);
int sleela_connector_property_set(sleela_connector_registry *registry,const char *reference,const char *key,const char *value,char *error,size_t error_size);
const char *sleela_connector_property_get(const sleela_connector_registry *registry,const char *reference,const char *key);
size_t sleela_connector_count(const sleela_connector_registry *registry);
#ifdef __cplusplus
}
#endif
#endif
