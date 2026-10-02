#include "slvm2.h"
#include <stdio.h>
#include <string.h>

int slvm2_config_defaults(slvm2_config_t *c) {
    if (!c) return -1;
    memset(c, 0, sizeof(*c));
    c->instance = 2;
    c->runtime_target = SLVM2_RUNTIME_AUTO;
    c->memory_limit = 512ULL * 1024ULL * 1024ULL;
    c->security_profile = 1;
    c->crypto_provider = 1;
    c->link_policy = 1;
    c->observer_mode = 1;
    c->certificate_policy = 1;
    c->compliance_profile = 1;
    return 0;
}

int slvm2_config_load_file(slvm2_config_t *c, const char *path) {
    FILE *f;
    char line[512];
    if (!c || !path) return -1;
    if (slvm2_config_defaults(c) != 0) return -1;
    f = fopen(path, "r");
    if (!f) return -1;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "vm.instance = 2", 16) == 0) c->instance = 2;
        else if (strncmp(line, "runtime.target = linux", 21) == 0) c->runtime_target = SLVM2_RUNTIME_LINUX;
        else if (strncmp(line, "runtime.target = windows", 23) == 0) c->runtime_target = SLVM2_RUNTIME_WINDOWS;
        else if (strncmp(line, "runtime.target = macos", 21) == 0) c->runtime_target = SLVM2_RUNTIME_MACOS;
        else if (strncmp(line, "memory.limit = ", 15) == 0) c->memory_limit = strtoull(line + 15, NULL, 10);
    }
    fclose(f);
    return slvm2_runtime_validate(c);
}

int slvm2_runtime_validate(const slvm2_config_t *c) {
    if (!c || c->instance != 2 || c->memory_limit == 0) return -1;
    return 0;
}
