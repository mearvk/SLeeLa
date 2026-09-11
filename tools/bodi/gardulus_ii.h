#ifndef GARDULUS_II_H
#define GARDULUS_II_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gardulus_sample {
    uint64_t iteration;
    uint64_t timestamp_ms;
    int input_integer;
    double p_phase;
    double b_phase;
    double p_start_phase;
    double b_start_phase;
    double p_speed_deg_s;
    double b_speed_deg_s;
    double p_orbital_frequency_hz;
    double b_orbital_frequency_hz;
    double relative_phase_deg;
    double relative_speed_deg_s;
    double relative_sync_ratio;
    double sync_coherence;
    unsigned long memory_total_kb;
    unsigned long memory_available_kb;
    unsigned long connections;
    unsigned long relations;
    unsigned long items;
    unsigned long status;
    unsigned long containment_count;
} gardulus_sample;

typedef int (*gardulus_listener)(const gardulus_sample *sample, void *user_data);

int gardulus_emit_json(const gardulus_sample *sample, char *buffer, size_t size);
int gardulus_run(int interval_ms, int iterations, gardulus_listener listener, void *user_data);

#ifdef __cplusplus
}
#endif

#endif
