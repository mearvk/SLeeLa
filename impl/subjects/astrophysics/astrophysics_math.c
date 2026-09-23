#include "astrophysics_math.h"
#include <math.h>
#define G 6.67430e-11
#define C 299792458.0
#define H 6.62607015e-34
#define KB 1.380649e-23
#define PI 3.14159265358979323846
#define B 2.897771955e-3
#define PC 3.0856775814913673e16
double sleela_astro_flux_from_luminosity(double L,double r){return r>0.0?L/(4.0*PI*r*r):0.0;}
double sleela_astro_luminosity_from_flux(double F,double r){return r>=0.0?4.0*PI*r*r*F:0.0;}
double sleela_astro_redshift_from_wavelength(double o,double r){return r!=0.0?o/r-1.0:0.0;}
double sleela_astro_observed_wavelength(double r,double z){return r*(1.0+z);}
double sleela_astro_planck_radiance(double w,double T){if(w<=0.0||T<=0.0)return 0.0;double x=H*C/(w*KB*T);if(x>700.0)return 0.0;return 2.0*H*C*C/(pow(w,5.0)*expm1(x));}
double sleela_astro_blackbody_peak_wavelength(double T){return T>0.0?B/T:0.0;}
double sleela_astro_kepler_period(double a,double M){return a>0.0&&M>0.0?2.0*PI*sqrt(a*a*a/(G*M)):0.0;}
double sleela_astro_escape_velocity(double M,double r){return M>0.0&&r>0.0?sqrt(2.0*G*M/r):0.0;}
double sleela_astro_gravitational_parameter(double M){return G*M;}
double sleela_astro_angular_size(double s,double r){return r>0.0?s/r:0.0;}
double sleela_astro_distance_modulus(double pc){return pc>0.0?5.0*log10(pc)-5.0:0.0;}
