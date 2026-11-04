#include "osls_runtime.h"
#include <assert.h>

static int observed;
static osls_runtime_status on_event(osls_runtime *r, const osls_event *e, void *u) {
    (void)r; (void)u;
    assert(e->kind == OSLS_EVENT_USER);
    ++observed;
    return OSLS_RUNTIME_OK;
}
static osls_runtime_status task(osls_runtime *r, void *u) {
    (void)r; ++*(int *)u; return OSLS_RUNTIME_OK;
}
int main(void) {
    osls_runtime *r = 0;
    osls_runtime_token token = 0;
    int calls = 0, ran = 0;
    assert(osls_runtime_create(&r) == OSLS_RUNTIME_OK);
    assert(osls_runtime_subscribe(r, OSLS_EVENT_USER, on_event, 0, &token) == OSLS_RUNTIME_OK);
    assert(osls_runtime_emit(r, OSLS_EVENT_USER, 0, 0) == OSLS_RUNTIME_OK && observed == 1);
    assert(osls_runtime_unsubscribe(r, token) == OSLS_RUNTIME_OK);
    assert(osls_runtime_post(r, task, &calls) == OSLS_RUNTIME_OK);
    assert(osls_runtime_pending(r) == 1);
    assert(osls_runtime_run_one(r, &ran) == OSLS_RUNTIME_OK && ran && calls == 1);
    assert(osls_runtime_pending(r) == 0);
    osls_runtime_destroy(r);
    return 0;
}
