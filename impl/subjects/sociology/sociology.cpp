#include "sociology.hpp"
namespace sleela::sociology {
double Model::proportion(double a,double b){return sleela_soc_proportion(a,b);}
double Model::rate(double a,double b,double c){return sleela_soc_rate(a,b,c);}
double Model::mean(const double*x,std::size_t n){return sleela_soc_mean(x,n);}
double Model::variance(const double*x,std::size_t n){return sleela_soc_variance(x,n);}
double Model::weightedMean(const double*x,const double*w,std::size_t n){return sleela_soc_weighted_mean(x,w,n);}
double Model::growthRate(double a,double b){return sleela_soc_growth_rate(a,b);}
double Model::riskRatio(double a,double b,double c,double d){return sleela_soc_risk_ratio(a,b,c,d);}
double Model::oddsRatio(double a,double b,double c,double d){return sleela_soc_odds_ratio(a,b,c,d);}
double Model::gini(const double*x,std::size_t n){return sleela_soc_gini(x,n);}
double Model::transitionProbability(double a,double b){return sleela_soc_transition_probability(a,b);}
double Model::dissimilarity(double a,double b,double c,double d){return sleela_soc_dissimilarity(a,b,c,d);}
}
