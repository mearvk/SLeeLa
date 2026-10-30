#include "neural_nets.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

struct slnn_network {
    size_t layer_count;
    size_t *sizes;
    size_t *weight_offsets;
    size_t *bias_offsets;
    double *weights;
    double *biases;
    double *activations;
    double *deltas;
    double *preactivations;
    size_t total_neurons;
    size_t total_weights;
    slnn_activation hidden_activation;
    slnn_activation output_activation;
};

static int valid_activation(slnn_activation a) {
    return a >= SLNN_ACTIVATION_LINEAR && a <= SLNN_ACTIVATION_SIGMOID;
}

static int finite_array(const double *values, size_t count) {
    size_t i;
    if (!values) return 0;
    for (i = 0; i < count; ++i) if (!isfinite(values[i])) return 0;
    return 1;
}

static double activate(double x, slnn_activation a) {
    switch (a) {
        case SLNN_ACTIVATION_RELU: return x > 0.0 ? x : 0.0;
        case SLNN_ACTIVATION_TANH: return tanh(x);
        case SLNN_ACTIVATION_SIGMOID:
            if (x >= 0.0) { double z = exp(-x); return 1.0 / (1.0 + z); }
            else { double z = exp(x); return z / (1.0 + z); }
        case SLNN_ACTIVATION_LINEAR:
        default: return x;
    }
}

static double derivative(double z, double y, slnn_activation a) {
    switch (a) {
        case SLNN_ACTIVATION_RELU: return z > 0.0 ? 1.0 : 0.0;
        case SLNN_ACTIVATION_TANH: return 1.0 - y * y;
        case SLNN_ACTIVATION_SIGMOID: return y * (1.0 - y);
        case SLNN_ACTIVATION_LINEAR:
        default: return 1.0;
    }
}

static uint64_t next_random(uint64_t *state) {
    uint64_t x = *state;
    if (x == 0) x = UINT64_C(0x9e3779b97f4a7c15);
    x ^= x >> 12; x ^= x << 25; x ^= x >> 27;
    *state = x;
    return x * UINT64_C(2685821657736338717);
}

static double random_weight(uint64_t *state, size_t fan_in) {
    double unit = (double)(next_random(state) >> 11) / 9007199254740992.0;
    double scale = fan_in ? sqrt(2.0 / (double)fan_in) : 1.0;
    return (unit * 2.0 - 1.0) * scale;
}

const char *slnn_status_string(slnn_status status) {
    switch (status) {
        case SLNN_OK: return "success";
        case SLNN_ERROR_INVALID_ARGUMENT: return "invalid argument";
        case SLNN_ERROR_OUT_OF_MEMORY: return "out of memory";
        case SLNN_ERROR_INVALID_STATE: return "invalid state";
        case SLNN_ERROR_SIZE_MISMATCH: return "buffer size mismatch";
        case SLNN_ERROR_NUMERIC: return "non-finite numeric value";
        default: return "unknown status";
    }
}

void slnn_network_destroy(slnn_network *n) {
    if (!n) return;
    free(n->sizes); free(n->weight_offsets); free(n->bias_offsets);
    free(n->weights); free(n->biases); free(n->activations);
    free(n->deltas); free(n->preactivations); free(n);
}

slnn_status slnn_network_create(const slnn_network_config *c, slnn_network **out) {
    slnn_network *n = NULL;
    size_t i, neurons = 0, weights = 0;
    uint64_t rng;
    if (!out) return SLNN_ERROR_INVALID_ARGUMENT;
    *out = NULL;
    if (!c || c->abi_version != SLNN_ABI_VERSION || !c->layer_sizes ||
        c->layer_count == 0 || !valid_activation(c->hidden_activation) ||
        !valid_activation(c->output_activation))
        return SLNN_ERROR_INVALID_ARGUMENT;
    if (c->layer_count > 1024) return SLNN_ERROR_INVALID_ARGUMENT;
    for (i = 0; i <= c->layer_count; ++i) {
        size_t s = c->layer_sizes[i];
        if (s == 0 || s > 100000 || neurons > SIZE_MAX - s)
            return SLNN_ERROR_INVALID_ARGUMENT;
        neurons += s;
        if (i && c->layer_sizes[i - 1] > SIZE_MAX / s)
            return SLNN_ERROR_INVALID_ARGUMENT;
        if (i) {
            size_t count = c->layer_sizes[i - 1] * s;
            if (weights > SIZE_MAX - count) return SLNN_ERROR_INVALID_ARGUMENT;
            weights += count;
        }
    }
    n = (slnn_network *)calloc(1, sizeof(*n));
    if (!n) return SLNN_ERROR_OUT_OF_MEMORY;
    n->layer_count = c->layer_count; n->total_neurons = neurons; n->total_weights = weights;
    n->hidden_activation = c->hidden_activation; n->output_activation = c->output_activation;
    n->sizes = (size_t *)calloc(c->layer_count + 1, sizeof(size_t));
    n->weight_offsets = (size_t *)calloc(c->layer_count, sizeof(size_t));
    n->bias_offsets = (size_t *)calloc(c->layer_count, sizeof(size_t));
    n->weights = (double *)calloc(weights, sizeof(double));
    n->biases = (double *)calloc(neurons - c->layer_sizes[0], sizeof(double));
    n->activations = (double *)calloc(neurons, sizeof(double));
    n->deltas = (double *)calloc(neurons, sizeof(double));
    n->preactivations = (double *)calloc(neurons, sizeof(double));
    if (!n->sizes || !n->weight_offsets || !n->bias_offsets || !n->weights ||
        !n->biases || !n->activations || !n->deltas || !n->preactivations) {
        slnn_network_destroy(n); return SLNN_ERROR_OUT_OF_MEMORY;
    }
    memcpy(n->sizes, c->layer_sizes, (c->layer_count + 1) * sizeof(size_t));
    {
        size_t neuron_offset = 0, weight_offset = 0, bias_offset = 0;
        for (i = 0; i < c->layer_count; ++i) {
            n->weight_offsets[i] = weight_offset;
            n->bias_offsets[i] = bias_offset;
            neuron_offset += n->sizes[i];
            weight_offset += n->sizes[i] * n->sizes[i + 1];
            bias_offset += n->sizes[i + 1];
        }
    }
    rng = c->seed ? c->seed : UINT64_C(0x534c4e4e5f763031);
    for (i = 0; i < c->layer_count; ++i) {
        size_t j, count = n->sizes[i] * n->sizes[i + 1];
        for (j = 0; j < count; ++j)
            n->weights[n->weight_offsets[i] + j] = random_weight(&rng, n->sizes[i]);
    }
    *out = n;
    return SLNN_OK;
}

