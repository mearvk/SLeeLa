// advantage.hpp - C++ wrapper over the C advantage module.
//
// Offers a small, STL-friendly API on top of the C core so C++ callers
// can work with std::vector instead of raw pointers.
#ifndef ADVANTAGE_HPP
#define ADVANTAGE_HPP

#include <vector>

#include "advantage.h"

namespace advantage {

// Single weighted advantage value (thin wrapper over advantage_value).
inline double value(double reward, double baseline, double weight = 1.0) {
    return advantage_value(reward, baseline, weight);
}

// Mean advantage over two equally-sized series. If the vectors differ in
// length, the shorter length is used; an empty input yields 0.0.
inline double mean(const std::vector<double>& rewards,
                   const std::vector<double>& baselines,
                   double weight = 1.0) {
    const std::size_t count =
        rewards.size() < baselines.size() ? rewards.size() : baselines.size();
    return advantage_mean(rewards.data(), baselines.data(), count, weight);
}

}  // namespace advantage

#endif  // ADVANTAGE_HPP
