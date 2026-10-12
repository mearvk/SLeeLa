#if !defined(_WIN32)
#define _POSIX_C_SOURCE 200809L
#endif
#include "config_location.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
static int set_env(const char *k, const char *v) { return SetEnvironmentVariableA(k, v) ? 0 : 1; }
#else
static int set_env(const char *k, const char *v) { return v ? setenv(k, v, 1) : unsetenv(k); }
#endif
#define CHECK(c, n) do { if (!(c)) { fprintf(stderr, "config location smoke failed: %d\n", (n)); return (n); } } while (0)
int main(void) {
    SLEELA_CONFIG_LOCATION c;
    char long_path[SLEELA_CONFIG_LOCATION_MAX + 8];
    sleela_config_location_defaults(&c);
    CHECK(sleela_config_location_load_policy("config/config-location.conf", &c), 1);
    CHECK(strcmp(c.relative_path, ".sleela/config") == 0, 2);
    CHECK(sleela_config_location_resolve(&c, "/tmp/SLeeLa", NULL, NULL), 3);
    CHECK(strcmp(sleela_config_location_root(&c), "/tmp/SLeeLa/.sleela/config") == 0, 4);
    CHECK(c.source == 2, 5);
    sleela_config_location_defaults(&c);
    CHECK(sleela_config_location_resolve(&c, NULL, "/opt/sleela", NULL), 6);
    CHECK(strcmp(sleela_config_location_root(&c), "/opt/sleela/Config") == 0, 7);
    CHECK(sleela_config_location_resolve(&c, NULL, NULL, "/var/lib/sleela/config"), 8);
    CHECK(strcmp(sleela_config_location_root(&c), "/var/lib/sleela/config") == 0, 9);
    CHECK(!sleela_config_location_resolve(&c, NULL, NULL, "relative/config"), 10);
    sleela_config_location_defaults(&c);
    CHECK(set_env("SLEELA_CONFIG_ROOT", "/tmp/sleela-env-config") == 0, 11);
    CHECK(sleela_config_location_from_environment(&c), 12);
    CHECK(sleela_config_location_resolve(&c, "/tmp/ignored", NULL, NULL), 13);
    CHECK(strcmp(sleela_config_location_root(&c), "/tmp/sleela-env-config") == 0, 14);
    CHECK(set_env("SLEELA_CONFIG_ROOT", NULL) == 0, 15);
    sleela_config_location_defaults(&c);
    memset(long_path, 'x', sizeof(long_path) - 1);
    long_path[sizeof(long_path) - 1] = '\0';
    CHECK(!sleela_config_location_resolve(&c, long_path, NULL, NULL), 16);
    puts("config location smoke: PASS");
    return 0;
}
