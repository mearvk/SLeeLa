#ifndef SLEELA_CONFIG_LOCATION_H
#define SLEELA_CONFIG_LOCATION_H
#ifdef __cplusplus
extern "C" {
#endif
#define SLEELA_CONFIG_LOCATION_MAX 1024
#define SLEELA_CONFIG_RELATIVE_MAX 256
#define SLEELA_CONFIG_INSTALL_MAX 64
typedef struct {
    char project_name[64];
    char project_root[SLEELA_CONFIG_LOCATION_MAX];
    char install_root[SLEELA_CONFIG_LOCATION_MAX];
    char config_root[SLEELA_CONFIG_LOCATION_MAX];
    char absolute_root[SLEELA_CONFIG_LOCATION_MAX];
    char relative_path[SLEELA_CONFIG_RELATIVE_MAX];
    char install_relative[SLEELA_CONFIG_INSTALL_MAX];
    int source; /* 1=explicit/env, 2=project, 3=install, 4=system absolute */
} SLEELA_CONFIG_LOCATION;
void sleela_config_location_defaults(SLEELA_CONFIG_LOCATION *);
int sleela_config_location_load_policy(const char *, SLEELA_CONFIG_LOCATION *);
int sleela_config_location_from_environment(SLEELA_CONFIG_LOCATION *);
int sleela_config_location_resolve(SLEELA_CONFIG_LOCATION *, const char *, const char *, const char *);
const char *sleela_config_location_root(const SLEELA_CONFIG_LOCATION *);
#ifdef __cplusplus
}
#endif
#endif
