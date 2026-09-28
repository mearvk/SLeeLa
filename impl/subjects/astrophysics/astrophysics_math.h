#ifndef SLEELA_ASTROPHYSICS_MATH_H
#define SLEELA_ASTROPHYSICS_MATH_H
#ifdef __cplusplus
extern "C" {
#endif
double sleela_astro_flux_from_luminosity(double luminosity_w,double distance_m);
double sleela_astro_luminosity_from_flux(double flux_w_m2,double distance_m);
double sleela_astro_redshift_from_wavelength(double observed_m,double rest_m);
double sleela_astro_observed_wavelength(double rest_m,double redshift);
double sleela_astro_planck_radiance(double wavelength_m,double temperature_k);
double sleela_astro_blackbody_peak_wavelength(double temperature_k);
double sleela_astro_kepler_period(double semi_major_axis_m,double central_mass_kg);
double sleela_astro_escape_velocity(double mass_kg,double radius_m);
double sleela_astro_gravitational_parameter(double mass_kg);
double sleela_astro_angular_size(double physical_size_m,double distance_m);
double sleela_astro_distance_modulus(double distance_pc);
#ifdef __cplusplus
}
#endif
#endif