size_t slnn_network_input_count(const slnn_network *n) { return n ? n->sizes[0] : 0; }
size_t slnn_network_output_count(const slnn_network *n) { return n ? n->sizes[n->layer_count] : 0; }

static slnn_status forward(slnn_network *n, const double *input) {
    size_t i, j, k, neuron_base = 0;
    memcpy(n->activations, input, n->sizes[0] * sizeof(double));
    for (i = 0; i < n->layer_count; ++i) {
        size_t in = n->sizes[i], out = n->sizes[i + 1];
        size_t prev_base = neuron_base;
        size_t next_base = neuron_base + in;
        slnn_activation act = (i + 1 == n->layer_count) ? n->output_activation : n->hidden_activation;
        for (j = 0; j < out; ++j) {
            double sum = n->biases[n->bias_offsets[i] + j];
            for (k = 0; k < in; ++k)
                sum += n->weights[n->weight_offsets[i] + j * in + k] *
                       n->activations[prev_base + k];
            n->preactivations[next_base + j] = sum;
            n->activations[next_base + j] = activate(sum, act);
        }
        neuron_base = next_base;
    }
    return finite_array(n->activations, n->total_neurons) ?
        SLNN_OK : SLNN_ERROR_NUMERIC;
}

slnn_status slnn_network_predict(const slnn_network *network,
    const double *input, size_t input_count, double *output, size_t output_count) {
    slnn_network *n = (slnn_network *)network;
    slnn_status status;
    size_t out_base = 0, i;
    if (!n || !input || !output) return SLNN_ERROR_INVALID_ARGUMENT;
    if (input_count != n->sizes[0] || output_count != n->sizes[n->layer_count])
        return SLNN_ERROR_SIZE_MISMATCH;
    if (!finite_array(input, input_count)) return SLNN_ERROR_NUMERIC;
    status = forward(n, input);
    if (status != SLNN_OK) return status;
    for (i = 0; i < n->layer_count; ++i) out_base += n->sizes[i];
    memcpy(output, n->activations + out_base, output_count * sizeof(double));
    return SLNN_OK;
}

slnn_status slnn_network_train_sample(slnn_network *n,
    const double *input, size_t input_count, const double *target,
    size_t target_count, double rate) {
    size_t i, j, k, base = 0, out_base = 0;
    slnn_status status;
    if (!n || !input || !target || !isfinite(rate) || rate <= 0.0)
        return SLNN_ERROR_INVALID_ARGUMENT;
    if (input_count != n->sizes[0] || target_count != n->sizes[n->layer_count])
        return SLNN_ERROR_SIZE_MISMATCH;
    if (!finite_array(input, input_count) || !finite_array(target, target_count))
        return SLNN_ERROR_NUMERIC;
    status = forward(n, input);
    if (status != SLNN_OK) return status;
    for (i = 0; i < n->layer_count; ++i) out_base += n->sizes[i];
    {
        size_t out = n->sizes[n->layer_count];
        slnn_activation a = n->output_activation;
        for (j = 0; j < out; ++j) {
            double y = n->activations[out_base + j];
            n->deltas[out_base + j] = (y - target[j]) *
                derivative(n->preactivations[out_base + j], y, a);
        }
    }
    base = out_base;
    for (i = n->layer_count; i-- > 1;) {
        size_t layer_base = 0, next_base = 0, width = n->sizes[i];
        size_t next_width = n->sizes[i + 1];
        for (j = 0; j < i; ++j) layer_base += n->sizes[j];
        next_base = layer_base + width;
        for (j = 0; j < width; ++j) {
            double sum = 0.0;
            for (k = 0; k < next_width; ++k)
                sum += n->weights[n->weight_offsets[i] + k * width + j] *
                       n->deltas[next_base + k];
            n->deltas[layer_base + j] = sum * derivative(
                n->preactivations[layer_base + j],
                n->activations[layer_base + j], n->hidden_activation);
        }
    }
    for (i = 0; i < n->layer_count; ++i) {
        size_t in = n->sizes[i], out = n->sizes[i + 1], prev_base = 0, next_base;
        for (j = 0; j < i; ++j) prev_base += n->sizes[j];
        next_base = prev_base + in;
        for (j = 0; j < out; ++j) {
            double delta = n->deltas[next_base + j];
            n->biases[n->bias_offsets[i] + j] -= rate * delta;
            for (k = 0; k < in; ++k)
                n->weights[n->weight_offsets[i] + j * in + k] -=
                    rate * delta * n->activations[prev_base + k];
        }
    }
    (void)base;
    return finite_array(n->weights, n->total_weights) ? SLNN_OK : SLNN_ERROR_NUMERIC;
}
