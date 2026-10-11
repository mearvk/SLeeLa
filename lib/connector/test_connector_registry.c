#include "connector_registry.h"

#include <assert.h>
#include <string.h>

int main(void) {
    char error[128];
    int mysql_handle = 1;
    int postgres_handle = 2;
    sleela_connector_view view;
    sleela_connector_registry *registry = sleela_connector_registry_create();

    assert(registry != NULL);
    assert(sleela_connector_register(registry, "mysql", "mysql",
                                     &mysql_handle, error, sizeof error));

    /* A single registered connector is unambiguous and may be resolved by default. */
    assert(sleela_connector_resolve_unnamed(registry, &view, error, sizeof error));
    assert(view.handle == &mysql_handle);
    assert(strcmp(view.reference, "mysql") == 0);

    assert(!sleela_connector_register(registry, "mysql", "postgres",
                                      &postgres_handle, error, sizeof error));
    assert(strstr(error, "duplicate") != NULL);
    assert(sleela_connector_resolve(registry, "mysql", &view, error, sizeof error));
    assert(view.handle == &mysql_handle);

    assert(sleela_connector_register(registry, "postgres", "postgres",
                                     &postgres_handle, error, sizeof error));
    assert(!sleela_connector_resolve_unnamed(registry, &view, error, sizeof error));
    assert(strstr(error, "ambiguous") != NULL);

    assert(sleela_connector_property_set(registry, "mysql", "database", "appdb",
                                         error, sizeof error));
    assert(strcmp(sleela_connector_property_get(registry, "mysql", "database"),
                  "appdb") == 0);
    assert(sleela_connector_property_get(registry, "postgres", "database") == NULL);

    sleela_connector_registry_destroy(registry);
    return 0;
}
