#ifndef SLEELA_SOCIOLOGY_HPP
#define SLEELA_SOCIOLOGY_HPP
#include "sociology_math.h"
#include <cstddef>
namespace sleela::sociology {
struct Observation { const char* id; const char* population; const char* timestamp_utc; const char* variable; double value; const char* unit; const char* source; const char* provenance; };
class Model {
public:
 static double proportion(double,double); static double rate(double,double,double);
 static double mean(const double*,std::size_t); static double variance(const double*,std::size_t);
 static double weightedMean(const double*,const double*,std::size_t);
 static double growthRate(double,double); static double riskRatio(double,double,double,double);
 static double oddsRatio(double,double,double,double); static double gini(const double*,std::size_t);
 static double transitionProbability(double,double); static double dissimilarity(double,double,double,double);
};
}
#endif
