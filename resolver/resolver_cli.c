#include "resolver.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--self-test") == 0) return sleela_resolver_self_test();
    if (argc == 3 && strcmp(argv[1], "--resolve") == 0) {
        sleela_resolution r;
        if (!sleela_resolver_resolve(argv[2], &r)) return 2;
        printf("kind=%d family=%d dynamic=%d canonical=%s address=%s\n",
               (int)r.kind, r.family, r.dynamic, r.canonical, r.address);
        return 0;
    }
    fprintf(stderr, "usage: %s --self-test | --resolve HOSTNAME_OR_IP_OR_PATH\n", argv[0]);
    return 2;
}
