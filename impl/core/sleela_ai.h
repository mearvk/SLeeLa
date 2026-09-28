#ifndef SLEELA_AI_H
#define SLEELA_AI_H

#include <stddef.h>
#include <stdint.h>
#include "sleela_media.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SL_AI_MAX_LABEL 96
#define SL_AI_MAX_PATH 1024
#define SL_AI_MAX_TEXT 4096
#define SL_AI_MAX_MODEL_ID 128
#define SL_AI_MAX_FORMAT 64
#define SL_AI_MAX_SOURCE 128
#define SL_AI_MAX_FIELDS 32
#define SL_AI_MAX_FIELD_NAME 64
#define SL_AI_MAX_FIELD_TYPE 64
#define SL_AI_MAX_URL 2048
#define SL_AI_MAX_ARGS 2048

typedef enum {
    SL_AI_KIND_DATA = 1,
    SL_AI_KIND_FILE = 2,
    SL_AI_KIND_AUDIO = 3,
    SL_AI_KIND_VIDEO = 4
} SLAIInputKind;

typedef enum {
    SL_AI_OP_INSPECT = 1,
    SL_AI_OP_CLASSIFY = 2,
    SL_AI_OP_EXTRACT = 3,
    SL_AI_OP_SUMMARIZE = 4,
    SL_AI_OP_TRANSFORM = 5
} SLAIOperation;

typedef enum {
    SL_AI_BACKEND_NATIVE = 1,
    SL_AI_BACKEND_CONNECTOR = 2
} SLAIBackend;

typedef struct {
    SLAIInputKind kind;
    const char *name;
    const char *path;
    const void *data;
    size_t size;
    const SLMediaFrame *media;
} SLAIInput;

typedef struct {
    char label[SL_AI_MAX_LABEL];
    double confidence;
    int64_t offset_ns;
    int64_t duration_ns;
} SLAIObservation;

typedef struct {
    SLAIOperation operation;
    SLAIBackend backend;
    const char *model_id;
    const char *instruction;
    size_t max_observations;
    SLAIObservation *observations;
    size_t observation_count;
} SLAIRequest;

typedef struct {
    int accepted;
    int completed;
    uint64_t request_id;
    size_t observation_count;
    char summary[SL_AI_MAX_TEXT];
} SLAIResult;

typedef struct {
    char model_id[SL_AI_MAX_MODEL_ID];
    char format[SL_AI_MAX_FORMAT];
    char source[SL_AI_MAX_SOURCE];
    uint32_t revision;
    int trusted;
} SLAIModelDescriptor;

typedef struct {
    char name[SL_AI_MAX_FIELD_NAME];
    char type[SL_AI_MAX_FIELD_TYPE];
    int required;
} SLAIDataModelField;

typedef struct {
    char id[SL_AI_MAX_MODEL_ID];
    uint32_t revision;
    size_t field_count;
    SLAIDataModelField fields[SL_AI_MAX_FIELDS];
} SLAIDataModel;

typedef enum {
    SL_AI_SOURCE_DATA = 1,
    SL_AI_SOURCE_FILE = 2,
    SL_AI_SOURCE_AUDIO = 3,
    SL_AI_SOURCE_VIDEO = 4,
    SL_AI_SOURCE_AI_FLOW = 5,
    SL_AI_SOURCE_DATA_FLOW = 6
} SLAIXMLSourceKind;

typedef struct {
    SLAIXMLSourceKind kind;
    char url[SL_AI_MAX_URL];
    char args[SL_AI_MAX_ARGS];
} SLAIXMLSource;

typedef int (*SLAIVMInvokeFn)(const SLAIRequest *request,
                             const SLAIInput *input,
                             SLAIResult *result,
                             void *context);

typedef struct {
    SLAIBackend backend;
    SLAIVMInvokeFn invoke;
    void *context;
} SLAIEngine;

int slai_engine_init(SLAIEngine *engine, SLAIBackend backend);
int slai_engine_bind_vm(SLAIEngine *engine, SLAIVMInvokeFn invoke, void *context);
int slai_engine_bind_native(SLAIEngine *engine, SLAIVMInvokeFn invoke, void *context);
int slai_validate_input(const SLAIInput *input);
int slai_validate_request(const SLAIRequest *request);
int slai_invoke(SLAIEngine *engine, const SLAIRequest *request,
                const SLAIInput *input, SLAIResult *result);
int slai_model_from_xml(const char *xml, size_t length, SLAIModelDescriptor *model);
int slai_data_model_from_xml(const char *xml, size_t length, SLAIDataModel *model);
int slai_source_from_xml(const char *xml, size_t length, SLAIXMLSource *source);
const char *slai_input_kind_name(SLAIInputKind kind);
const char *slai_operation_name(SLAIOperation operation);
const char *slai_backend_name(SLAIBackend backend);
const char *slai_source_kind_name(SLAIXMLSourceKind kind);

#ifdef __cplusplus
}
#endif
#endif
