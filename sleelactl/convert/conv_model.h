/* conv_model.h -- loader for the Java -> SLeeLa conversion subject model.
 *
 * The model file (impl/subjects/conversion/conversion.model) is the single
 * source of Java semantic knowledge. This C module only parses and indexes it;
 * it holds no Java rules of its own.
 *
 * Record shape: kind + up to CM_MAXF whitespace-separated fields; the final
 * field of a record keeps the rest of the line (templates contain spaces).
 */
#ifndef SLEELACTL_CONV_MODEL_H
#define SLEELACTL_CONV_MODEL_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CM_MAXF 4

typedef struct cm_rec {
    char *kind;
    char *f[CM_MAXF];
    int   nf;
    int   line;
} cm_rec;

typedef struct cm_model {
    cm_rec *recs;
    size_t  n, cap;
    char   *name;
    char   *rev;
} cm_model;

/* Returns 0 on success; on failure writes a message to err and returns -1. */
int  cm_load(const char *path, cm_model *m, char *err, size_t errlen);
void cm_free(cm_model *m);

/* First record of `kind` whose leading fields equal a (and b, c when non-NULL). */
const cm_rec *cm_find(const cm_model *m, const char *kind,
                      const char *a, const char *b, const char *c);

/* Number of records of a kind (for self-checks and reporting). */
size_t cm_count(const cm_model *m, const char *kind);

#ifdef __cplusplus
}
#endif
#endif
