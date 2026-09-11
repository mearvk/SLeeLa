#include "sleela_gui_bridge.h"

#include <new>

struct SLGuiBridge {
    void *context;
    SLGuiCallFn call;
};

SLGuiBridge *slgui_bridge_create(void *context, SLGuiCallFn call) {
    if (call == nullptr) return nullptr;
    SLGuiBridge *bridge = new (std::nothrow) SLGuiBridge{context, call};
    return bridge;
}

const char *slgui_bridge_call(SLGuiBridge *bridge,
                              const char *operation,
                              const char *arguments) {
    if (bridge == nullptr || bridge->call == nullptr || operation == nullptr) return nullptr;
    return bridge->call(bridge->context, operation, arguments == nullptr ? "" : arguments);
}

void slgui_bridge_destroy(SLGuiBridge *bridge) {
    delete bridge;
}
