#ifndef SLEELA_DATA_STRUCTURES_H
#define SLEELA_DATA_STRUCTURES_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SLDS_NULL = 0,
    SLDS_INT,
    SLDS_DOUBLE,
    SLDS_BOOL,
    SLDS_STRING
} SLDSKind;

typedef struct {
    SLDSKind kind;
    union {
        int64_t i;
        double d;
        int b;
        const char *s;
    } as;
} SLDSValue;

typedef struct {
    SLDSValue *items;
    size_t size;
    size_t capacity;
} SLDSVector;

typedef struct {
    SLDSVector values;
} SLDSStack;

typedef struct {
    SLDSValue *items;
    size_t head;
    size_t size;
    size_t capacity;
} SLDSQueue;

typedef struct {
    char **keys;
    SLDSValue *values;
    size_t size;
    size_t capacity;
} SLDSMap;

typedef struct {
    char *name;
    SLDSMap fields;
} SLDSObject;

void slds_vector_init(SLDSVector *v);
void slds_vector_free(SLDSVector *v);
int slds_vector_push(SLDSVector *v, SLDSValue value);
int slds_vector_get(const SLDSVector *v, size_t index, SLDSValue *out);

void slds_stack_init(SLDSStack *s);
void slds_stack_free(SLDSStack *s);
int slds_stack_push(SLDSStack *s, SLDSValue value);
int slds_stack_pop(SLDSStack *s, SLDSValue *out);
int slds_stack_peek(const SLDSStack *s, SLDSValue *out);

void slds_queue_init(SLDSQueue *q);
void slds_queue_free(SLDSQueue *q);
int slds_queue_enqueue(SLDSQueue *q, SLDSValue value);
int slds_queue_dequeue(SLDSQueue *q, SLDSValue *out);
int slds_queue_front(const SLDSQueue *q, SLDSValue *out);

void slds_map_init(SLDSMap *m);
void slds_map_free(SLDSMap *m);
int slds_map_put(SLDSMap *m, const char *key, SLDSValue value);
int slds_map_get(const SLDSMap *m, const char *key, SLDSValue *out);
int slds_map_contains(const SLDSMap *m, const char *key);

void slds_object_init(SLDSObject *o, const char *name);
void slds_object_free(SLDSObject *o);
int slds_object_set(SLDSObject *o, const char *field, SLDSValue value);
int slds_object_get(const SLDSObject *o, const char *field, SLDSValue *out);

#ifdef __cplusplus
}
#endif

#endif
