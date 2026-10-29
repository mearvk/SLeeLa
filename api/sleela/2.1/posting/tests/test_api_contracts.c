#include "../sleela_post.h"
#include "../../listener/sleela_listener.h"
#include "../../router/sleela_router.h"
#include <stdio.h>
#include <string.h>

static int failures;
static void check(const char *name, int ok) {
    printf("%s %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) ++failures;
}

int main(void) {
    char error[256] = {0};
    sleela_listener_t listener = {"127.0.0.1", 8080, "http", "/health"};
    check("listener valid descriptor", sleela_listener_validate(&listener, error, sizeof(error)) == 0);
    listener.port = 0;
    check("listener rejects zero port", sleela_listener_validate(&listener, error, sizeof(error)) != 0 && strstr(error, "port") != NULL);
    listener.port = 8080;
    listener.protocol = "https";
    check("listener rejects unsupported protocol", sleela_listener_validate(&listener, error, sizeof(error)) != 0);
    check("listener rejects null descriptor", sleela_listener_validate(NULL, error, sizeof(error)) != 0);

    sleela_route_t route = {"GET", "/health", "/handlers/health"};
    check("router valid descriptor", sleela_router_validate(&route, error, sizeof(error)) == 0);
    check("router exact method and path match", sleela_router_match(&route, "GET", "/health") == 1);
    check("router method mismatch", sleela_router_match(&route, "POST", "/health") == 0);
    check("router path mismatch", sleela_router_match(&route, "GET", "/health/") == 0);
    route.path = "health";
    check("router rejects relative path", sleela_router_validate(&route, error, sizeof(error)) != 0);
    check("router rejects null descriptor", sleela_router_validate(NULL, error, sizeof(error)) != 0);

    sleela_post_t post = {"POST", "/submit", "text/plain", "hello"};
    check("post valid descriptor", sleela_post_validate(&post, error, sizeof(error)) == 0);
    post.method = "GET";
    check("post rejects non-POST method", sleela_post_validate(&post, error, sizeof(error)) != 0);
    if (failures) {
        printf("API POST/LISTENER/ROUTER TESTS: FAIL (%d)\n", failures);
        return 1;
    }
    puts("API POST/LISTENER/ROUTER TESTS: PASS");
    return 0;
}
