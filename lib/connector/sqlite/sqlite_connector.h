#ifndef SLEELA_SQLITE_CONNECTOR_H
#define SLEELA_SQLITE_CONNECTOR_H
#include <stddef.h>
#include <stdint.h>
#include <sqlite3.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sleela_sqlite { sqlite3 *db; char error[512]; } sleela_sqlite;
int sleela_sqlite_open(sleela_sqlite *c, const char *path);
void sleela_sqlite_close(sleela_sqlite *c);
const char *sleela_sqlite_error(const sleela_sqlite *c);
int sleela_sqlite_prepare(sleela_sqlite *c, const char *sql, sqlite3_stmt **out);
int sleela_sqlite_bind_text(sqlite3_stmt *s, int index, const char *value);
int sleela_sqlite_bind_int64(sqlite3_stmt *s, int index, int64_t value);
int sleela_sqlite_bind_double(sqlite3_stmt *s, int index, double value);
int sleela_sqlite_bind_null(sqlite3_stmt *s, int index);
int sleela_sqlite_step(sqlite3_stmt *s); /* SQLITE_ROW, SQLITE_DONE, or error */
void sleela_sqlite_finalize(sqlite3_stmt *s);
int64_t sleela_sqlite_changes(const sleela_sqlite *c);
#ifdef __cplusplus
}
#endif
#endif
