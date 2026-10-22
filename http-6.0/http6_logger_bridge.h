#ifndef SLEELA_HTTP6_LOGGER_BRIDGE_H
#define SLEELA_HTTP6_LOGGER_BRIDGE_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
int h6_logger_open(void);
void h6_logger_close(void);
/* Metadata-only packet records. Payload logging is intentionally unavailable. */
int h6_logger_record(int sent, int admission, size_t bytes, const char *reason);
#ifdef __cplusplus
}
#endif
#endif
