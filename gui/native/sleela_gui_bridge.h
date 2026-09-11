#ifndef SLEELA_GUI_BRIDGE_H
#define SLEELA_GUI_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SLGuiBridge SLGuiBridge;

typedef const char *(*SLGuiCallFn)(void *context,
                                   const char *operation,
                                   const char *arguments);

SLGuiBridge *slgui_bridge_create(void *context, SLGuiCallFn call);
const char *slgui_bridge_call(SLGuiBridge *bridge,
                              const char *operation,
                              const char *arguments);
void slgui_bridge_destroy(SLGuiBridge *bridge);

#ifdef __cplusplus
}
#endif

#endif
