#ifndef SLEELA_DELIMITED_CONNECTOR_H
#define SLEELA_DELIMITED_CONNECTOR_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sleela_delimited_row { char **fields; size_t count; } sleela_delimited_row;
int sleela_delimited_parse(const char *line,char delimiter,sleela_delimited_row *out,char *error,size_t error_size);
void sleela_delimited_row_free(sleela_delimited_row *row);
#ifdef __cplusplus
}
#endif
#endif
