/*
 * ballum/include/sleela_ballum/ballum_host_profiles.h
 * Starter profiles for the Ballum program host: ready-made, reviewed launch
 * descriptions so a terminal or admin tool can be put into the Ballum
 * environment by name. A profile is data: a title (for the APP NAME slot), the
 * program argv, and the entry/exit transitions that suit it. Programs are
 * resolved against the trusted search path and run sandboxed by default.
 */
#ifndef SLEELA_BALLUM_HOST_PROFILES_H
#define SLEELA_BALLUM_HOST_PROFILES_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char        *name;         /* short id, e.g. "sleela-terminal"        */
    const char        *title;        /* what the APP NAME component shows       */
    const char        *description;
    const char *const *argv;         /* NULL-terminated; argv[0] resolved safely */
    const char        *enter;        /* transition name, see ballum_transition.h */
    const char        *leave;
} ballum_profile_t;

size_t                  ballum_profile_count(void);
const ballum_profile_t *ballum_profile_at(size_t index);        /* NULL if out of range */
const ballum_profile_t *ballum_profile_find(const char *name);  /* NULL if unknown      */

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_BALLUM_HOST_PROFILES_H */
