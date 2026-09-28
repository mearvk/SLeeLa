#ifndef SLEELA_MUNCTION_H
#define SLEELA_MUNCTION_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLEELA_MUNCTION_MIN_CALLS 4
#define SLEELA_MUNCTION_MAX_CALLS 16
#define SLEELA_MUNCTION_MAX_INTERIMS 256
#define SLEELA_MUNCTION_MAX_NAME 128
#define SLEELA_MUNCTION_MAX_ADDRESS 256
#define SLEELA_MUNCTION_MAX_RECEIPT 2048
typedef enum { SLEELA_MUNCTION_STARTED=0, SLEELA_MUNCTION_CONNECTED, SLEELA_MUNCTION_MOVING, SLEELA_MUNCTION_CLOSED } sleela_munction_phase;
typedef enum { SLEELA_MUNCTION_REACHED=0, SLEELA_MUNCTION_CONTAINED, SLEELA_MUNCTION_ABORTED } sleela_munction_outcome;
typedef struct { uint64_t offered, acknowledged, digest; int coherent; } sleela_munction_send;
typedef struct { const uint8_t *data; size_t length; uint64_t digest, sequence; int present; } sleela_munction_reception;
typedef struct {
    const char *scheme;
    int (*open)(void *ctx, const char *address);
    int64_t (*send)(void *ctx, const uint8_t *data, size_t length);
    int64_t (*consume)(void *ctx, uint8_t *out, size_t capacity, uint64_t *sequence);
    int (*thatch)(void *ctx, const char *spec);
    int (*observe)(void *ctx, char *out, size_t capacity);
    int (*latch)(void *ctx);
    int (*close)(void *ctx);
    void (*destroy)(void *ctx);
    void *ctx;
} sleela_munction_channel;
typedef struct sleela_munction sleela_munction;
sleela_munction *sleela_munction_start(const char *name);
int sleela_munction_connect(sleela_munction *, const sleela_munction_channel *, const char *address);
int sleela_munction_enable(sleela_munction *, const char *policy);
int64_t sleela_munction_send(sleela_munction *, const uint8_t *, size_t);
int sleela_munction_thatch(sleela_munction *, const char *spec);
int64_t sleela_munction_consume(sleela_munction *, uint8_t *, size_t);
int sleela_munction_observe(sleela_munction *, char *, size_t);
int sleela_munction_latch(sleela_munction *);
sleela_munction_outcome sleela_munction_close(sleela_munction *, char *, size_t);
sleela_munction_outcome sleela_munction_abort(sleela_munction *, char *, size_t);
int sleela_munction_call_count(const sleela_munction *);
uint64_t sleela_munction_sent_bytes(const sleela_munction *);
uint64_t sleela_munction_acknowledged_bytes(const sleela_munction *);
uint64_t sleela_munction_received_units(const sleela_munction *);
int sleela_munction_coherent(const sleela_munction *);
sleela_munction_phase sleela_munction_phase_of(const sleela_munction *);
uint64_t sleela_munction_digest(const uint8_t *, size_t);
#ifdef __cplusplus
}
#endif
#endif
