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

/*
 * Sample standard deviation of the per-sample advantage (weight applied).
 * Measures how consistent the advantage is. Returns 0.0 when count < 2 or
 * either pointer is NULL (spread is unknowable from fewer than two samples).
 */
double advantage_stddev(const double *rewards,
                        const double *baselines,
                        size_t count,
                        double weight);

/*
 * Confidence in [0, 1) that a mean over `count` samples deserves trust:
 * count / (count + prior). A few observations earn little confidence; many
 * earn nearly full confidence. `prior` is the number of observations at
 * which confidence is exactly one half. Returns 0.0 when count is 0, and 1.0
 * for any count > 0 if prior <= 0 (no skepticism requested).
 */
double advantage_confidence(size_t count, double prior);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ADVANTAGE_H */
