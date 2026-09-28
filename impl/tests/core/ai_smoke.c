#include <stdio.h>
#include <string.h>
#include "sleela_ai.h"

static int vm_stub(const SLAIRequest *request, const SLAIInput *input,
                   SLAIResult *result, void *context) {
    (void)context;
    result->accepted = 1;
    result->completed = 1;
    result->request_id = 1;
    result->observation_count = 1;
    snprintf(result->summary, sizeof(result->summary), "%s:%s",
             slai_operation_name(request->operation), slai_input_kind_name(input->kind));
    return 0;
}

int main(void) {
    SLAIEngine engine;
    SLAIRequest request = { SL_AI_OP_INSPECT, SL_AI_BACKEND_CONNECTOR,
                            "sleela.example", "inspect", 0, NULL, 0 };
    const char bytes[] = "SLeeLa AI";
    SLAIInput input = { SL_AI_KIND_DATA, "sample", NULL, bytes, sizeof(bytes)-1, NULL };
    SLAIResult result;
    const char model_xml[] = "<model id=\"sleela.example\" revision=\"1\" format=\"sleela-ai-1\" source=\"native\" trusted=\"true\"/>";
    const char data_xml[] = "<dataset id=\"demo\" revision=\"1\"><field name=\"kind\" type=\"string\" required=\"true\"/></dataset>";
    SLAIModelDescriptor descriptor;
    SLAIDataModel data_model;
    SLAIXMLSource source;
    if (slai_model_from_xml(model_xml, sizeof(model_xml)-1, &descriptor)) return 1;
    if (!descriptor.trusted || strcmp(descriptor.model_id, "sleela.example")) return 2;
    if (slai_data_model_from_xml(data_xml, sizeof(data_xml)-1, &data_model)) return 3;
    if (data_model.field_count != 1 || strcmp(data_model.fields[0].name, "kind")) return 4;
    {
        const char source_xml[] = "<source kind=\"audio\" url=\"file:///media/input.xml\" args=\"model=audio-v1&flow=inspect\"/>";
        if (slai_source_from_xml(source_xml, sizeof(source_xml)-1, &source)) return 5;
        if (source.kind != SL_AI_SOURCE_AUDIO || strcmp(source.url, "file:///media/input.xml") ||
            strcmp(source.args, "model=audio-v1&flow=inspect")) return 6;
    }
    if (slai_engine_init(&engine, SL_AI_BACKEND_CONNECTOR)) return 5;
    if (slai_engine_bind_vm(&engine, vm_stub, NULL)) return 6;
    if (slai_invoke(&engine, &request, &input, &result)) return 7;
    if (!result.accepted || !result.completed || strcmp(result.summary, "inspect:data")) return 8;
    if (slai_engine_init(&engine, SL_AI_BACKEND_NATIVE)) return 9;\n    if (slai_engine_bind_native(&engine, vm_stub, NULL)) return 10;\n    request.backend = SL_AI_BACKEND_NATIVE;\n    if (slai_invoke(&engine, &request, &input, &result)) return 11;\n    puts("AI API: PASS");
    return 0;
}
