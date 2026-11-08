/*
 * advantage.c - C implementation of the advantage module.
 */
#include "advantage.h"

double advantage_value(double reward, double baseline, double weight)
{
    return (reward - baseline) * weight;
}

double advantage_mean(const double *rewards,
                      const double *baselines,
                      size_t count,
                      double weight)
{
    double sum = 0.0;
    size_t i;

    if (rewards == NULL || baselines == NULL || count == 0) {
        return 0.0;
    }

    for (i = 0; i < count; ++i) {
        sum += advantage_value(rewards[i], baselines[i], weight);
    }

    return sum / (double)count;
}
