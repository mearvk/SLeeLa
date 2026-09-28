#include "astrophysics.hpp"
namespace sleela::astrophysics {
double Model::fluxFromLuminosity(double a,double b){return sleela_astro_flux_from_luminosity(a,b);}
double Model::redshift(double a,double b){return sleela_astro_redshift_from_wavelength(a,b);}
double Model::planck(double a,double b){return sleela_astro_planck_radiance(a,b);}
double Model::wienPeak(double a){return sleela_astro_blackbody_peak_wavelength(a);}
double Model::keplerPeriod(double a,double b){return sleela_astro_kepler_period(a,b);}
}
