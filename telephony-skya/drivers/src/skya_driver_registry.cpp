#include "../include/skya_phone_driver.h"

static const skya_phone_driver *g_drivers[64];
static size_t g_driver_count = 0;

extern "C" int skya_driver_register(const skya_phone_driver *driver) {
    if (!driver || !driver->name || g_driver_count >= 64) return -1;
    g_drivers[g_driver_count++] = driver;
    return 0;
}

extern "C" size_t skya_driver_count(void) { return g_driver_count; }

extern "C" const skya_phone_driver *skya_driver_at(size_t index) {
    return index < g_driver_count ? g_drivers[index] : 0;
}

extern "C" const skya_phone_driver *skya_driver_find(const skya_driver_device *device) {
    if (!device) return 0;
    for (size_t i = 0; i < g_driver_count; ++i) {
        if (g_drivers[i]->probe && g_drivers[i]->probe(device) > 0) return g_drivers[i];
    }
    return 0;
}
