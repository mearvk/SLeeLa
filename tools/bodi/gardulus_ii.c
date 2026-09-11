#define _GNU_SOURCE
#include "gardulus_ii.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

static uint64_t now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000ULL + (uint64_t)ts.tv_nsec / 1000000ULL;
}

static void read_memory(unsigned long *total, unsigned long *avail) {
    FILE *f = fopen("/proc/meminfo", "r");
    char key[64]; unsigned long value; char unit[16];
    *total = 0; *avail = 0;
    if (!f) return;
    while (fscanf(f, "%63s %lu %15s", key, &value, unit) == 3) {
        if (strcmp(key, "MemTotal:") == 0) *total = value;
        else if (strcmp(key, "MemAvailable:") == 0) *avail = value;
    }
    fclose(f);
}

static unsigned long count_tcp_lines(const char *path) {
    FILE *f = fopen(path, "r"); char line[512]; unsigned long n = 0;
    if (!f) return 0;
    while (fgets(line, sizeof(line), f)) n++;
    fclose(f);
    return n > 0 ? n - 1 : 0;
}

int gardulus_emit_json(const gardulus_sample *s, char *buffer, size_t size) {
    if (!s || !buffer || size == 0) return -1;
    return snprintf(buffer, size,
        "{\"iteration\":%llu,\"timestamp_ms\":%llu,\"input\":%d,\"p_phase\":%.6f,\"b_phase\":%.6f,\"p_start_phase\":%.6f,\"b_start_phase\":%.6f,\"p_speed_deg_s\":%.6f,\"b_speed_deg_s\":%.6f,\"p_orbital_frequency_hz\":%.9f,\"b_orbital_frequency_hz\":%.9f,\"relative_phase_deg\":%.6f,\"relative_speed_deg_s\":%.6f,\"relative_sync_ratio\":%.9f,\"sync_coherence\":%.9f,\"memory_total_kb\":%lu,\"memory_available_kb\":%lu,\"connections\":%lu,\"relations\":%lu,\"items\":%lu,\"status\":%lu,\"containment_count\":%lu}\n",
        (unsigned long long)s->iteration, (unsigned long long)s->timestamp_ms,
        s->input_integer, s->p_phase, s->b_phase, s->p_start_phase,
        s->b_start_phase, s->p_speed_deg_s, s->b_speed_deg_s,
        s->p_orbital_frequency_hz, s->b_orbital_frequency_hz,
        s->relative_phase_deg, s->relative_speed_deg_s,
        s->relative_sync_ratio, s->sync_coherence,
        s->memory_total_kb, s->memory_available_kb, s->connections,
        s->relations, s->items, s->status, s->containment_count);
}

int gardulus_run(int interval_ms, int iterations, gardulus_listener listener, void *user_data) {
    if (interval_ms < 1 || !listener) return -1;
    for (int i = 0; iterations <= 0 || i < iterations; ++i) {
        gardulus_sample s;
        memset(&s, 0, sizeof(s));
        s.iteration = (uint64_t)i;
        s.timestamp_ms = now_ms();
        s.input_integer = interval_ms;

        double t = (double)s.timestamp_ms / 1000.0;
        double p_frequency = 1000.0 / (double)interval_ms;
        double b_frequency = 1000.0 / (double)(interval_ms + 1);
        double p_speed = 360.0 * p_frequency;
        double b_speed = 360.0 * b_frequency;
        double p_start = fmod((double)interval_ms * 137.507764, 360.0);
        double b_start = fmod((double)(interval_ms + 1) * 137.507764, 360.0);

        s.p_start_phase = p_start;
        s.b_start_phase = b_start;
        s.p_speed_deg_s = p_speed;
        s.b_speed_deg_s = b_speed;
        s.p_orbital_frequency_hz = p_frequency;
        s.b_orbital_frequency_hz = b_frequency;
        s.p_phase = fmod(p_start + t * p_speed, 360.0);
        s.b_phase = fmod(b_start + t * b_speed, 360.0);
        s.relative_phase_deg = fmod((s.b_phase - s.p_phase) + 540.0, 360.0) - 180.0;
        s.relative_speed_deg_s = b_speed - p_speed;
        s.relative_sync_ratio = b_frequency / p_frequency;
        s.sync_coherence = 1.0 - (fabs(s.relative_phase_deg) / 180.0);
        if (s.sync_coherence < 0.0) s.sync_coherence = 0.0;

        read_memory(&s.memory_total_kb, &s.memory_available_kb);
        s.connections = count_tcp_lines("/proc/net/tcp");
        s.relations = count_tcp_lines("/proc/net/tcp6");
        s.items = s.memory_total_kb ? (s.memory_total_kb - s.memory_available_kb) : 0;
        s.status = (s.memory_available_kb > 0) ? 1UL : 0UL;
        s.containment_count = (unsigned long)(i + 1);
        if (listener(&s, user_data) != 0) return 1;

        struct timespec req = { interval_ms / 1000, (long)(interval_ms % 1000) * 1000000L };
        nanosleep(&req, NULL);
    }
    return 0;
}
