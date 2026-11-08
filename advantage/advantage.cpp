// advantage.cpp - C++ demonstration driver for the advantage module.
//
// Links against advantage.c and prints both a single advantage value and
// the mean advantage over a short series of samples.
#include <iostream>
#include <vector>

#include "advantage.hpp"

int main() {
    const std::vector<double> rewards   = {1.0, 2.5, 0.5, 3.0};
    const std::vector<double> baselines = {0.5, 2.0, 1.0, 2.0};
    const double weight = 0.9;

    std::cout << "single advantage(reward=2.5, baseline=2.0, weight=0.9) = "
              << advantage::value(2.5, 2.0, weight) << "\n";

    std::cout << "mean advantage over " << rewards.size()
              << " samples = "
              << advantage::mean(rewards, baselines, weight) << "\n";

    return 0;
}
