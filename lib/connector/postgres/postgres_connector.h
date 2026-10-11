#ifndef SLEELA_POSTGRES_CONNECTOR_H
#define SLEELA_POSTGRES_CONNECTOR_H
#include <stddef.h>
#include <libpq-fe.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sleela_postgres { PGconn *conn; char error[512]; } sleela_postgres;
int sleela_postgres_connect(sleela_postgres *c,const char *conninfo);
void sleela_postgres_close(sleela_postgres *c);
const char *sleela_postgres_error(const sleela_postgres *c);
PGresult *sleela_postgres_query_params(sleela_postgres *c,const char *sql,int nparams,const char *const *values);
int sleela_postgres_result_ok(const PGresult *r);
void sleela_postgres_result_clear(PGresult *r);
#ifdef __cplusplus
}
#endif
#endif
