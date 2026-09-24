#ifndef SKYA_SLEEELA_BRIDGE_H
#define SKYA_SLEEELA_BRIDGE_H
#include "skya_policy.h"
#ifdef __cplusplus
extern "C" {
#endif
int skya_sleela_runtime_probe(void);
int skya_sleela_initial_policy(skya_policy_t *policy);
int skya_sleela_command(int argc, char **argv);
#ifdef __cplusplus
}
#endif
#endif
