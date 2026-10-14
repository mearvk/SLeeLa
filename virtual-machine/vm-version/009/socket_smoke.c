/* socket_smoke.c -- C-level smoke test for Sleela 1.1 TCP built-ins.
 *
 * The test assembles a tiny VM program directly through the core builder API:
 * main listens, spawns a server worker, then runs a client in the main thread.
 * The client connects over loopback, sends "ping", receives "pong", and
 * returns the received String. The server accepts, reads, writes "pong", and
 * closes. This exercises listen/accept/connect/sockread/sockwrite/sockclose,
 * socket-table lifecycle, and interaction with OP_SPAWN.
 */
#include "sleela_core.h"

#include <stdio.h>
#include <string.h>

static int add_str(SLVM* vm, const char* s) {
    return slvm_add_const_str(vm, s);
}

int main(void) {
    enum { PORT = 45217 };
    SLVM* vm = slvm_new();
    if (!vm) return 1;

    int listener_g = slvm_declare_global(vm, "listener");

    int server = slvm_begin_func(vm, "server", 0, 0);
    (void)server;
    slvm_emit(vm, OP_LOADG, listener_g);
    slvm_emit(vm, OP_ACCEPT, 0);
    slvm_emit(vm, OP_STOREG, listener_g); /* reuse the global as client handle */
    slvm_emit(vm, OP_LOADG, listener_g);
    slvm_emit(vm, OP_SOCKREAD, 0);
    slvm_emit(vm, OP_POP, 0);
    slvm_emit(vm, OP_LOADG, listener_g);
    slvm_emit(vm, OP_CONST, add_str(vm, "pong"));
    slvm_emit(vm, OP_SOCKWRITE, 0);
    slvm_emit(vm, OP_POP, 0);
    slvm_emit(vm, OP_LOADG, listener_g);
    slvm_emit(vm, OP_SOCKCLOSE, 0);
    slvm_emit(vm, OP_POP, 0);
    slvm_emit(vm, OP_CONST, slvm_add_const_int(vm, 0));
    slvm_emit(vm, OP_RET, 0);
    slvm_end_func(vm);

    int client = slvm_begin_func(vm, "client", 0, 1);
    (void)client;
    slvm_emit(vm, OP_CONST, add_str(vm, "127.0.0.1"));
    slvm_emit(vm, OP_CONST, slvm_add_const_int(vm, PORT));
    slvm_emit(vm, OP_CONNECT, 0);
    slvm_emit(vm, OP_STOREL, 0);
    slvm_emit(vm, OP_LOADL, 0);
    slvm_emit(vm, OP_CONST, add_str(vm, "ping"));
    slvm_emit(vm, OP_SOCKWRITE, 0);
    slvm_emit(vm, OP_POP, 0);
    slvm_emit(vm, OP_LOADL, 0);
    slvm_emit(vm, OP_SOCKREAD, 0);
    slvm_emit(vm, OP_LOADL, 0);
    slvm_emit(vm, OP_SOCKCLOSE, 0);
    slvm_emit(vm, OP_POP, 0);
    slvm_emit(vm, OP_RET, 0);
    slvm_emit(vm, OP_CONST, slvm_add_const_str(vm, ""));
    slvm_emit(vm, OP_RET, 0);
    slvm_end_func(vm);

    int main_fn = slvm_begin_func(vm, "main", 0, 0);
    (void)main_fn;
    slvm_emit(vm, OP_CONST, slvm_add_const_int(vm, PORT));
    slvm_emit(vm, OP_LISTEN, 0);
    slvm_emit(vm, OP_STOREG, listener_g);
    slvm_emit(vm, OP_SPAWN, server);
    slvm_emit(vm, OP_POP, 0);
    slvm_emit(vm, OP_CALL, client);
    slvm_emit(vm, OP_RET, 0);
    slvm_end_func(vm);

    slvm_set_entry(vm, main_fn);
    SLResult result = slvm_run(vm);
    SLValue value = slvm_result(vm);

    int ok = result == SLR_OK && value.type == SL_STR &&
             strcmp(slvm_str(vm, value.as.s), "pong") == 0;
    if (!ok) {
        fprintf(stderr, "socket smoke failed: result=%d type=%d error=%s\n",
                result, value.type, slvm_error(vm) ? slvm_error(vm) : "none");
    } else {
        printf("socket smoke: listen/accept/connect/read/write/close OK\n");
    }

    slvm_free(vm);
    return ok ? 0 : 1;
}
