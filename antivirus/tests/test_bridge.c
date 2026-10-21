#include "sleelavirin_bridge.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    sleelavirin_bridge_request_t request;
    sleelavirin_bridge_result_t result;

    memset(&request, 0, sizeof(request));
    snprintf(request.target_path, sizeof(request.target_path), "tests/native_sample.txt");
    snprintf(request.database_path, sizeof(request.database_path), "tests/native-sha256.txt");
    snprintf(request.engine_name, sizeof(request.engine_name), "native");
    snprintf(request.mode, sizeof(request.mode), "single-file");
    request.scope = SLEELAVIRIN_SCOPE_LOCAL;
    request.recursive = 0;
    request.follow_links = 0;
    request.max_file_size_bytes = 268435456ULL;
    request.timeout_seconds = 120U;
    request.require_admin = 0;

    assert(sleelavirin_bridge_validate_request(&request) == 0);
    assert(sleelavirin_bridge_scan_native(&request, &result) == 0);
    assert(strcmp(result.verdict, "DETECTED") == 0);
    assert(result.detections == 1U);
    printf("Bridge verdict=%s detail=%s\n", result.verdict, result.detail);
    return 0;
}
