#ifndef SLEELA_TIME_H
#define SLEELA_TIME_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SL_TIME_SOURCE_SYSTEM = 0,
    SL_TIME_SOURCE_NTP = 1
} SLTimeSource;

typedef enum {
    SL_TIME_LOCATION_UTC = 0,
    SL_TIME_LOCATION_HOST = 1,
    SL_TIME_LOCATION_COUNTRY = 2
} SLTimeLocationMode;

typedef struct {
    int64_t utc_ms;
    uint64_t monotonic_ns;
    int64_t utc_offset_ms;
    uint64_t uncertainty_us;
    uint32_t source;
    uint32_t stratum;
    char country[3];
    char timezone[64];
    char source_host[256];
} SLTimeSample;

int sltime_init(void);
int64_t sltime_utc_millis(void);
int64_t sltime_utc_nanos(void);
uint64_t sltime_monotonic_nanos(void);
int sltime_sample(SLTimeSample* sample);
int sltime_set_location(SLTimeLocationMode mode, const char* country_iso2, const char* iana_timezone);
const char* sltime_location_timezone(void);
const char* sltime_location_country(void);
int sltime_format_iso8601(int64_t utc_ms, char* out, size_t out_size);
int sltime_http_date(int64_t utc_ms, char* out, size_t out_size);
int sltime_query_ntp(const char* host, uint32_t timeout_ms, SLTimeSample* sample);
int sltime_json(const SLTimeSample* sample, char* out, size_t out_size);
int sltime_rmi_record(const SLTimeSample* sample, char* out, size_t out_size);
size_t sltime_bodi_record(const SLTimeSample* sample, uint8_t* out, size_t out_size);
/* Send a standard NTP request and an optional one-byte interoperability marker. */
int sltime_send_raw_time(const char* host, uint16_t port, uint8_t marker, uint32_t timeout_ms, SLTimeSample* sample);

#ifdef __cplusplus
}
#endif
#endif
