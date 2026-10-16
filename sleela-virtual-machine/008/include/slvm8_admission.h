#ifndef SLVM8_ADMISSION_H
#define SLVM8_ADMISSION_H
#include "slvm8.h"
typedef struct { uint8_t artifact_valid,policy_valid,managers_valid,resources_valid,capabilities_valid,attestation_valid,lineage_valid,admitted; } slvm8_admission_t;
int slvm8_admission_validate(const slvm8_admission_t *a);
int slvm8_admission_evaluate(slvm8_admission_t *a);
#endif
