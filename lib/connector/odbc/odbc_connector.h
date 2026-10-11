#ifndef SLEELA_ODBC_CONNECTOR_H
#define SLEELA_ODBC_CONNECTOR_H
#include <sql.h>
#include <sqlext.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sleela_odbc { SQLHENV env; SQLHDBC dbc; SQLHSTMT stmt; char error[512]; } sleela_odbc;
int sleela_odbc_connect(sleela_odbc *c,const char *connection_string);
void sleela_odbc_close(sleela_odbc *c);
const char *sleela_odbc_error(const sleela_odbc *c);
int sleela_odbc_exec(sleela_odbc *c,const char *sql);
SQLLEN sleela_odbc_row_count(sleela_odbc *c);
#ifdef __cplusplus
}
#endif
#endif
