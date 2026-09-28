#ifndef SLEELA_SOCIOLOGY_MATH_H
#define SLEELA_SOCIOLOGY_MATH_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
double sleela_soc_proportion(double part,double total);
double sleela_soc_rate(double events,double population,double scale);
double sleela_soc_mean(const double *x,size_t n);
double sleela_soc_variance(const double *x,size_t n);
double sleela_soc_weighted_mean(const double *x,const double *w,size_t n);
double sleela_soc_growth_rate(double old_value,double new_value);
double sleela_soc_risk_ratio(double exposed_cases,double exposed_total,double control_cases,double control_total);
double sleela_soc_odds_ratio(double a,double b,double c,double d);
double sleela_soc_gini(const double *x,size_t n);
double sleela_soc_transition_probability(double transitions,double origin_total);
double sleela_soc_dissimilarity(double a_group,double b_group,double a_total,double b_total);
#ifdef __cplusplus
}
#endif
#endif
