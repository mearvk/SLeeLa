/*
 * gui/native/slgui_bridge_test.cpp
 *
 * Round-trip test for the SLeeLa GUI native ABI bridge (Path 3). It exercises
 * the C-compatible callback boundary end to end:
 *   - slgui_bridge_create requires a non-null call callback (fail-closed);
 *   - slgui_bridge_call forwards operation/arguments to the host callback and
 *     returns its result, with NULL arguments normalized to "";
 *   - slgui_bridge_on_document_change installs/clears the refresh callback;
 *   - slgui_bridge_document_changed forwards exactly one change and returns 1
 *     when a callback is installed, 0 otherwise, with NULL revision -> "";
 *   - the 1..14 document bound (SLGUI_MIN/MAX_DOCUMENTS) is the documented ABI
 *     contract a host enforces when attaching documents.
 *
 * Exit code 0 means every assertion held. Any failure prints a diagnostic to
 * stderr and exits non-zero so `make test` fails closed.
 */
#include "sleela_gui_bridge.h"

#include <cstdio>
#include <cstring>
#include <string>

namespace {

int g_failures = 0;

void check(bool condition, const char *what) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", what);
        ++g_failures;
    }
}

// Host-side call context: records the last operation/arguments it was asked to
// perform and returns a deterministic, inspectable result string.
struct CallContext {
    std::string lastOperation;
    std::string lastArguments;
    std::string result;
};

const char *onCall(void *context, const char *operation, const char *arguments) {
    CallContext *ctx = static_cast<CallContext *>(context);
    ctx->lastOperation = operation != nullptr ? operation : "";
    ctx->lastArguments = arguments != nullptr ? arguments : "";
    ctx->result = "ok:" + ctx->lastOperation;
    return ctx->result.c_str();
}

// Document-change refresh sink: counts deliveries and records the last change.
struct RefreshContext {
    int deliveries = 0;
    std::string lastId;
    std::string lastRevision;
};

RefreshContext g_refresh;

void onRefresh(void * /*context*/, const char *document_id, const char *revision) {
    ++g_refresh.deliveries;
    g_refresh.lastId = document_id != nullptr ? document_id : "";
    g_refresh.lastRevision = revision != nullptr ? revision : "";
}

}  // namespace

int main() {
    // Fail-closed construction: a null call callback yields no bridge.
    check(slgui_bridge_create(nullptr, nullptr) == nullptr,
          "create with null call returns null");

    CallContext callCtx;
    SLGuiBridge *bridge = slgui_bridge_create(&callCtx, onCall);
    check(bridge != nullptr, "create with valid call returns a bridge");

    // call() forwards to the host callback and returns its result.
    const char *result = slgui_bridge_call(bridge, "show", "Editor;800;600");
    check(result != nullptr && std::strcmp(result, "ok:show") == 0,
          "call returns host result");
    check(callCtx.lastOperation == "show", "call forwards operation");
    check(callCtx.lastArguments == "Editor;800;600", "call forwards arguments");

    // NULL arguments are normalized to the empty string.
    slgui_bridge_call(bridge, "noop", nullptr);
    check(callCtx.lastArguments.empty(), "null arguments normalized to empty");

    // A guarded call on a null operation is rejected.
    check(slgui_bridge_call(bridge, nullptr, "x") == nullptr,
          "call with null operation returns null");

    // Document-change hook: no delivery before a callback is installed.
    check(slgui_bridge_document_changed(bridge, "notes.md", "100:2048") == 0,
          "document_changed is a no-op with no callback");

    slgui_bridge_on_document_change(bridge, onRefresh);
    check(slgui_bridge_document_changed(bridge, "notes.md", "100:2048") == 1,
          "document_changed delivers once a callback is installed");
    check(g_refresh.deliveries == 1, "refresh delivered exactly once");
    check(g_refresh.lastId == "notes.md", "refresh carries the document id");
    check(g_refresh.lastRevision == "100:2048", "refresh carries the revision");

    // NULL revision is normalized to the empty string.
    slgui_bridge_document_changed(bridge, "notes.md", nullptr);
    check(g_refresh.lastRevision.empty(), "null revision normalized to empty");

    // A null document id is rejected (returns 0, no delivery).
    int before = g_refresh.deliveries;
    check(slgui_bridge_document_changed(bridge, nullptr, "1:1") == 0,
          "document_changed with null id returns 0");
    check(g_refresh.deliveries == before, "null id delivers nothing");

    // Clearing the callback restores the no-op behavior.
    slgui_bridge_on_document_change(bridge, nullptr);
    check(slgui_bridge_document_changed(bridge, "notes.md", "101:2049") == 0,
          "document_changed is a no-op after the callback is cleared");

    // The documented 1..14 ABI bound a host enforces when attaching documents.
    check(SLGUI_MIN_DOCUMENTS == 1, "min documents is 1");
    check(SLGUI_MAX_DOCUMENTS == 14, "max documents is 14");

    slgui_bridge_destroy(bridge);
    // Destroying a null bridge is safe.
    slgui_bridge_destroy(nullptr);

    if (g_failures == 0) {
        std::printf("PASS: slgui_bridge round-trip (%d checks)\n", 20);
        return 0;
    }
    std::fprintf(stderr, "slgui_bridge test failed: %d failure(s)\n", g_failures);
    return 1;
}
