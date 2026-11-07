# SLeeLa Neural Networks

SLeeLa-facing package for the native CPU neural-network implementation in
`artificial-intelligence/neural-nets/`. The native C ABI remains authoritative
for allocation, inference, and SGD updates; these SLeeLa classes describe and
validate the language-level configuration and lifecycle. They do not silently
pretend to execute native kernels.

## Public units

- `SLPackage` — package identity and native ABI contract.
- `SLNeuralNetworkConfig` — layer widths, activation choices, and deterministic seed.
- `SLNeuralNetwork` — opaque native-network lifecycle and operation contract.

## Native bridge

Compile/link `neural_nets.c` (C11) and, when using C++, `neural_nets.cpp`
(C++17). The C ABI is version 1 (`SLNN_ABI_VERSION`). A host adapter maps
SLeeLa configuration to `slnn_network_config`, then maps create, predict,
train-sample, and destroy to the corresponding `slnn_*` functions.

The network is fully connected and CPU-backed. It supports linear, ReLU, tanh,
and sigmoid activations, deterministic seeded initialization, one-sample
inference, and online single-sample MSE/SGD training. A single network instance
is not thread-safe. GPU execution, batching, persistence, and automatic
differentiation are not part of this ABI.

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

The example configures the SLeeLa-side contract. Actual inference/training
requires the native adapter described above.
