#ifndef SLEELA_GUI_BRIDGE_H
#define SLEELA_GUI_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SLGuiBridge SLGuiBridge;

typedef const char *(*SLGuiCallFn)(void *context,
                                   const char *operation,
                                   const char *arguments);

/*
 * Document-change refresh callback (Path 3, native ABI).
 *
 * The native/C++ side invokes this when a watched document changes on an OS
 * call, so the hosted GUI can refresh the running window. document_id is the
 * SLeeLa-facing document name and revision is the OS-derived change
 * fingerprint (last-modified:size); both are NUL-terminated UTF-8. This is the
 * native counterpart of DocumentListener -> SleelaGui.refresh() on the Java
 * side. The document count a host attaches through this bridge is bounded to
 * 1..14 (SLGUI_MIN_DOCUMENTS..SLGUI_MAX_DOCUMENTS) to match the Java listener.
 */
#define SLGUI_MIN_DOCUMENTS 1
#define SLGUI_MAX_DOCUMENTS 14

typedef void (*SLGuiRefreshFn)(void *context,
                               const char *document_id,
                               const char *revision);

SLGuiBridge *slgui_bridge_create(void *context, SLGuiCallFn call);
const char *slgui_bridge_call(SLGuiBridge *bridge,
                              const char *operation,
                              const char *arguments);

/*
 * Register the document-change refresh callback and deliver a change to it.
 * slgui_bridge_on_document_change installs refresh (may be NULL to clear).
 * slgui_bridge_document_changed forwards one change to the installed callback;
 * it is a no-op when no callback is installed. Returns 1 when delivered, else 0.
 */
void slgui_bridge_on_document_change(SLGuiBridge *bridge, SLGuiRefreshFn refresh);
int slgui_bridge_document_changed(SLGuiBridge *bridge,
                                  const char *document_id,
                                  const char *revision);

void slgui_bridge_destroy(SLGuiBridge *bridge);

#ifdef __cplusplus
}
#endif

#endif
