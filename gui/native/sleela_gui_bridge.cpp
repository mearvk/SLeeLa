#include "sleela_gui_bridge.h"

#include <new>

struct SLGuiBridge {
    void *context;
    SLGuiCallFn call;
    SLGuiRefreshFn refresh;
};

SLGuiBridge *slgui_bridge_create(void *context, SLGuiCallFn call) {
    if (call == nullptr) return nullptr;
    SLGuiBridge *bridge = new (std::nothrow) SLGuiBridge{context, call, nullptr};
    return bridge;
}

const char *slgui_bridge_call(SLGuiBridge *bridge,
                              const char *operation,
                              const char *arguments) {
    if (bridge == nullptr || bridge->call == nullptr || operation == nullptr) return nullptr;
    return bridge->call(bridge->context, operation, arguments == nullptr ? "" : arguments);
}

void slgui_bridge_on_document_change(SLGuiBridge *bridge, SLGuiRefreshFn refresh) {
    if (bridge == nullptr) return;
    bridge->refresh = refresh;
}

int slgui_bridge_document_changed(SLGuiBridge *bridge,
                                  const char *document_id,
                                  const char *revision) {
    if (bridge == nullptr || bridge->refresh == nullptr || document_id == nullptr) return 0;
    bridge->refresh(bridge->context, document_id, revision == nullptr ? "" : revision);
    return 1;
}

void slgui_bridge_destroy(SLGuiBridge *bridge) {
    delete bridge;
}
