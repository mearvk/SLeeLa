#include "neural_nets.hpp"

#include <utility>

namespace sleela::ai {

void NeuralNetwork::throw_if_error(slnn_status status) {
    if (status != SLNN_OK)
        throw std::runtime_error(slnn_status_string(status));
}

void NeuralNetwork::require_handle() const {
    if (!handle_) throw std::logic_error("NeuralNetwork is empty or moved-from");
}

NeuralNetwork::NeuralNetwork(const std::vector<std::size_t>& sizes,
                             slnn_activation hidden,
                             slnn_activation output,
                             std::uint64_t seed) {
    if (sizes.size() < 2) throw std::invalid_argument("a network needs at least input and output layers");
    slnn_network_config config{};
    config.abi_version = SLNN_ABI_VERSION;
    config.layer_count = sizes.size() - 1;
    config.layer_sizes = sizes.data();
    config.hidden_activation = hidden;
    config.output_activation = output;
    config.seed = seed;
    throw_if_error(slnn_network_create(&config, &handle_));
}

NeuralNetwork::~NeuralNetwork() { slnn_network_destroy(handle_); }

NeuralNetwork::NeuralNetwork(NeuralNetwork&& other) noexcept
    : handle_(std::exchange(other.handle_, nullptr)) {}

NeuralNetwork& NeuralNetwork::operator=(NeuralNetwork&& other) noexcept {
    if (this != &other) {
        slnn_network_destroy(handle_);
        handle_ = std::exchange(other.handle_, nullptr);
    }
    return *this;
}

std::vector<double> NeuralNetwork::predict(const std::vector<double>& input) const {
    require_handle();
    if (input.size() != input_count())
        throw std::invalid_argument("input vector size does not match network");
    std::vector<double> output(output_count());
    throw_if_error(slnn_network_predict(handle_, input.data(), input.size(),
                                        output.data(), output.size()));
    return output;
}

void NeuralNetwork::train_sample(const std::vector<double>& input,
                                 const std::vector<double>& target,
                                 double learning_rate) {
    require_handle();
    throw_if_error(slnn_network_train_sample(handle_, input.data(), input.size(),
                                             target.data(), target.size(), learning_rate));
}

std::size_t NeuralNetwork::input_count() const noexcept {
    return slnn_network_input_count(handle_);
}
std::size_t NeuralNetwork::output_count() const noexcept {
    return slnn_network_output_count(handle_);
}

} // namespace sleela::ai
