#include "skya_engine.h"
#include "../drivers/include/skya_builtin_drivers.h"
#include "../drivers/src/skya_driver_registry.h"
#include "../drivers/include/skya_phone_driver.h"
#include <cstdio>
#include <cstring>
#include <string>

// List every built-in phone/headset driver the drivers subsystem registers.
// This exercises libskya-drivers.a: skya_register_builtin_drivers() fills the
// registry from the per-vendor driver tree, and we enumerate it here.
static int list_drivers() {
    // skya_register_builtin_drivers() returns the number of drivers it
    // registered; a non-positive result means nothing registered.
    if (skya_register_builtin_drivers() <= 0) {
        std::fprintf(stderr, "skya: failed to register built-in drivers\n");
        return 1;
    }
    size_t n = skya_driver_count();
    std::printf("Skya built-in drivers: %zu\n", n);
    for (size_t i = 0; i < n; i++) {
        const skya_phone_driver *d = skya_driver_at(i);
        if (d) std::printf("  %-12s %s\n", d->vendor ? d->vendor : "?", d->name ? d->name : "?");
    }
    return 0;
}

int main(int argc, char **argv) {
    skya_options_t o{};
    o.max_peers = 256;
    o.port = 8443;
    o.http_version = 3;
    skya_role_t role = SKYA_BOTH;
    std::string room = "lobby";
    for (int i = 1; i < argc; i++) {
        if (!std::strcmp(argv[i], "--server")) role = SKYA_SERVER;
        else if (!std::strcmp(argv[i], "--client")) role = SKYA_CLIENT;
        else if (!std::strcmp(argv[i], "--both")) role = SKYA_BOTH;
        else if (!std::strcmp(argv[i], "--http2")) o.http_version = 2;
        else if (!std::strcmp(argv[i], "--http3")) o.http_version = 3;
        else if (!std::strcmp(argv[i], "--room") && i + 1 < argc) room = argv[++i];
        else if (!std::strcmp(argv[i], "--drivers")) return list_drivers();
    }
    auto *e = skya_create(&o);
    if (!e) return 1;
    int rc = skya_start(e, role);
    if (rc == 0) rc = skya_join(e, room.c_str());
    std::printf("Skya %s HTTP/%u port=%u room=%s\n", skya_status(e), o.http_version, o.port, room.c_str());
    skya_destroy(e);
    return rc ? 1 : 0;
}
