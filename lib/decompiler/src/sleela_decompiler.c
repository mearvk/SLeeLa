#include "sleela_decompiler.h"
#include <string.h>

int sleela_decompiler_validate_request(const sleela_decompiler_request *request) {
    if (!request || !request->source_language || !request->desired_output) return 0;
    if (request->fractional_policy < SLEELA_FRACTIONAL_PRESERVE ||
        request->fractional_policy > SLEELA_FRACTIONAL_STRICT) return 0;
    if (request->analysis_depth < 0) return 0;
    return 1;
}

int sleela_decompiler_score_os_evidence(const unsigned char *data, size_t size,
                                        sleela_decompiler_report *report) {
    if (!data || !report) return 0;
    memset(report, 0, sizeof(*report));
    report->evidence_count = size > 0 ? 1 : 0;

    /* Conservative format observations. Language/OS modules add stronger evidence. */
    if (size >= 2 && data[0] == 'M' && data[1] == 'Z') report->os_score += 20;
    if (size >= 4 && data[0] == 0x7f && data[1] == 'E' &&
        data[2] == 'L' && data[3] == 'F') report->os_score += 20;
    if (size >= 4 && data[0] == 0xFE && data[1] == 0xED) report->os_score += 10;
    report->format_score = report->os_score;
    return 1;
}

int sleela_decompiler_preserve_fractional(sleela_decompiler_report *report,
                                           size_t valid_bytes, size_t total_bytes,
                                           sleela_fractional_policy policy) {
    if (!report || valid_bytes > total_bytes) return 0;
    if (valid_bytes == total_bytes) {
        report->unresolved_regions = 0;
        return 1;
    }
    report->unresolved_regions = 1;
    return policy != SLEELA_FRACTIONAL_STRICT;
}
