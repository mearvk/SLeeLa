#include "ofdma.h"
ofdma_status ofdma_validate_numerology(const ofdma_numerology *n) {
    if (!n) return OFDMA_ERR_ARGUMENT;
    if (!n->fft_size || !n->active_subcarriers || !n->subcarrier_spacing_hz ||
        !n->symbols_per_slot || !n->resource_block_subcarriers) return OFDMA_ERR_ARGUMENT;
    if (n->active_subcarriers > n->fft_size ||
        n->active_subcarriers % n->resource_block_subcarriers != 0) return OFDMA_ERR_RANGE;
    if (n->cyclic_prefix_samples >= n->fft_size) return OFDMA_ERR_RANGE;
    return OFDMA_OK;
}
ofdma_status ofdma_calculate_metrics(const ofdma_numerology *n, uint32_t grid_symbols, ofdma_metrics *out) {
    if (!out || !grid_symbols) return OFDMA_ERR_ARGUMENT;
    ofdma_status status = ofdma_validate_numerology(n);
    if (status != OFDMA_OK) return status;
    out->useful_symbol_seconds = 1.0 / (double)n->subcarrier_spacing_hz;
    out->sample_rate_hz = (double)n->fft_size * n->subcarrier_spacing_hz;
    out->resource_blocks = n->active_subcarriers / n->resource_block_subcarriers;
    out->grid_resource_elements = (uint64_t)n->active_subcarriers * grid_symbols;
    return OFDMA_OK;
}
ofdma_status ofdma_schedule_round_robin(ofdma_user_request *users, size_t user_count, uint32_t available_rbs, uint32_t *allocated_total) {
    if (!users || !allocated_total || !user_count) return OFDMA_ERR_ARGUMENT;
    if (user_count > OFDMA_MAX_USERS) return OFDMA_ERR_CAPACITY;
    for (size_t i = 0; i < user_count; ++i) users[i].allocated_rbs = 0;
    uint32_t remaining = available_rbs;
    while (remaining) {
        int progress = 0;
        for (size_t i = 0; i < user_count && remaining; ++i) {
            if (users[i].allocated_rbs < users[i].requested_rbs) {
                ++users[i].allocated_rbs; --remaining; progress = 1;
            }
        }
        if (!progress) break;
    }
    *allocated_total = available_rbs - remaining;
    return OFDMA_OK;
}
