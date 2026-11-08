/*
 * advantage.h - Public interface for the advantage module.
 *
 * Provides a small C-callable API for computing a weighted "advantage"
 * score and a reduction over a series of samples. Safe to include from
 * both C and C++ translation units.
 */
#ifndef ADVANTAGE_H
#define ADVANTAGE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Compute a single advantage value: the reward discounted toward a
 * baseline. advantage = reward - baseline, scaled by weight.
 */
double advantage_value(double reward, double baseline, double weight);

/*
 * Compute the mean advantage over `count` samples. `rewards` and
 * `baselines` must each point to `count` doubles. Returns 0.0 when
 * count is 0 or either pointer is NULL.
 */
double advantage_mean(const double *rewards,
                      const double *baselines,
                      size_t count,
                      double weight);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ADVANTAGE_H */
