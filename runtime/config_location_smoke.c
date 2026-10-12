#if !defined(_WIN32)
#define _POSIX_C_SOURCE 200809L
#endif
#include "config_location.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#define PROJECT_ROOT "C:\\temp\\SLeeLa"
#define PROJECT_EXPECTED "C:\\temp\\SLeeLa\\.sleela\\config"
#define INSTALL_ROOT "C:\\opt\\sleela"
#define INSTALL_EXPECTED "C:\\opt\\sleela\\Config"
#define EXPLICIT_ROOT "C:\\var\\lib\\sleela\\config"
#define ENV_ROOT "C:\\temp\\sleela-env-config"
static int set_env(const char *k, const char *v) { return SetEnvironmentVariableA(k, v) ? 0 : 1; }
#else
#define PROJECT_ROOT "/tmp/SLeeLa"
#define PROJECT_EXPECTED "/tmp/SLeeLa/.sleela/config"
#define INSTALL_ROOT "/opt/sleela"
#define INSTALL_EXPECTED "/opt/sleela/Config"
#define EXPLICIT_ROOT "/var/lib/sleela/config"
#define ENV_ROOT "/tmp/sleela-env-config"
static int set_env(const char *k, const char *v) { return v ? setenv(k, v, 1) : unsetenv(k); }
#endif
#define CHECK(c, n) do { if (!(c)) { fprintf(stderr, "config location smoke failed: %d\n", (n)); return (n); } } while (0)
int main(void) {
    SLEELA_CONFIG_LOCATION c;
    char long_path[SLEELA_CONFIG_LOCATION_MAX + 8];
    sleela_config_location_defaults(&c);
    CHECK(sleela_config_location_load_policy("config/config-location.conf", &c), 1);
    CHECK(strcmp(c.relative_path, ".sleela/config") == 0, 2);
    CHECK(sleela_config_location_resolve(&c, PROJECT_ROOT, NULL, NULL), 3);
    CHECK(strcmp(sleela_config_location_root(&c), PROJECT_EXPECTED) == 0, 4);
    CHECK(c.source == 2, 5);
    sleela_config_location_defaults(&c);
    CHECK(sleela_config_location_resolve(&c, NULL, INSTALL_ROOT, NULL), 6);
    CHECK(strcmp(sleela_config_location_root(&c), INSTALL_EXPECTED) == 0, 7);
    CHECK(sleela_config_location_resolve(&c, NULL, NULL, EXPLICIT_ROOT), 8);
    CHECK(strcmp(sleela_config_location_root(&c), EXPLICIT_ROOT) == 0, 9);
    CHECK(!sleela_config_location_resolve(&c, NULL, NULL, "relative/config"), 10);
    sleela_config_location_defaults(&c);
    CHECK(set_env("SLEELA_CONFIG_ROOT", ENV_ROOT) == 0, 11);
    CHECK(sleela_config_location_from_environment(&c), 12);
    CHECK(sleela_config_location_resolve(&c, PROJECT_ROOT, NULL, NULL), 13);
    CHECK(strcmp(sleela_config_location_root(&c), ENV_ROOT) == 0, 14);
    CHECK(set_env("SLEELA_CONFIG_ROOT", NULL) == 0, 15);
    sleela_config_location_defaults(&c);
    CHECK(set_env("SLEELA_PROJECT_ROOT", PROJECT_ROOT) == 0, 16);
    CHECK(set_env("SLEELA_INSTALL_ROOT", NULL) == 0, 17);
    CHECK(sleela_config_location_from_environment(&c), 18);
    CHECK(sleela_config_location_resolve(&c, NULL, NULL, NULL), 19);
    CHECK(strcmp(sleela_config_location_root(&c), PROJECT_EXPECTED) == 0, 20);
    CHECK(set_env("SLEELA_PROJECT_ROOT", NULL) == 0, 21);
    CHECK(set_env("SLEELA_INSTALL_ROOT", INSTALL_ROOT) == 0, 22);
    sleela_config_location_defaults(&c);
    CHECK(sleela_config_location_from_environment(&c), 23);
    CHECK(sleela_config_location_resolve(&c, NULL, NULL, NULL), 24);
    CHECK(strcmp(sleela_config_location_root(&c), INSTALL_EXPECTED) == 0, 25);
    CHECK(set_env("SLEELA_INSTALL_ROOT", NULL) == 0, 26);
    memset(long_path, 'x', sizeof(long_path) - 1);
#ifdef _WIN32
    long_path[0] = 'C'; long_path[1] = ':'; long_path[2] = '\\';
#else
    long_path[0] = '/';
#endif
    long_path[sizeof(long_path) - 1] = '\0';
    sleela_config_location_defaults(&c);
    CHECK(!sleela_config_location_resolve(&c, long_path, NULL, NULL), 27);
    puts("config location smoke: PASS");
    return 0;
}
