#include "sleela_registry_v3.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static slr3_status load_file(const char *path, slr3_registry **out) {
    slr3_registry *r = slr3_registry_create("00000000000000000000000000000000", 4096);
    slr3_status s;
    if (!r) return SLR3_IO;
    s = slr3_registry_load_v2(r, path);
    if (s != SLR3_OK) { slr3_registry_destroy(r); return s; }
    *out = r;
    return SLR3_OK;
}
static int parse_u64(const char *s, uint64_t *out) {
    char *end = NULL;
    unsigned long long value;
    errno = 0;
    value = strtoull(s, &end, 10);
    if (errno || !end || end == s || *end) return 0;
    *out = (uint64_t)value;
    return 1;
}
static int finish(slr3_registry *r, const char *path, slr3_status s) {
    if (s == SLR3_OK) s = slr3_registry_save_v2(r, path);
    if (s != SLR3_OK) fprintf(stderr, "registry operation failed: %s\n", slr3_status_name(s));
    else printf("registry saved: %s\n", path);
    slr3_registry_destroy(r);
    return s == SLR3_OK ? 0 : 1;
}
static void usage(void) {
    fputs("usage:\n"
          "  sleela-registry3 init FILE REGISTRY_ID\n"
          "  sleela-registry3 migrate-v1 INPUT OUTPUT REGISTRY_ID\n"
          "  sleela-registry3 add FILE ID NAMESPACE KIND NAME\n"
          "  sleela-registry3 alias FILE ID ALIAS\n"
          "  sleela-registry3 relate FILE SOURCE_ID TYPE TARGET_ID\n"
          "  sleela-registry3 rename FILE ID EXPECTED_REVISION NEW_NAME\n"
          "  sleela-registry3 transition FILE ID EXPECTED_REVISION NEXT_STATE\n"
          "  sleela-registry3 modium-register FILE ID NAME REGION_KIND LOCATOR BYTES OWNER FORMAT SEAL\n"
          "  sleela-registry3 identify FILE NAMESPACE NAME_OR_ALIAS\n"
          "  sleela-registry3 describe FILE RECORD_ID\n"
          "  sleela-registry3 review FILE\n"
          "  sleela-registry3 verify FILE\n", stderr);
}
int main(int argc, char **argv) {
    slr3_status s;
    slr3_registry *r = NULL;
    char report[65536];
    uint64_t revision, bytes;

    if (argc == 4 && !strcmp(argv[1], "init")) {
        r = slr3_registry_create(argv[3], 4096);
        if (!r) return 2;
        return finish(r, argv[2], SLR3_OK);
    }
    if (argc == 5 && !strcmp(argv[1], "migrate-v1")) {
        s = slr3_registry_migrate_v1(argv[2], argv[3], argv[4], 4096);
        if (s != SLR3_OK) { fprintf(stderr, "migration failed: %s\n", slr3_status_name(s)); return 1; }
        printf("migrated %s -> %s (SLREG 1 -> SLREG 2)\n", argv[2], argv[3]);
        return 0;
    }
    if (argc == 5 && !strcmp(argv[1], "identify")) {
        const slr3_record *record;
        s = load_file(argv[2], &r);
        if (s != SLR3_OK) goto failed;
        record = slr3_registry_identify(r, argv[3], argv[4]);
        if (!record) { fprintf(stderr, "record not found in namespace %s\\n", argv[3]); slr3_registry_destroy(r); return 1; }
        printf("%s %s %s %s\\n", record->id, record->kind, record->state, record->name);
        slr3_registry_destroy(r);
        return 0;
    }
    if (argc == 4 && !strcmp(argv[1], "describe")) {
        const slr3_record *record;
        s = load_file(argv[2], &r);
        if (s != SLR3_OK) goto failed;
        record = slr3_registry_get(r, argv[3]);
        if (!record) { fprintf(stderr, "record not found: %s\\n", argv[3]); slr3_registry_destroy(r); return 1; }
        printf("id=%s\\nnamespace=%s\\nkind=%s\\nname=%s\\nstate=%s\\nrevision=%llu\\nlocator=%s\\nintegrity=%s\\n",
               record->id, record->name_space, record->kind, record->name, record->state,
               (unsigned long long)record->revision, record->locator, record->integrity_status);
        slr3_registry_destroy(r);
        return 0;
    }
    if (argc == 3 && (!strcmp(argv[1], "review") || !strcmp(argv[1], "verify"))) {
        s = load_file(argv[2], &r);
        if (s != SLR3_OK) { fprintf(stderr, "registry invalid: %s\n", slr3_status_name(s)); return 1; }
        if (!strcmp(argv[1], "review")) {
            s = slr3_registry_review(r, report, sizeof report);
            if (s == SLR3_OK) fputs(report, stdout);
        } else {
            printf("SLREG 2 structure verified: %zu records; id=%s\n",
                   slr3_registry_count(r), slr3_registry_id(r));
        }
        slr3_registry_destroy(r);
        return s == SLR3_OK ? 0 : 1;
    }
    if (argc >= 2 && !strcmp(argv[1], "add") && argc == 7) {
        slr3_record record;
        s = load_file(argv[2], &r); if (s != SLR3_OK) goto failed;
        slr3_record_init(&record, argv[3], argv[4], argv[5], argv[6]);
        return finish(r, argv[2], slr3_registry_add(r, &record));
    }
    if (argc == 5 && !strcmp(argv[1], "alias")) {
        s = load_file(argv[2], &r); if (s != SLR3_OK) goto failed;
        return finish(r, argv[2], slr3_registry_add_alias(r, argv[3], argv[4]));
    }
    if (argc == 6 && !strcmp(argv[1], "relate")) {
        s = load_file(argv[2], &r); if (s != SLR3_OK) goto failed;
        return finish(r, argv[2], slr3_registry_relate(r, argv[3], argv[4], argv[5]));
    }
    if (argc == 6 && !strcmp(argv[1], "rename")) {
        if (!parse_u64(argv[4], &revision)) { fputs("invalid revision\n", stderr); return 2; }
        s = load_file(argv[2], &r); if (s != SLR3_OK) goto failed;
        return finish(r, argv[2], slr3_registry_rename(r, argv[3], argv[5], revision));
    }
    if (argc == 6 && !strcmp(argv[1], "transition")) {
        if (!parse_u64(argv[4], &revision)) { fputs("invalid revision\n", stderr); return 2; }
        s = load_file(argv[2], &r); if (s != SLR3_OK) goto failed;
        return finish(r, argv[2], slr3_registry_transition(r, argv[3], argv[5], revision));
    }
    if (argc == 11 && !strcmp(argv[1], "modium-register")) {
        if (!parse_u64(argv[7], &bytes)) { fputs("invalid byte capacity\n", stderr); return 2; }
        s = load_file(argv[2], &r); if (s != SLR3_OK) goto failed;
        return finish(r, argv[2], slr3_modium_region_register(
            r, argv[3], argv[4], argv[5], argv[6], bytes, argv[8], argv[9], argv[10]));
    }
    usage();
    return 2;

failed:
    fprintf(stderr, "cannot load registry: %s\n", slr3_status_name(s));
    return 1;
}
