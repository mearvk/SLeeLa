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
    config->locked = 0;
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
            /* No trailing newline: either the line was longer than the buffer
               (truncation -> reject as malformed) or it is the final line at
               EOF (accept). `peek` deliberately does not shadow the outer
               `next` config accumulator. */
            int peek = fgetc(file);
            if (peek != EOF) {
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
        } else if (!strcmp(key, "vm.locked")) {
            if (seen & 8u) { fclose(file); return -9; }
            if (!strcmp(value, "true") || !strcmp(value, "1")) next.locked = 1;
            else if (!strcmp(value, "false") || !strcmp(value, "0")) next.locked = 0;
            else { fclose(file); return -9; }
            seen |= 8u;
        }
    }
    if (ferror(file)) { fclose(file); return -7; }
    if (fclose(file) != 0) return -7;
    if (sleela_vm_config_validate(&next) != 0) return -8;
    *config = next;
    return 0;
}

/* Return 1 iff a trimmed config line sets one of the canonical vm.* keys we
 * rewrite (vm.version / vm.profile / vm.strict / vm.locked). Such lines are
 * dropped on save and re-emitted from `config`; every other line is preserved
 * verbatim (comments, blanks, the java.* block, unknown keys). */
static int is_managed_vm_key_line(const char *line) {
    const char *keys[4] = { "vm.version", "vm.profile", "vm.strict", "vm.locked" };
    char buf[256];
    char *entry, *equals, *key;
    size_t i;
    size_t n = strlen(line);
    if (n >= sizeof(buf)) return 0;           /* over-long: leave untouched */
    memcpy(buf, line, n + 1);
    entry = trim(buf);
    if (!*entry || *entry == '#' || *entry == ';') return 0;
    equals = strchr(entry, '=');
    if (!equals) return 0;
    *equals = '\0';
    key = trim(entry);
    for (i = 0; i < 4; ++i) if (!strcmp(key, keys[i])) return 1;
    return 0;
}

int sleela_vm_config_save(const char *path, const SLEELA_VM_CONFIG *config) {
    FILE *in, *out;
    char line[512];
    char tmp[1024];
    int preserved_any = 0;
    if (!path || !config) return -1;
    if (sleela_vm_config_validate(config) != 0) return -2;
    if ((size_t)snprintf(tmp, sizeof(tmp), "%s.tmp", path) >= sizeof(tmp)) return -3;

    out = fopen(tmp, "wb");
    if (!out) return -4;

    /* Canonical vm.* block first, always well-formed. */
    if (fprintf(out,
            "# SLeeLa VM selection (managed by `sleela vm lock`/`unlock`).\n"
            "# Valid generations: 1 through %d; default generation is %d.\n"
            "vm.version = %d\n"
            "vm.profile = %s\n"
            "vm.strict = %s\n"
            "# vm.locked: when true the version above is PINNED; the CLI refuses\n"
            "# any other generation. Set with `sleela vm lock`, clear with unlock.\n"
            "vm.locked = %s\n",
            SLEELA_VM_MAX_VERSION, SLEELA_VM_DEFAULT_VERSION,
            config->version, config->profile,
            config->strict ? "true" : "false",
            config->locked ? "true" : "false") < 0) {
        fclose(out); remove(tmp); return -5;
    }

    /* Preserve every other line from the existing file (e.g. the java.* block),
     * dropping only the managed vm.* keys we just re-emitted. */
    in = fopen(path, "rb");
    if (in) {
        while (fgets(line, sizeof(line), in)) {
            if (is_managed_vm_key_line(line)) continue;
            if (!preserved_any) { fputc('\n', out); preserved_any = 1; }
            if (fputs(line, out) == EOF) { fclose(in); fclose(out); remove(tmp); return -6; }
        }
        fclose(in);
    }

    if (fclose(out) != 0) { remove(tmp); return -7; }
    if (rename(tmp, path) != 0) { remove(tmp); return -8; }
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
    if (config->strict != 0 && config->strict != 1) return -3;
    return (config->locked == 0 || config->locked == 1) ? 0 : -4;
}
