#ifndef SLEELA_DATA_ANALYTICS_H
#define SLEELA_DATA_ANALYTICS_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLEELA_ANALYTICS_MAX_VALUES 1024
typedef struct { const double *values; size_t count; } sleela_data_analytics_set_t;
int sleela_data_analytics_validate(const sleela_data_analytics_set_t *, char *, size_t);
int sleela_data_analytics_sum(const sleela_data_analytics_set_t *, double *);
int sleela_data_analytics_mean(const sleela_data_analytics_set_t *, double *);
int sleela_data_analytics_min(const sleela_data_analytics_set_t *, double *);
int sleela_data_analytics_max(const sleela_data_analytics_set_t *, double *);
int sleela_data_analytics_variance(const sleela_data_analytics_set_t *, double *);
int sleela_data_analytics_stddev(const sleela_data_analytics_set_t *, double *);
int sleela_data_analytics_correlation(const sleela_data_analytics_set_t *, const sleela_data_analytics_set_t *, double *);
#ifdef __cplusplus
}
#endif
#endif
