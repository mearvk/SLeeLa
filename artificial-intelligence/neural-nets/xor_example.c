/*
 * Deterministic XOR learning example for SLeeLa's CPU neural-network API.
 *
 * Build from the repository root:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic \
 *     -I artificial-intelligence/neural-nets \
 *     artificial-intelligence/neural-nets/xor_example.c \
 *     artificial-intelligence/neural-nets/neural_nets.c -lm -o xor-example
 *
 * Run:
 *   ./xor-example
 */
#include "neural_nets.h"

#include <math.h>
#include <stdio.h>

int main(void) {
    static const size_t layer_sizes[] = { 2, 4, 1 };
    static const double inputs[4][2] = {
        { 0.0, 0.0 },
        { 0.0, 1.0 },
        { 1.0, 0.0 },
        { 1.0, 1.0 }
    };
    static const double targets[4][1] = {
        { 0.0 },
        { 1.0 },
        { 1.0 },
        { 0.0 }
    };
    const size_t epochs = 5000;
    const double learning_rate = 0.5;
    const double low_limit = 0.2;
    const double high_limit = 0.8;
    slnn_network_config config = {
        SLNN_ABI_VERSION,
        2,
        layer_sizes,
        SLNN_ACTIVATION_SIGMOID,
        SLNN_ACTIVATION_SIGMOID,
        UINT64_C(42)
    };
    slnn_network *network = NULL;
    slnn_status status;
    size_t epoch, sample;
    int passed = 1;

    status = slnn_network_create(&config, &network);
    if (status != SLNN_OK) {
        fprintf(stderr, "XOR: network creation failed: %s\n",
                slnn_status_string(status));
        return 1;
    }

    for (epoch = 0; epoch < epochs; ++epoch) {
        for (sample = 0; sample < 4; ++sample) {
            status = slnn_network_train_sample(
                network, inputs[sample], 2, targets[sample], 1,
                learning_rate);
            if (status != SLNN_OK) {
                fprintf(stderr,
                        "XOR: training failed at epoch %zu, sample %zu: %s\n",
                        epoch, sample, slnn_status_string(status));
                slnn_network_destroy(network);
                return 1;
            }
        }
    }

    puts("XOR results (expected -> predicted):");
    for (sample = 0; sample < 4; ++sample) {
        double output = 0.0;
        status = slnn_network_predict(
            network, inputs[sample], 2, &output, 1);
        if (status != SLNN_OK || !isfinite(output)) {
            fprintf(stderr, "XOR: prediction failed for sample %zu: %s\n",
                    sample, slnn_status_string(status));
            slnn_network_destroy(network);
            return 1;
        }

        printf("%.0f XOR %.0f -> %.0f (%.6f)\n",
               inputs[sample][0], inputs[sample][1],
               targets[sample][0], output);

        if (targets[sample][0] == 1.0) {
            if (output < high_limit) passed = 0;
        } else if (output > low_limit) {
            passed = 0;
        }
    }

    slnn_network_destroy(network);

    if (!passed) {
        fprintf(stderr,
                "XOR: FAIL (expected 0 outputs <= %.2f and 1 outputs >= %.2f)\n",
                low_limit, high_limit);
        return 1;
    }

    printf("XOR: PASS (seed=42, epochs=%zu, learning_rate=%.2f)\n",
           epochs, learning_rate);
    return 0;
}
