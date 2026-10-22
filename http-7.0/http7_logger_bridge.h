#ifndef SLEELA_HTTP7_LOGGER_BRIDGE_H
#define SLEELA_HTTP7_LOGGER_BRIDGE_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
int h7_logger_open(void);
void h7_logger_close(void);
/* Metadata only: this API intentionally accepts no payload pointer. */
int h7_logger_record(int sent, int admission, size_t bytes, const char *reason);
#ifdef __cplusplus
}
#endif
#endif
