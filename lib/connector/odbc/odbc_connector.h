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
int sleela_odbc_prepare(sleela_odbc *c,const char *sql);
int sleela_odbc_execute_prepared(sleela_odbc *c);
SQLRETURN sleela_odbc_fetch(sleela_odbc *c); /* SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NO_DATA, or error */
SQLRETURN sleela_odbc_get_data(sleela_odbc *c,SQLUSMALLINT column,SQLSMALLINT target_type,SQLPOINTER buffer,SQLLEN buffer_bytes,SQLLEN *indicator);
SQLHSTMT sleela_odbc_statement(sleela_odbc *c); /* for SQLBindParameter and advanced ODBC operations */
SQLLEN sleela_odbc_row_count(sleela_odbc *c);
#ifdef __cplusplus
}
#endif
#endif
