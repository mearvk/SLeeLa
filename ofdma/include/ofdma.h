/* SLeeLa OFDMA reference stack — C11 public API.
 * Educational resource-grid model; not a standards-conformance implementation.
 * Max Rupplin - MEARVK LLC - 2026
 */
#ifndef SLEELA_OFDMA_H
#define SLEELA_OFDMA_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define OFDMA_MAX_USERS 64u
typedef enum { OFDMA_OK = 0, OFDMA_ERR_ARGUMENT = -1, OFDMA_ERR_RANGE = -2, OFDMA_ERR_CAPACITY = -3 } ofdma_status;
typedef struct {
    uint32_t fft_size, active_subcarriers, subcarrier_spacing_hz;
    uint32_t symbols_per_slot, cyclic_prefix_samples, resource_block_subcarriers;
} ofdma_numerology;
typedef struct { uint32_t resource_blocks, symbols, subcarriers_per_rb; } ofdma_grid;
typedef struct { uint32_t user_id, requested_rbs, allocated_rbs; } ofdma_user_request;
typedef struct {
    double useful_symbol_seconds, sample_rate_hz;
    uint64_t grid_resource_elements;
    uint32_t resource_blocks;
} ofdma_metrics;
ofdma_status ofdma_validate_numerology(const ofdma_numerology *n);
ofdma_status ofdma_calculate_metrics(const ofdma_numerology *n, uint32_t grid_symbols, ofdma_metrics *out);
ofdma_status ofdma_schedule_round_robin(ofdma_user_request *users, size_t user_count, uint32_t available_rbs, uint32_t *allocated_total);
#ifdef __cplusplus
}
#endif
#endif
