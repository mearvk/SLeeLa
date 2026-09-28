#ifndef SLEELA_OBSERVATION_STORE_H
#define SLEELA_OBSERVATION_STORE_H
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {const char* id;const char* subject;const char* target;const char* timestamp_utc;const char* quantity;double value;const char* unit;const char* instrument;const char* source;const char* provenance;} SleelaObservationRecord;
int sleela_observation_append_xml(const char*,const SleelaObservationRecord*);
#ifdef __cplusplus
}
#endif
#endif
