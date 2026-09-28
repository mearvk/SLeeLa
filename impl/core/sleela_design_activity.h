#ifndef SLEELA_DESIGN_ACTIVITY_H
#define SLEELA_DESIGN_ACTIVITY_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLDA_DIMENSIONS 6
#define SLDA_VERSION 1
typedef enum { SLDA_CORRECTNESS=0, SLDA_REPRODUCIBILITY, SLDA_OBSERVABILITY, SLDA_SAFETY, SLDA_RESOURCE_DISCIPLINE, SLDA_INTEROPERABILITY } SLDA_Dimension;
typedef enum { SLDA_SCIENCE_GENERAL=0, SLDA_SCIENCE_MATH, SLDA_SCIENCE_PHYSICS, SLDA_SCIENCE_CHEMISTRY, SLDA_SCIENCE_ECONOMICS, SLDA_SCIENCE_INFERENCE, SLDA_SCIENCE_FINANCE, SLDA_SCIENCE_DATA_ANALYTICS } SLDA_ScienceDomain;
typedef struct { double value[SLDA_DIMENSIONS]; } SLDA_Vector;
typedef struct { int version; SLDA_ScienceDomain domain; SLDA_Vector normalized; SLDA_Vector expected; double mean_squared_variance; double rms_deviation; double l2_distance; } SLDA_Result;
int slda_normalize(const double raw[SLDA_DIMENSIONS], SLDA_Vector *out);
int slda_science_profile(SLDA_ScienceDomain domain, SLDA_Vector *out);
int slda_compare(const SLDA_Vector *normalized,const SLDA_Vector *expected,SLDA_Result *out);
const char *slda_domain_name(SLDA_ScienceDomain domain);
int slda_parse_domain(const char *name, SLDA_ScienceDomain *out);
int slda_format_json(const char *executable,const SLDA_Result *result,char *buf,size_t cap);
#ifdef __cplusplus
}
#endif
#endif
