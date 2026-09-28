#ifndef SLEELA_ASTROPHYSICS_HPP
#define SLEELA_ASTROPHYSICS_HPP
#include "astrophysics_math.h"
namespace sleela::astrophysics {
struct Observation { const char* id;const char* target;const char* timestamp_utc;double value;const char* unit;const char* instrument;const char* source; };
class Model {
public:
 static double fluxFromLuminosity(double,double); static double redshift(double,double);
 static double planck(double,double); static double wienPeak(double); static double keplerPeriod(double,double);
};
}
#endif
