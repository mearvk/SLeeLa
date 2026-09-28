#include "data_structures.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    SLDSStack stack; slds_stack_init(&stack);
    SLDSValue a = { .kind = SLDS_INT, .as.i = 42 };
    assert(slds_stack_push(&stack, a));
    SLDSValue out = {0};
    assert(slds_stack_pop(&stack, &out));
    assert(out.kind == SLDS_INT && out.as.i == 42);
    slds_stack_free(&stack);

    SLDSQueue queue; slds_queue_init(&queue);
    assert(slds_queue_enqueue(&queue, a));
    assert(slds_queue_dequeue(&queue, &out));
    assert(out.as.i == 42);
    slds_queue_free(&queue);

    SLDSMap map; slds_map_init(&map);
    assert(slds_map_put(&map, "answer", a));
    assert(slds_map_get(&map, "answer", &out));
    assert(out.as.i == 42);
    slds_map_free(&map);

    puts("data-structures C tests: ok");
    return 0;
}
