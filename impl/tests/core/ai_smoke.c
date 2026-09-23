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
    if (slai_engine_init(&engine, SL_AI_BACKEND_CONNECTOR)) return 1;
    if (slai_engine_bind_vm(&engine, vm_stub, NULL)) return 2;
    if (slai_invoke(&engine, &request, &input, &result)) return 3;
    if (!result.accepted || !result.completed || strcmp(result.summary, "inspect:data")) return 4;
    puts("AI API: PASS");
    return 0;
}
