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
    sleela_vm_config_defaults(&c);
    CHECK(c.version == 11 && c.strict, 1);
    CHECK(sleela_vm_config_set_version(&c, 1) == 0 && c.version == 1, 2);
    CHECK(sleela_vm_config_set_version(&c, 12) != 0, 3);
    CHECK(sleela_vm_config_load("config/vm.conf", &c) == 0 && c.version == 11, 4);
    CHECK(set_env("SLEELA_VM_VERSION", "7") == 0, 5);
    CHECK(sleela_vm_config_apply_environment(&c) == 0 && c.version == 7, 6);
    CHECK(set_env("SLEELA_VM_VERSION", "4294967297") == 0, 7);
    CHECK(sleela_vm_config_apply_environment(&c) != 0, 8);
    CHECK(set_env("SLEELA_VM_VERSION", NULL) == 0, 9);
    puts("VM CONFIG: PASS");
    return 0;
}
