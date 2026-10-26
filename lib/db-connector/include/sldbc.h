/*
 * lib/db-connector/include/sldbc.h
 *
 * SLeeLa DB Connector -- the SLeeLa equivalent of JDBC + a driver.
 *
 * Java has a database API (java.sql.*) plus pluggable DRIVERS shipped as jars
 * (e.g. the PostgreSQL connector jar). SLeeLa has the same split:
 *
 *   - lib/database (.sleela)       : the API SHAPE (SLDatabase, SLResultSet...)
 *   - this connector (sldbc_*)     : the driver manager + driver contract
 *   - lib/sleela-sql (ssql_*)      : a concrete backend the first driver targets
 *
 * This header is the driver-agnostic connector ABI. A program opens a
 * connection by URL (e.g. "sleelasql:./data"), prepares statements, binds
 * positional parameters, executes, and iterates result rows -- exactly the
 * JDBC lifecycle, in C. The bundled "sleelasql" driver wraps the local
 * lib/sleela-sql engine; other backends (PostgreSQL, MySQL, ...) are registered
 * by name but return SLDBC_UNSUPPORTED until a real driver is written, so the
 * connector never pretends to support a database it cannot actually reach.
 */
#ifndef SLEELA_SLDBC_H
#define SLEELA_SLDBC_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  SLDBC_OK = 0,
  SLDBC_ERR_ARG = -1,        /* bad argument                         */
  SLDBC_ERR_URL = -2,        /* malformed connection URL             */
  SLDBC_ERR_NODRIVER = -3,   /* no driver registered for that scheme */
  SLDBC_UNSUPPORTED = -4,    /* driver is a stub / op not supported  */
  SLDBC_ERR_OPEN = -5,       /* backend failed to open               */
  SLDBC_ERR_EXEC = -6,       /* statement failed                     */
  SLDBC_ERR_RANGE = -7,      /* row/column index out of range        */
  SLDBC_ERR_OOM = -8         /* allocation failed                    */
} sldbc_status;

/* Opaque handles (JDBC: Connection, PreparedStatement, ResultSet). */
typedef struct sldbc_conn sldbc_conn;
typedef struct sldbc_stmt sldbc_stmt;
typedef struct sldbc_rows sldbc_rows;

/* ---- driver manager -------------------------------------------------- */

/*
 * Open a connection. `url` is "<scheme>:<target>" -- e.g. "sleelasql:./data".
 * The scheme selects the driver; the target is driver-specific (for sleelasql
 * it is the database directory). On success *out holds a connection the caller
 * must sldbc_close().
 */
sldbc_status sldbc_open(const char *url, sldbc_conn **out);

/* Close a connection (finalizes nothing the caller still holds). Safe on NULL. */
void sldbc_close(sldbc_conn *c);

/* The driver (scheme) name backing a connection, e.g. "sleelasql". */
const char *sldbc_driver_name(const sldbc_conn *c);

/* Is a scheme backed by a real (non-stub) driver? 1 yes, 0 stub/unknown. */
int sldbc_driver_available(const char *scheme);

/* ---- statements ------------------------------------------------------ */

/*
 * Prepare `sql` (SQL or fluent SLeeLaSQL; '?' positional placeholders). On
 * success *out holds a statement the caller must sldbc_finalize().
 */
sldbc_status sldbc_prepare(sldbc_conn *c, const char *sql, sldbc_stmt **out);

/* Bind a value to placeholder `index` (1-based, like JDBC). Value is copied. */
sldbc_status sldbc_bind(sldbc_stmt *s, int index, const char *value);

/* Clear all bound values so the statement can be re-bound and re-run. */
void sldbc_clear(sldbc_stmt *s);

/*
 * Execute a statement that returns no rows (INSERT/UPDATE/DELETE/CREATE/...).
 * Returns SLDBC_OK or an error.
 */
sldbc_status sldbc_execute(sldbc_stmt *s);

/*
 * Execute a query and capture its result set. On success *out holds a row set
 * the caller must sldbc_rows_free(). Works for SELECT / SHOW TABLES.
 */
sldbc_status sldbc_query(sldbc_stmt *s, sldbc_rows **out);

/* Release a prepared statement. Safe on NULL. */
void sldbc_finalize(sldbc_stmt *s);

/* ---- result sets (JDBC: ResultSet) ----------------------------------- */

size_t sldbc_rows_count(const sldbc_rows *r);     /* number of data rows   */
size_t sldbc_rows_columns(const sldbc_rows *r);   /* number of columns     */
const char *sldbc_rows_column_name(const sldbc_rows *r, size_t col);

/*
 * Cell value at (row, col), or NULL if out of range. The returned pointer is
 * owned by the row set and valid until sldbc_rows_free().
 */
const char *sldbc_rows_value(const sldbc_rows *r, size_t row, size_t col);

/* Value by column name in a given row; NULL if the column is absent. */
const char *sldbc_rows_value_named(const sldbc_rows *r, size_t row,
                                   const char *column);

void sldbc_rows_free(sldbc_rows *r);

/* ---- transactions (JDBC: setAutoCommit(false)/commit/rollback) ------- */

sldbc_status sldbc_begin(sldbc_conn *c);
sldbc_status sldbc_commit(sldbc_conn *c);
sldbc_status sldbc_rollback(sldbc_conn *c);

/* ---- diagnostics ----------------------------------------------------- */

const char *sldbc_strerror(sldbc_status s);
const char *sldbc_version(void);

#ifdef __cplusplus
}
#endif
#endif /* SLEELA_SLDBC_H */
