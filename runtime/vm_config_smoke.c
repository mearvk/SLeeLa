#if !defined(_WIN32)
#define _POSIX_C_SOURCE 200809L
#endif
#include "vm_config.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
static int set_env(const char *k, const char *v) { return SetEnvironmentVariableA(k, v) ? 0 : 1; }
#else
static int set_env(const char *k, const char *v) { return v ? setenv(k, v, 1) : unsetenv(k); }
#endif
#define CHECK(c, n) do { if (!(c)) { fprintf(stderr, "VM config smoke failed: %d\n", (n)); return (n); } } while (0)
int main(void) {
    SLEELA_VM_CONFIG c;
    int load_result;
    sleela_vm_config_defaults(&c);
    CHECK(c.version == 11 && c.strict, 1);
    CHECK(sleela_vm_config_set_version(&c, 1) == 0 && c.version == 1, 2);
    CHECK(sleela_vm_config_set_version(&c, 12) != 0, 3);
    load_result = sleela_vm_config_load("config/vm.conf", &c);
    if (load_result != 0) {
        fprintf(stderr, "VM config smoke failed: 4 (load returned %d; expected config/vm.conf from repository root)\n", load_result);
        return 4;
    }
    if (c.version != 11) {
        fprintf(stderr, "VM config smoke failed: 4 (loaded version %d; expected 11)\n", c.version);
        return 4;
    }
    CHECK(set_env("SLEELA_VM_VERSION", "7") == 0, 5);
    CHECK(sleela_vm_config_apply_environment(&c) == 0 && c.version == 7, 6);
    CHECK(set_env("SLEELA_VM_VERSION", "99999999999999999999999999999999") == 0, 7);
    CHECK(sleela_vm_config_apply_environment(&c) != 0, 8);
    CHECK(set_env("SLEELA_VM_VERSION", "11x") == 0, 9);
    CHECK(sleela_vm_config_apply_environment(&c) != 0, 10);
    CHECK(set_env("SLEELA_VM_VERSION", NULL) == 0, 11);

    /* Lock semantics: default is unlocked; validation accepts 0/1; a save/load
     * round-trip persists a pinned (locked) version and preserves it. */
    sleela_vm_config_defaults(&c);
    CHECK(c.locked == 0, 12);
    c.locked = 1; c.version = 11;
    CHECK(sleela_vm_config_validate(&c) == 0, 13);
    c.locked = 5;
    CHECK(sleela_vm_config_validate(&c) != 0, 14);
    {
        const char *tmp = "vm_config_smoke_lock.tmp.conf";
        SLEELA_VM_CONFIG w, r;
        sleela_vm_config_defaults(&w);
        w.version = 11; w.locked = 1; w.strict = 1;
        CHECK(sleela_vm_config_save(tmp, &w) == 0, 15);
        sleela_vm_config_defaults(&r);
        CHECK(sleela_vm_config_load(tmp, &r) == 0, 16);
        CHECK(r.version == 11 && r.locked == 1, 17);
        remove(tmp);
    }

    puts("VM CONFIG: PASS");
    return 0;
}
