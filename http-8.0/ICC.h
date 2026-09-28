#ifndef SLEELA_ICC_H
#define SLEELA_ICC_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Project-level ICC model.
 * This does not grant the International Criminal Court permission to use
 * U.S. government emblems and does not represent an official U.S. or ICC API.
 */
bool sleela_icc_us_emblem_use_allowed(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_ICC_H */
