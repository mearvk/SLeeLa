# SLeeLa Neural Networks

SLeeLa-facing package for the native CPU neural-network implementation in
`artificial-intelligence/neural-nets/`. The native C ABI remains authoritative
for allocation, inference, and SGD updates. The SLeeLa classes validate and
retain language-level configuration; they do not pretend to execute native
kernels when no host adapter is installed.

## Public units

- `SLPackage` — package identity and native ABI contract.
- `SLNeuralNetworkConfig` — layer widths, activation choices, and seed.
- `SLNeuralNetwork` — configuration state and inference/training contract.

## Native bridge contract

The adapter must verify `SLNN_ABI_VERSION == 1`, widen the SLeeLa 32-bit seed
to the native `uint64_t` seed field, convert layer widths to `size_t`, and
map activation values without reordering them. It owns the opaque
`slnn_network *` handle and must destroy it exactly once. Map native status
codes 0–5 directly; status 6 is SLeeLa's `STATUS_NATIVE_ADAPTER_REQUIRED`
and is not a native ABI status.

Compile/link `neural_nets.c` (C11) and, when using C++, `neural_nets.cpp`
(C++17). The network is fully connected and CPU-backed. It supports linear,
ReLU, tanh, and sigmoid activations, deterministic seeded initialization,
single-sample inference, and online single-sample MSE/SGD training. A single
network instance is not thread-safe. GPU execution, batching, persistence, and
automatic differentiation are not part of this ABI.

## Example

```sleela
#sleela 3.0
material;
SLNeuralNetworkConfig config = new SLNeuralNetworkConfig();
config.setLayerSizes(new int[]{2, 8, 1});
config.setHiddenActivation(SLNeuralNetworkConfig.ACTIVATION_RELU);
config.setOutputActivation(SLNeuralNetworkConfig.ACTIVATION_LINEAR);
config.setSeed(42);
SLNeuralNetwork network = new SLNeuralNetwork();
int status = network.configure(config);
```

The example validates and configures the SLeeLa-side contract. Until the
embedding runtime registers the native adapter, `predict` and `trainSample`
return `STATUS_NATIVE_ADAPTER_REQUIRED`.
