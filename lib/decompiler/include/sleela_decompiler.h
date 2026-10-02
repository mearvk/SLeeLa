#ifndef SLEELA_DECOMPILER_H
#define SLEELA_DECOMPILER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SLEELA_FRACTIONAL_PRESERVE = 0,
    SLEELA_FRACTIONAL_PARTIAL = 1,
    SLEELA_FRACTIONAL_REPORT_ONLY = 2,
    SLEELA_FRACTIONAL_STRICT = 3
} sleela_fractional_policy;

typedef struct {
    const char *source_language;
    const char *source_version;
    const char *expected_input;
    const char *desired_output;
    const char *os_policy;
    sleela_fractional_policy fractional_policy;
    int analysis_depth;
    int preserve_evidence;
} sleela_decompiler_request;

typedef struct {
    int format_score;
    int os_score;
    int architecture_score;
    int evidence_count;
    int unresolved_regions;
    int conflicting_evidence;
} sleela_decompiler_report;

int sleela_decompiler_validate_request(const sleela_decompiler_request *request);
int sleela_decompiler_score_os_evidence(const unsigned char *data, size_t size,
                                        sleela_decompiler_report *report);
int sleela_decompiler_preserve_fractional(sleela_decompiler_report *report,
                                           size_t valid_bytes, size_t total_bytes,
                                           sleela_fractional_policy policy);

#ifdef __cplusplus
}
#endif

#endif
