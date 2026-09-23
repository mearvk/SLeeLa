#include "sleela_ai.h"
#include <string.h>
#include <stdio.h>

static int has_text(const char *s) { return s && *s; }

int slai_engine_init(SLAIEngine *engine, SLAIBackend backend) {
    if (!engine || (backend != SL_AI_BACKEND_NATIVE && backend != SL_AI_BACKEND_CONNECTOR))
        return -1;
    memset(engine, 0, sizeof(*engine));
    engine->backend = backend;
    return 0;
}

int slai_engine_bind_vm(SLAIEngine *engine, SLAIVMInvokeFn invoke, void *context) {
    if (!engine || !invoke) return -1;
    engine->invoke = invoke;
    engine->context = context;
    engine->backend = SL_AI_BACKEND_CONNECTOR;
    return 0;
}

int slai_validate_input(const SLAIInput *input) {
    if (!input || input->kind < SL_AI_KIND_DATA || input->kind > SL_AI_KIND_VIDEO)
        return -1;
    if (!has_text(input->name)) return -2;
    if (input->kind == SL_AI_KIND_FILE && !has_text(input->path)) return -3;
    if ((input->kind == SL_AI_KIND_AUDIO || input->kind == SL_AI_KIND_VIDEO) && !input->media)
        return -4;
    if (input->kind == SL_AI_KIND_DATA && !input->data && input->size != 0)
        return -5;
    return 0;
}

int slai_validate_request(const SLAIRequest *request) {
    if (!request || request->operation < SL_AI_OP_INSPECT || request->operation > SL_AI_OP_TRANSFORM)
        return -1;
    if (request->backend != SL_AI_BACKEND_NATIVE && request->backend != SL_AI_BACKEND_CONNECTOR)
        return -2;
    if (!has_text(request->model_id)) return -3;
    if (request->max_observations > 0 && !request->observations) return -4;
    return 0;
}

int slai_invoke(SLAIEngine *engine, const SLAIRequest *request,
                const SLAIInput *input, SLAIResult *result) {
    if (!engine || !result || slai_validate_request(request) || slai_validate_input(input))
        return -1;
    memset(result, 0, sizeof(*result));
    if (!engine->invoke)
        return -2;
    if (request->backend != engine->backend && engine->backend != SL_AI_BACKEND_CONNECTOR)
        return -3;
    return engine->invoke(request, input, result, engine->context);
}

int slai_model_from_xml(const char *xml, size_t length, SLAIModelDescriptor *model) {
    const char *p;
    const char *end;
    if (!xml || !model || length == 0) return -1;
    memset(model, 0, sizeof(*model));
    end = xml + length;
    p = strstr(xml, "model id="");
    if (!p || p >= end) return -2;
    p += 10;
    {
        const char *q = strchr(p, '"');
        if (!q || q >= end) return -3;
        model->model_id = p;
    }
    p = strstr(xml, "format="");
    if (p && p < end) model->format = p + 8;
    p = strstr(xml, "source="");
    if (p && p < end) model->source = p + 8;
    model->revision = 1;
    p = strstr(xml, "trusted="true"");
    model->trusted = (p && p < end) ? 1 : 0;
    return 0;
}

const char *slai_input_kind_name(SLAIInputKind kind) {
    switch (kind) {
        case SL_AI_KIND_DATA: return "data";
        case SL_AI_KIND_FILE: return "file";
        case SL_AI_KIND_AUDIO: return "audio";
        case SL_AI_KIND_VIDEO: return "video";
        default: return "unknown";
    }
}

const char *slai_operation_name(SLAIOperation operation) {
    switch (operation) {
        case SL_AI_OP_INSPECT: return "inspect";
        case SL_AI_OP_CLASSIFY: return "classify";
        case SL_AI_OP_EXTRACT: return "extract";
        case SL_AI_OP_SUMMARIZE: return "summarize";
        case SL_AI_OP_TRANSFORM: return "transform";
        default: return "unknown";
    }
}

const char *slai_backend_name(SLAIBackend backend) {
    return backend == SL_AI_BACKEND_NATIVE ? "native" :
           backend == SL_AI_BACKEND_CONNECTOR ? "connector" : "unknown";
}
