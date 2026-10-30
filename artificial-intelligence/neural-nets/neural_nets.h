#ifndef SLEELA_AI_NEURAL_NETS_H
#define SLEELA_AI_NEURAL_NETS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
#  if defined(SLEELA_AI_BUILD)
#    define SLNN_API __declspec(dllexport)
#  else
#    define SLNN_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
#  define SLNN_API __attribute__((visibility("default")))
#else
#  define SLNN_API
#endif

#define SLNN_ABI_VERSION 1u

typedef struct slnn_network slnn_network;

typedef enum slnn_status {
    SLNN_OK = 0,
    SLNN_ERROR_INVALID_ARGUMENT = 1,
    SLNN_ERROR_OUT_OF_MEMORY = 2,
    SLNN_ERROR_INVALID_STATE = 3,
    SLNN_ERROR_SIZE_MISMATCH = 4,
    SLNN_ERROR_NUMERIC = 5
} slnn_status;

typedef enum slnn_activation {
    SLNN_ACTIVATION_LINEAR = 0,
    SLNN_ACTIVATION_RELU = 1,
    SLNN_ACTIVATION_TANH = 2,
    SLNN_ACTIVATION_SIGMOID = 3
} slnn_activation;

typedef struct slnn_network_config {
    uint32_t abi_version;
    size_t layer_count;             /* Number of transitions; must be nonzero. */
    const size_t *layer_sizes;      /* layer_count + 1 entries, input through output. */
    slnn_activation hidden_activation;
    slnn_activation output_activation;
    uint64_t seed;                  /* Zero selects a deterministic default seed. */
} slnn_network_config;

/* Returns a stable, non-NULL status string for every defined status. */
SLNN_API const char *slnn_status_string(slnn_status status);

/* Creates a fully-connected CPU network. The configuration is copied. */
SLNN_API slnn_status slnn_network_create(
    const slnn_network_config *config, slnn_network **out_network);

/* Destroys a network; NULL is accepted. */
SLNN_API void slnn_network_destroy(slnn_network *network);

SLNN_API size_t slnn_network_input_count(const slnn_network *network);
SLNN_API size_t slnn_network_output_count(const slnn_network *network);

/*
 * Computes one inference pass.
 * input_count/output_count must exactly match the network dimensions.
 * Input and output buffers must not overlap.
 */
SLNN_API slnn_status slnn_network_predict(
    const slnn_network *network,
    const double *input, size_t input_count,
    double *output, size_t output_count);

/*
 * Performs one online, single-sample mean-squared-error SGD update.
 * The caller owns input and target buffers. learning_rate must be finite
 * and positive. This function mutates network weights and is not thread-safe
 * when called concurrently with another operation on the same network.
 */
SLNN_API slnn_status slnn_network_train_sample(
    slnn_network *network,
    const double *input, size_t input_count,
    const double *target, size_t target_count,
    double learning_rate);

#ifdef __cplusplus
}
#endif

#endif
