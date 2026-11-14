/* conv_model.c -- see conv_model.h. Plain C99, no dependencies. */
#include "conv_model.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *dup_range(const char *s, size_t n) {
    char *p = (char *)malloc(n + 1);
    if (!p) return NULL;
    memcpy(p, s, n);
    p[n] = '\0';
    return p;
}

/* Fixed field counts (excluding kind). The last field takes the rest of line. */
static int fields_for(const char *kind) {
    static const struct { const char *k; int n; } t[] = {
        {"model", 2},   {"type", 4},    {"box", 2},      {"promote", 2},
        {"widen", 2},   {"compound", 2}, {"binop", 3},   {"unop", 3},
        {"cast", 3},    {"incr", 2},    {"ret", 3},      {"field", 3},
        {"call", 2},    {"unsupported", 2}, {"tostr", 2},
        {"chartext", 3},
        {"qualify", 1},
    };
    size_t i;
    for (i = 0; i < sizeof t / sizeof t[0]; i++)
        if (strcmp(t[i].k, kind) == 0) return t[i].n;
    return -1;
}

static int push(cm_model *m, cm_rec *r) {
    if (m->n == m->cap) {
        size_t nc = m->cap ? m->cap * 2 : 256;
        cm_rec *p = (cm_rec *)realloc(m->recs, nc * sizeof *p);
        if (!p) return -1;
        m->recs = p;
        m->cap = nc;
    }
    m->recs[m->n++] = *r;
    return 0;
}

static int fail(char *err, size_t n, const char *path, int line, const char *msg) {
    snprintf(err, n, "%s:%d: %s", path, line, msg);
    return -1;
}

int cm_load(const char *path, cm_model *m, char *err, size_t errlen) {
    FILE *fp;
    char *buf = NULL;
    size_t cap = 0;
    int line = 0;
    int c;
    size_t len = 0;

    memset(m, 0, sizeof *m);
    fp = fopen(path, "rb");
    if (!fp) {
        snprintf(err, errlen, "cannot open conversion model '%s'", path);
        return -1;
    }
    for (;;) {
        c = fgetc(fp);
        if (c == EOF || c == '\n') {
            cm_rec r;
            char *s, *e;
            int want, i;
            line++;
            if (len > 0 && buf[len - 1] == '\r') len--;
            if (buf) buf[len] = '\0';
            s = buf ? buf : (char *)"";
            while (*s && isspace((unsigned char)*s)) s++;
            if (*s && *s != '#') {
                memset(&r, 0, sizeof r);
                r.line = line;
                e = s;
                while (*e && !isspace((unsigned char)*e)) e++;
                r.kind = dup_range(s, (size_t)(e - s));
                want = fields_for(r.kind);
                if (want < 0) {
                    fclose(fp); free(buf);
                    return fail(err, errlen, path, line, "unknown record kind");
                }
                for (i = 0; i < want; i++) {
                    char *b;
                    while (*e && isspace((unsigned char)*e)) e++;
                    b = e;
                    if (i == want - 1) {
                        /* the last field keeps the rest of the line, trimmed */
                        size_t n = strlen(b);
                        while (n > 0 && isspace((unsigned char)b[n - 1])) n--;
                        if (n == 0 && strcmp(r.kind, "compound") != 0 &&
                            strcmp(r.kind, "widen") != 0 && strcmp(r.kind, "promote") != 0 &&
                            strcmp(r.kind, "box") != 0 && strcmp(r.kind, "model") != 0) {
                            fclose(fp); free(buf);
                            return fail(err, errlen, path, line, "record is missing a field");
                        }
                        r.f[i] = dup_range(b, n);
                        e = b + strlen(b);
                    } else {
                        while (*e && !isspace((unsigned char)*e)) e++;
                        if (e == b) {
                            fclose(fp); free(buf);
                            return fail(err, errlen, path, line, "record is missing a field");
                        }
                        r.f[i] = dup_range(b, (size_t)(e - b));
                    }
                }
                r.nf = want;
                if (push(m, &r) != 0) {
                    fclose(fp); free(buf);
                    return fail(err, errlen, path, line, "out of memory");
                }
                if (strcmp(r.kind, "model") == 0) {
                    m->name = dup_range(r.f[0], strlen(r.f[0]));
                    m->rev  = dup_range(r.f[1], strlen(r.f[1]));
                }
            }
            len = 0;
            if (c == EOF) break;
            continue;
        }
        if (len + 2 > cap) {
            size_t nc = cap ? cap * 2 : 256;
            char *p = (char *)realloc(buf, nc);
            if (!p) { fclose(fp); free(buf); return fail(err, errlen, path, line, "out of memory"); }
            buf = p; cap = nc;
        }
        buf[len++] = (char)c;
    }
    fclose(fp);
    free(buf);
    if (!m->name) {
        snprintf(err, errlen, "%s: no 'model' record", path);
        return -1;
    }
    return 0;
}

void cm_free(cm_model *m) {
    size_t i;
    int j;
    for (i = 0; i < m->n; i++) {
        free(m->recs[i].kind);
        for (j = 0; j < m->recs[i].nf; j++) free(m->recs[i].f[j]);
    }
    free(m->recs);
    free(m->name);
    free(m->rev);
    memset(m, 0, sizeof *m);
}

const cm_rec *cm_find(const cm_model *m, const char *kind,
                      const char *a, const char *b, const char *c) {
    size_t i;
    for (i = 0; i < m->n; i++) {
        const cm_rec *r = &m->recs[i];
        if (strcmp(r->kind, kind) != 0) continue;
        if (a && (r->nf < 1 || strcmp(r->f[0], a) != 0)) continue;
        if (b && (r->nf < 2 || strcmp(r->f[1], b) != 0)) continue;
        if (c && (r->nf < 3 || strcmp(r->f[2], c) != 0)) continue;
        return r;
    }
    return NULL;
}

size_t cm_count(const cm_model *m, const char *kind) {
    size_t i, n = 0;
    for (i = 0; i < m->n; i++)
        if (strcmp(m->recs[i].kind, kind) == 0) n++;
    return n;
}
