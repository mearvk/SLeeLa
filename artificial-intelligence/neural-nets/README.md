# Neural Networks

Portable native neural-network API for SLeeLa, with a C ABI and a C++17 facade.

## Public files

| File | Purpose |
|---|---|
| `neural_nets.h` | Stable C declarations, status codes, configuration, and opaque network handle |
| `neural_nets.c` | Dependency-light CPU implementation |
| `neural_nets.hpp` | C++17 RAII interface |
| `neural_nets.cpp` | C++ facade implementation |
| `xor_example.c` | Deterministic XOR learning example and regression check |

## Current capabilities

- Fully-connected feed-forward networks with configurable layer widths
- Linear, ReLU, tanh, and sigmoid activation functions
- Deterministic seeded initialization
- CPU inference for one input sample
- Online, single-sample mean-squared-error SGD training
- C ABI suitable for use by C, C++, and foreign-function bindings

## C example

```c
#include "neural_nets.h"

size_t layers[] = { 2, 8, 1 };
slnn_network_config config = {
    SLNN_ABI_VERSION, 2, layers,
    SLNN_ACTIVATION_RELU, SLNN_ACTIVATION_LINEAR, 42
};
slnn_network *network = NULL;

if (slnn_network_create(&config, &network) == SLNN_OK) {
    double input[] = { 0.25, 0.75 };
    double output[1];
    if (slnn_network_predict(network, input, 2, output, 1) == SLNN_OK) {
        /* Consume output[0]. */
    }
}
slnn_network_destroy(network);
```

## Build

Compile the C implementation as C11 and link the platform math library where required:

```sh
cc -std=c11 -O2 -c neural_nets.c -o neural_nets.o
c++ -std=c++17 -O2 -c neural_nets.cpp -o neural_nets_cpp.o
```

Link the C++ facade with the C object and the platform math library (often `-lm` on Unix-like systems).

## XOR learning example

`xor_example.c` trains a seeded 2–4–1 sigmoid network on the four XOR
samples using the public single-sample SGD API. It runs 5,000 epochs and
checks that both expected-zero outputs are at most `0.20` and both expected-one
outputs are at least `0.80`. It prints all four predictions and exits nonzero
if training or classification fails.

From the repository root, run the integrated test with:

```sh
make -C tests ai-neural-nets-xor
```

The same target is included in the native portability workflow for Linux,
macOS, and Windows/MinGW-w64.

## Contract and limitations

- The configuration and layer-size array are validated and copied during creation.
- The caller owns input/output arrays; their lengths must exactly match the network.
- A network owns its weights and scratch buffers. Destroy it with `slnn_network_destroy`.
- Operations on one network instance are not thread-safe; use separate instances or external synchronization.
- Training updates weights in place and currently supports one sample per call.
- This is a foundational CPU implementation, not a general tensor framework. GPU acceleration, batching, optimizers beyond SGD, persistence, and automatic differentiation are future extensions.
- No third-party model or dataset is bundled.
