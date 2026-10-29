#include "vm_config.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *trim(char *s) {
    while (*s && isspace((unsigned char)*s)) ++s;
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) --end;
    *end = '\0';
    return s;
}

static int version_valid(long value) {
    return value >= SLEELA_VM_MIN_VERSION && value <= SLEELA_VM_MAX_VERSION;
}

static int parse_version(const char *text, int *version) {
    char *end = NULL;
    long value;
    if (!text || !*text || !version) return 0;
    errno = 0;
    value = strtol(text, &end, 10);
    if (errno == ERANGE || end == text || !end || *end || !version_valid(value)) return 0;
    *version = (int)value;
    return 1;
}

void sleela_vm_config_defaults(SLEELA_VM_CONFIG *config) {
    if (!config) return;
    config->version = SLEELA_VM_DEFAULT_VERSION;
    config->strict = 1;
    memcpy(config->profile, "user", sizeof("user"));
}

int sleela_vm_config_set_version(SLEELA_VM_CONFIG *config, int version) {
    if (!config || !version_valid(version)) return -1;
    config->version = version;
    return 0;
}

int sleela_vm_config_load(const char *path, SLEELA_VM_CONFIG *config) {
    FILE *file;
    char line[256];
    unsigned seen = 0;
    SLEELA_VM_CONFIG next;
    if (!path || !config) return -1;
    file = fopen(path, "rb");
    if (!file) return -2;
    sleela_vm_config_defaults(&next);
    while (fgets(line, sizeof(line), file)) {
        size_t length = strlen(line);
        char *entry, *equals, *key, *value;
        if (length && line[length - 1] != '\n') {
            int next = fgetc(file);
            if (next != EOF) {
                fclose(file);
                return -3;
            }
            if (ferror(file)) {
                fclose(file);
                return -7;
            }
        }
        entry = trim(line);
        if (!*entry || *entry == '#' || *entry == ';') continue;
        equals = strchr(entry, '=');
        if (!equals) { fclose(file); return -3; }
        *equals = '\0';
        key = trim(entry);
        value = trim(equals + 1);
        /* JavaFX is optional; an empty module path means use Java defaults. */
        if (!*key || (!*value && strcmp(key, "javafx.module_path") != 0)) {
            fclose(file);
            return -3;
        }
        if (!strcmp(key, "vm.version")) {
            if ((seen & 1u) || !parse_version(value, &next.version)) { fclose(file); return -4; }
            seen |= 1u;
        } else if (!strcmp(key, "vm.strict")) {
            if (seen & 2u) { fclose(file); return -5; }
            if (!strcmp(value, "true") || !strcmp(value, "1")) next.strict = 1;
            else if (!strcmp(value, "false") || !strcmp(value, "0")) next.strict = 0;
            else { fclose(file); return -5; }
            seen |= 2u;
        } else if (!strcmp(key, "vm.profile")) {
            if ((seen & 4u) || (strcmp(value, "user") &&
                strcmp(value, "administrator") && strcmp(value, "developer"))) {
                fclose(file);
                return -6;
            }
            memcpy(next.profile, value, strlen(value) + 1);
            seen |= 4u;
        }
    }
    if (ferror(file)) { fclose(file); return -7; }
    if (fclose(file) != 0) return -7;
    if (sleela_vm_config_validate(&next) != 0) return -8;
    *config = next;
    return 0;
}

int sleela_vm_config_apply_environment(SLEELA_VM_CONFIG *config) {
    const char *value;
    int version;
    if (!config) return -1;
    value = getenv("SLEELA_VM_VERSION");
    if (!value || !*value) return 0;
    if (!parse_version(value, &version)) return -2;
    config->version = version;
    return 0;
}

int sleela_vm_config_validate(const SLEELA_VM_CONFIG *config) {
    if (!config || !version_valid(config->version)) return -1;
    if (strcmp(config->profile, "user") &&
        strcmp(config->profile, "administrator") &&
        strcmp(config->profile, "developer")) return -2;
    return (config->strict == 0 || config->strict == 1) ? 0 : -3;
}
