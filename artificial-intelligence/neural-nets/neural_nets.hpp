#ifndef SLEELA_AI_NEURAL_NETS_HPP
#define SLEELA_AI_NEURAL_NETS_HPP

#include "neural_nets.h"

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace sleela::ai {

class NeuralNetwork {
public:
    NeuralNetwork(const std::vector<std::size_t>& layer_sizes,
                  slnn_activation hidden = SLNN_ACTIVATION_RELU,
                  slnn_activation output = SLNN_ACTIVATION_LINEAR,
                  std::uint64_t seed = 0);
    ~NeuralNetwork();

    NeuralNetwork(const NeuralNetwork&) = delete;
    NeuralNetwork& operator=(const NeuralNetwork&) = delete;
    NeuralNetwork(NeuralNetwork&& other) noexcept;
    NeuralNetwork& operator=(NeuralNetwork&& other) noexcept;

    std::vector<double> predict(const std::vector<double>& input) const;
    void train_sample(const std::vector<double>& input,
                      const std::vector<double>& target,
                      double learning_rate);

    std::size_t input_count() const noexcept;
    std::size_t output_count() const noexcept;

private:
    slnn_network* handle_ = nullptr;
    void require_handle() const;
    static void throw_if_error(slnn_status status);
};

} // namespace sleela::ai

#endif
