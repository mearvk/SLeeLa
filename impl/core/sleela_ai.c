#include "sleela_ai.h"
#include <string.h>
#include <stdlib.h>

static int has_text(const char *s) { return s && *s; }

static int copy_xml_attr(const char *xml, size_t length, const char *key,
                         char *out, size_t out_size) {
    size_t key_len = strlen(key);
    size_t i;
    if (!xml || !key || !out || out_size == 0) return -1;
    out[0] = '\0';
    for (i = 0; i + key_len + 2 <= length; ++i) {
        if (memcmp(xml + i, key, key_len) == 0 && xml[i + key_len] == '=' &&
            xml[i + key_len + 1] == '"') {
            size_t j = i + key_len + 2;
            size_t n = 0;
            while (j < length && xml[j] != '"') {
                if (n + 1 >= out_size) return -2;
                out[n++] = xml[j++];
            }
            if (j >= length) return -3;
            out[n] = '\0';
            return 0;
        }
    }
    return 1;
}

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
    if (!engine->invoke) return -2;
    if (request->backend != engine->backend) return -3;
    return engine->invoke(request, input, result, engine->context);
}

int slai_model_from_xml(const char *xml, size_t length, SLAIModelDescriptor *model) {
    int rc;
    if (!xml || !model || length == 0) return -1;
    memset(model, 0, sizeof(*model));
    rc = copy_xml_attr(xml, length, "id", model->model_id, sizeof(model->model_id));
    if (rc != 0) return -2;
    rc = copy_xml_attr(xml, length, "format", model->format, sizeof(model->format));
    if (rc != 0) return -3;
    rc = copy_xml_attr(xml, length, "source", model->source, sizeof(model->source));
    if (rc != 0) return -4;
    {
        char revision[16];
        rc = copy_xml_attr(xml, length, "revision", revision, sizeof(revision));
        if (rc != 0) return -5;
        model->revision = (uint32_t)strtoul(revision, NULL, 10);
    }
    {
        char trusted[16];
        rc = copy_xml_attr(xml, length, "trusted", trusted, sizeof(trusted));
        if (rc != 0) return -6;
        model->trusted = strcmp(trusted, "true") == 0;
    }
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
