#include "../garbage_collector.h"
#include "../security_supervisor.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    SLGCObject *child;
} Node;

static void trace_node(SLGCObject *object, void *context) {
    (void)context;
    Node *node = (Node *)object->payload;
    if (node && node->child) {
        /* The node's owning collector is supplied by the test's global. */
        extern GarbageCollector *test_gc_current;
        gc_mark(test_gc_current, node->child);
    }
}

GarbageCollector *test_gc_current = NULL;

static void test_root_reachability_and_sweep(void) {
    GarbageCollector gc;
    gc_init(&gc, 1024u * 1024u);
    test_gc_current = &gc;

    SLGCObject *parent = gc_allocate(&gc, sizeof(Node), trace_node, NULL, NULL);
    SLGCObject *child = gc_allocate(&gc, sizeof(Node), trace_node, NULL, NULL);
    assert(parent && child);
    ((Node *)parent->payload)->child = child;
    assert(gc_add_root(&gc, parent) == 1);
    assert(gc_collect_full(&gc) == 0);
    assert(gc_live_objects(&gc) == 2);

    assert(gc_remove_root(&gc, parent) == 1);
    assert(gc_collect_full(&gc) > 0);
    assert(gc_live_objects(&gc) == 0);
    gc_free(&gc);
    test_gc_current = NULL;
}


static void test_remembered_owner_survives_repeated_young_collections(void) {
    GarbageCollector gc;
    gc_init(&gc, 1024u * 1024u);
    test_gc_current = &gc;

    SLGCObject *parent = gc_allocate(&gc, sizeof(Node), trace_node, NULL, NULL);
    assert(parent);
    assert(gc_add_root(&gc, parent) == 1);
    assert(gc_collect_young(&gc) == 0);
    assert(gc_collect_young(&gc) == 0);
    assert(parent->generation == SLGC_GENERATION_OLD);

    SLGCObject *child = gc_allocate(&gc, sizeof(Node), trace_node, NULL, NULL);
    assert(child);
    ((Node *)parent->payload)->child = child;
    gc_write_barrier(&gc, parent, NULL, child);

    assert(gc_collect_young(&gc) == 0);
    assert(gc_live_objects(&gc) == 2);
    assert(gc_collect_young(&gc) == 0);
    assert(gc_live_objects(&gc) == 2);

    assert(gc_remove_root(&gc, parent) == 1);
    assert(gc_collect_full(&gc) > 0);
    assert(gc_live_objects(&gc) == 0);
    gc_free(&gc);
    test_gc_current = NULL;
}

static void test_external_payload_ownership(void) {
    GarbageCollector gc;
    gc_init(&gc, 1024u * 1024u);
    int *payload = (int *)malloc(sizeof(*payload));
    assert(payload);
    *payload = 7;
    SLGCObject *object = gc_allocate_external(&gc, payload, sizeof(*payload), NULL, NULL, NULL);
    assert(object);
    assert(gc_collect_full(&gc) >= sizeof(*payload));
    /* External payloads remain owned by the caller unless a destroy callback
       explicitly assumes ownership. */
    assert(*payload == 7);
    free(payload);
    gc_free(&gc);
}


static void test_managed_heap_limit(void) {
    GarbageCollector gc;
    gc_init(&gc, 1024u * 1024u);
    assert(gc_limit(&gc) == 512u * 1024u * 1024u);
    assert(gc_set_limit(&gc, sizeof(Node)) == 1);
    SLGCObject *first = gc_allocate(&gc, sizeof(Node), NULL, NULL, NULL);
    assert(first);
    assert(gc_allocate(&gc, 1, NULL, NULL, NULL) == NULL);
    assert(gc_set_limit(&gc, 1) == 0);
    gc_free(&gc);
}

static void test_security_default_deny_and_quotas(void) {
    SecuritySupervisor security;
    security_supervisor_init(&security, SLS_ROLE_USER);
    assert(security_supervisor_authorize_class(&security, "UnregisteredClass") == 0);
    assert(security_supervisor_set_class(&security, "AllowedClass", 1, 1, 16));
    assert(security_supervisor_set_role_limit(&security, SLS_ROLE_USER, 2, 32));
    assert(security_supervisor_reserve(&security, "AllowedClass", 16) == 1);
    assert(security_supervisor_reserve(&security, "AllowedClass", 1) == 0);
    assert(security_supervisor_reserve(&security, "UnregisteredClass", 1) == 0);
    security_supervisor_release(&security, "AllowedClass", 16);
    assert(security.total_instances == 0);
    assert(security.total_bytes == 0);
}

int main(void) {
    test_root_reachability_and_sweep();
    test_remembered_owner_survives_repeated_young_collections();
    test_external_payload_ownership();
    test_managed_heap_limit();
    test_security_default_deny_and_quotas();
    puts("runtime GC/security regression tests: PASS");
    return 0;
}
