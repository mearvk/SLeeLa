#define _GNU_SOURCE
#include "gardulus_ii.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define GARDULUS_DEFAULT_XML_SIZE 1024U
#define GARDULUS_MAX_ORBIT_SCALE 1.5

static uint64_t now_ms(void) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return (uint64_t)ts.tv_sec * 1000ULL + (uint64_t)ts.tv_nsec / 1000000ULL; }
static void read_memory(unsigned long *total, unsigned long *avail) {
    FILE *f = fopen("/proc/meminfo", "r"); char key[64]; unsigned long value; char unit[16]; *total = 0; *avail = 0; if (!f) return;
    while (fscanf(f, "%63s %lu %15s", key, &value, unit) == 3) { if (strcmp(key, "MemTotal:") == 0) *total = value; else if (strcmp(key, "MemAvailable:") == 0) *avail = value; }
    fclose(f);
}
static unsigned long count_tcp_lines(const char *path) { FILE *f = fopen(path, "r"); char line[512]; unsigned long n = 0; if (!f) return 0; while (fgets(line, sizeof(line), f)) n++; fclose(f); return n > 0 ? n - 1 : 0; }
static void orbit_point(double radius, double inclination_deg, double node_deg, double phase_deg, double *x, double *y, double *z) {
    double i = inclination_deg * (M_PI / 180.0), o = node_deg * (M_PI / 180.0), t = phase_deg * (M_PI / 180.0);
    double ct = cos(t), st = sin(t), ci = cos(i), co = cos(o), so = sin(o);
    *x = radius * (co * ct - so * st * ci);
    *y = radius * (so * ct + co * st * ci);
    *z = radius * (st * sin(i));
}
int gardulus_emit_json(const gardulus_sample *s, char *buffer, size_t size) {
    if (!s || !buffer || size == 0) return -1;
    return snprintf(buffer, size,
        "{\"iteration\":%llu,\"timestamp_ms\":%llu,\"input\":%d,\"p_phase\":%.6f,\"b_phase\":%.6f,\"p_start_phase\":%.6f,\"b_start_phase\":%.6f,\"p_speed_deg_s\":%.6f,\"b_speed_deg_s\":%.6f,\"p_orbital_frequency_hz\":%.9f,\"b_orbital_frequency_hz\":%.9f,\"relative_phase_deg\":%.6f,\"relative_speed_deg_s\":%.6f,\"relative_sync_ratio\":%.9f,\"sync_coherence\":%.9f,\"xml_size_bytes\":%zu,\"orbit_radius_units\":%.6f,\"p_orbit_inclination_deg\":%.6f,\"b_orbit_inclination_deg\":%.6f,\"p_orbit_x\":%.6f,\"p_orbit_y\":%.6f,\"p_orbit_z\":%.6f,\"b_orbit_x\":%.6f,\"b_orbit_y\":%.6f,\"b_orbit_z\":%.6f,\"memory_total_kb\":%lu,\"memory_available_kb\":%lu,\"connections\":%lu,\"relations\":%lu,\"items\":%lu,\"status\":%lu,\"containment_count\":%lu}\n",
        (unsigned long long)s->iteration, (unsigned long long)s->timestamp_ms, s->input_integer,
        s->p_phase, s->b_phase, s->p_start_phase, s->b_start_phase, s->p_speed_deg_s, s->b_speed_deg_s,
        s->p_orbital_frequency_hz, s->b_orbital_frequency_hz, s->relative_phase_deg, s->relative_speed_deg_s,
        s->relative_sync_ratio, s->sync_coherence, s->xml_size_bytes, s->orbit_radius_units,
        s->p_orbit_inclination_deg, s->b_orbit_inclination_deg, s->p_orbit_x, s->p_orbit_y, s->p_orbit_z,
        s->b_orbit_x, s->b_orbit_y, s->b_orbit_z, s->memory_total_kb, s->memory_available_kb,
        s->connections, s->relations, s->items, s->status, s->containment_count);
}
int gardulus_run(int interval_ms, int iterations, gardulus_listener listener, void *user_data) {
    return gardulus_run_with_xml_size(interval_ms, iterations, GARDULUS_DEFAULT_XML_SIZE, listener, user_data);
}
int gardulus_run_with_xml_size(int interval_ms, int iterations, size_t xml_size_bytes, gardulus_listener listener, void *user_data) {
    if (interval_ms < 1 || !listener || xml_size_bytes < 1) return -1;
    for (int i = 0; iterations <= 0 || i < iterations; ++i) {
        gardulus_sample s; memset(&s, 0, sizeof(s)); s.iteration = (uint64_t)i; s.timestamp_ms = now_ms(); s.input_integer = interval_ms;
        double t = (double)s.timestamp_ms / 1000.0;
        double pf = 1000.0 / (double)interval_ms, bf = 1000.0 / (double)(interval_ms + 1);
        double ps = 360.0 * pf, bs = 360.0 * bf;
        double pst = fmod((double)interval_ms * 137.507764, 360.0), bst = fmod((double)(interval_ms + 1) * 137.507764, 360.0);
        s.p_start_phase = pst; s.b_start_phase = bst; s.p_speed_deg_s = ps; s.b_speed_deg_s = bs;
        s.p_orbital_frequency_hz = pf; s.b_orbital_frequency_hz = bf;
        s.p_phase = fmod(pst + t * ps, 360.0); s.b_phase = fmod(bst + t * bs, 360.0);
        s.relative_phase_deg = fmod((s.b_phase - s.p_phase) + 540.0, 360.0) - 180.0; s.relative_speed_deg_s = bs - ps;
        s.relative_sync_ratio = bf / pf; s.sync_coherence = 1.0 - (fabs(s.relative_phase_deg) / 180.0); if (s.sync_coherence < 0.0) s.sync_coherence = 0.0;
        s.xml_size_bytes = xml_size_bytes; s.orbit_radius_units = (double)xml_size_bytes;
        if (s.orbit_radius_units > GARDULUS_MAX_ORBIT_SCALE * (double)xml_size_bytes) s.orbit_radius_units = GARDULUS_MAX_ORBIT_SCALE * (double)xml_size_bytes;
        s.p_orbit_inclination_deg = 35.0; s.b_orbit_inclination_deg = 55.0;
        orbit_point(s.orbit_radius_units, s.p_orbit_inclination_deg, 0.0, s.p_phase, &s.p_orbit_x, &s.p_orbit_y, &s.p_orbit_z);
        orbit_point(s.orbit_radius_units, s.b_orbit_inclination_deg, 25.0, s.b_phase, &s.b_orbit_x, &s.b_orbit_y, &s.b_orbit_z);
        read_memory(&s.memory_total_kb, &s.memory_available_kb); s.connections = count_tcp_lines("/proc/net/tcp"); s.relations = count_tcp_lines("/proc/net/tcp6");
        s.items = s.memory_total_kb ? (s.memory_total_kb - s.memory_available_kb) : 0; s.status = s.memory_available_kb ? 1UL : 0UL; s.containment_count = (unsigned long)(i + 1);
        if (listener(&s, user_data) != 0) return 1;
        struct timespec req = { interval_ms / 1000, (long)(interval_ms % 1000) * 1000000L }; nanosleep(&req, NULL);
    }
    return 0;
}
