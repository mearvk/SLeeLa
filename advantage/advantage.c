/*
 * advantage.c - C implementation of the advantage module.
 */
#include "advantage.h"

#include <math.h>

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

double advantage_stddev(const double *rewards,
                        const double *baselines,
                        size_t count,
                        double weight)
{
    double mean, acc = 0.0;
    size_t i;

    if (rewards == NULL || baselines == NULL || count < 2) {
        return 0.0;
    }

    mean = advantage_mean(rewards, baselines, count, weight);
    for (i = 0; i < count; ++i) {
        double d = advantage_value(rewards[i], baselines[i], weight) - mean;
        acc += d * d;
    }
    return sqrt(acc / (double)(count - 1));
}

double advantage_confidence(size_t count, double prior)
{
    if (count == 0) {
        return 0.0;
    }
    if (prior <= 0.0) {
        return 1.0;
    }
    return (double)count / ((double)count + prior);
}
