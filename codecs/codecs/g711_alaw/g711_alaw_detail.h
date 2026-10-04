#ifndef SLEELA_CODEC_G711_ALAW_DETAIL_H
#define SLEELA_CODEC_G711_ALAW_DETAIL_H

/* A-law-specific detail (C++), declared with C linkage for g711_alaw.c. */

#ifdef __cplusplus
extern "C" {
#endif

/* A-law is the European G.711 variant; μ-law is North-American/Japanese. */
int sleela_g711_alaw_is_european(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_CODEC_G711_ALAW_DETAIL_H */
