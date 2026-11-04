#include "sleela_os_support.h"
#include <assert.h>

static int observed;
static int ran_tasks;
static slosh_status on_event(slosh_runtime *r, const slosh_event *e, void *u) {
    (void)r; (void)u;
    assert(e->kind == SLOSH_EVENT_USER);
    ++observed;
    return SLOSH_OK;
}
static slosh_status task(slosh_runtime *r, void *u) {
    (void)r;
    ++*(int *)u;
    return SLOSH_OK;
}
int main(void) {
    slosh_runtime *r = NULL;
    slosh_token token = 0;
    int ran = 0;
    assert(slosh_runtime_create(&r) == SLOSH_OK);
    assert(slosh_subscribe(r, SLOSH_EVENT_USER, on_event, NULL, &token) == SLOSH_OK);
    assert(slosh_emit(r, SLOSH_EVENT_USER, NULL, 0) == SLOSH_OK && observed == 1);
    assert(slosh_unsubscribe(r, token) == SLOSH_OK);
    assert(slosh_post(r, task, &ran_tasks) == SLOSH_OK);
    assert(slosh_pending(r) == 1);
    assert(slosh_run_one(r, &ran) == SLOSH_OK && ran && ran_tasks == 1);
    assert(slosh_pending(r) == 0);
    slosh_runtime_destroy(r);
    return 0;
}
