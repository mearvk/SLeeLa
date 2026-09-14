#include "data_structures.h"
#include <stdlib.h>
#include <string.h>

static int grow_values(SLDSValue **items, size_t *capacity, size_t need) {
    if (*capacity >= need) return 1;
    size_t c = *capacity ? *capacity : 8;
    while (c < need) c *= 2;
    SLDSValue *p = (SLDSValue *)realloc(*items, c * sizeof(*p));
    if (!p) return 0;
    *items = p; *capacity = c; return 1;
}

void slds_vector_init(SLDSVector *v) { memset(v, 0, sizeof(*v)); }
void slds_vector_free(SLDSVector *v) { free(v->items); memset(v, 0, sizeof(*v)); }

int slds_vector_push(SLDSVector *v, SLDSValue value) {
    if (!grow_values(&v->items, &v->capacity, v->size + 1)) return 0;
    v->items[v->size++] = value; return 1;
}
int slds_vector_get(const SLDSVector *v, size_t index, SLDSValue *out) {
    if (!out || index >= v->size) return 0; *out = v->items[index]; return 1;
}

void slds_stack_init(SLDSStack *s) { slds_vector_init(&s->values); }
void slds_stack_free(SLDSStack *s) { slds_vector_free(&s->values); }
int slds_stack_push(SLDSStack *s, SLDSValue value) { return slds_vector_push(&s->values, value); }
int slds_stack_pop(SLDSStack *s, SLDSValue *out) {
    if (!s->values.size || !out) return 0;
    *out = s->values.items[--s->values.size]; return 1;
}
int slds_stack_peek(const SLDSStack *s, SLDSValue *out) {
    if (!s->values.size || !out) return 0;
    *out = s->values.items[s->values.size - 1]; return 1;
}

void slds_queue_init(SLDSQueue *q) { memset(q, 0, sizeof(*q)); }
void slds_queue_free(SLDSQueue *q) { free(q->items); memset(q, 0, sizeof(*q)); }
int slds_queue_enqueue(SLDSQueue *q, SLDSValue value) {
    if (!grow_values(&q->items, &q->capacity, q->size + 1)) return 0;
    size_t tail = (q->head + q->size) % q->capacity;
    q->items[tail] = value; q->size++; return 1;
}
int slds_queue_dequeue(SLDSQueue *q, SLDSValue *out) {
    if (!q->size || !out) return 0;
    *out = q->items[q->head]; q->head = (q->head + 1) % q->capacity; q->size--; return 1;
}
int slds_queue_front(const SLDSQueue *q, SLDSValue *out) {
    if (!q->size || !out) return 0; *out = q->items[q->head]; return 1;
}

static char *dupstr(const char *s) {
    if (!s) return NULL; size_t n = strlen(s) + 1; char *p = (char *)malloc(n);
    if (p) memcpy(p, s, n); return p;
}
void slds_map_init(SLDSMap *m) { memset(m, 0, sizeof(*m)); }
void slds_map_free(SLDSMap *m) {
    for (size_t i = 0; i < m->size; ++i) free(m->keys[i]);
    free(m->keys); free(m->values); memset(m, 0, sizeof(*m));
}
static int map_grow(SLDSMap *m) {
    if (m->capacity >= m->size + 1) return 1;
    size_t c = m->capacity ? m->capacity * 2 : 8;
    char **keys = (char **)realloc(m->keys, c * sizeof(*keys));
    SLDSValue *values = (SLDSValue *)realloc(m->values, c * sizeof(*values));
    if (!keys || !values) return 0; m->keys = keys; m->values = values; m->capacity = c; return 1;
}
int slds_map_put(SLDSMap *m, const char *key, SLDSValue value) {
    for (size_t i=0;i<m->size;i++) if (!strcmp(m->keys[i], key)) { m->values[i]=value; return 1; }
    if (!map_grow(m)) return 0; m->keys[m->size]=dupstr(key); if (!m->keys[m->size]) return 0;
    m->values[m->size++]=value; return 1;
}
int slds_map_get(const SLDSMap *m, const char *key, SLDSValue *out) {
    if (!out) return 0; for(size_t i=0;i<m->size;i++) if(!strcmp(m->keys[i],key)){*out=m->values[i];return 1;} return 0;
}
int slds_map_contains(const SLDSMap *m, const char *key) { SLDSValue v; return slds_map_get(m,key,&v); }

void slds_object_init(SLDSObject *o, const char *name) { o->name=dupstr(name); slds_map_init(&o->fields); }
void slds_object_free(SLDSObject *o) { free(o->name); slds_map_free(&o->fields); o->name=NULL; }
int slds_object_set(SLDSObject *o, const char *field, SLDSValue value) { return slds_map_put(&o->fields, field, value); }
int slds_object_get(const SLDSObject *o, const char *field, SLDSValue *out) { return slds_map_get(&o->fields, field, out); }
