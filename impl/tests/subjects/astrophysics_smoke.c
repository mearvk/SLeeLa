#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "astrophysics_math.h"
#include "observation_store.h"
int main(void){double f=sleela_astro_flux_from_luminosity(4.0*3.141592653589793,1.0);assert(fabs(f-1.0)<1e-12);assert(fabs(sleela_astro_redshift_from_wavelength(656.3,656.3))<1e-12);assert(fabs(sleela_astro_observed_wavelength(500e-9,1.0)-1e-6)<1e-18);assert(fabs(sleela_astro_blackbody_peak_wavelength(5772.0)-5.019e-7)<2e-9);assert(sleela_astro_kepler_period(1.495978707e11,1.98847e30)>3.1e7);SleelaObservationRecord r={"obs-1","astrophysics","Sun","2026-09-22T00:00:00Z","wavelength",500e-9,"m","demo","synthetic","test"};assert(sleela_observation_append_xml("build/astrophysics-observations.xml",&r));puts("astrophysics: PASS");return 0;}
