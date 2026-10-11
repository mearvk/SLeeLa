/*
 * sleela_sql.h -- a simple CSV-backed MySQL-subset engine (C API).
 *
 * Part of mearvk/Nintendo. This is a self-contained, dependency-free SQL
 * engine: every table is a plain CSV file on disk (header row = column names,
 * one data row per record). It ships no copyrighted content.
 *
 * The division of labour mirrors the Professional Editor: this C engine does
 * the byte/file I/O (parsing statements, reading and writing CSV), while the
 * SLeeLa counterpart in /lib/sleela-sql owns the *model and validation* layer.
 *
 * TWO DIALECTS, ONE ENGINE
 * ------------------------
 * Every database statement can be written in either of two surface languages,
 * and both compile to the SAME internal operation and run on the SAME executor:
 *
 *   1. SQL          -- the familiar MySQL-flavoured subset.
 *   2. SLeeLaSQL    -- a clean, fluent, method-chaining dialect that mirrors
 *                      SLeeLa's own syntax (see docs/SLEELASQL.md). It is
 *                      feature-complete with the SQL subset: anything you can
 *                      say in SQL you can say in SLeeLaSQL and vice-versa.
 *
 * PREPARED STATEMENTS (the speed-up)
 * ----------------------------------
 * A statement in EITHER dialect can be *compiled once* into an ssql_stmt and
 * then executed many times with different bound values. Placeholders are '?'
 * (positional) in both dialects. Compiling once and re-binding avoids
 * re-parsing on every call -- the same win a real DB's PreparedStatement gives.
 */
#ifndef SLEELA_SQL_H
#define SLEELA_SQL_H

#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SSQL_MAX_COLS   64
#define SSQL_MAX_FIELD  512

typedef enum {
    SSQL_OK = 0,
    SSQL_ERR_IO       = -1,  /* file could not be read or written           */
    SSQL_ERR_SYNTAX   = -2,  /* the statement did not parse                 */
    SSQL_ERR_NOTABLE  = -3,  /* table (CSV file) does not exist             */
    SSQL_ERR_EXISTS   = -4,  /* CREATE on an existing table                 */
    SSQL_ERR_NOCOL    = -5,  /* referenced a column the table does not have */
    SSQL_ERR_ARITY    = -6,  /* INSERT value count != column count          */
    SSQL_ERR_ARG      = -7,  /* bad argument                                */
    SSQL_ERR_BIND     = -8,  /* unbound placeholder / bad bind index        */
    SSQL_ERR_OOM      = -9   /* allocation failed                           */
} ssql_status;

/* Which surface language a statement was written in. */
typedef enum {
    SSQL_DIALECT_AUTO = 0,   /* sniff: SLeeLaSQL if it looks fluent, else SQL */
    SSQL_DIALECT_SQL,        /* force classic SQL                             */
    SSQL_DIALECT_SLEELA      /* force SLeeLaSQL                                */
} ssql_dialect;

/* An engine instance: all tables live as <name>.csv under `dir`. */
typedef struct {
    char dir[SSQL_MAX_FIELD]; /* directory holding the .csv table files */
} ssql_db;

/* A compiled statement (prepared, dialect-agnostic). Opaque; heap-owned. */
typedef struct ssql_stmt ssql_stmt;

/* ---- database lifecycle ---------------------------------------------- */

/* Open (or create) a database rooted at a directory. */
ssql_status ssql_open(ssql_db *db, const char *dir);

/* ---- one-shot execution ---------------------------------------------- */

/*
 * Parse and execute a single statement in `text`. The dialect is auto-detected.
 * Any row output (SELECT / SHOW TABLES) is written to `out` as CSV; pass NULL
 * to discard. Equivalent to prepare + run + finalize with no bound values.
 */
ssql_status ssql_exec(ssql_db *db, const char *text, FILE *out);

/* As ssql_exec, but force a specific dialect (no sniffing). */
ssql_status ssql_exec_dialect(ssql_db *db, const char *text,
                              ssql_dialect dialect, FILE *out);

/* ---- prepared statements (both dialects) ----------------------------- */

/*
 * Compile `text` (in the given dialect; AUTO sniffs) into a reusable statement.
 * On success *out_stmt holds a heap object the caller must ssql_finalize().
 * The compile happens once; execute it repeatedly with ssql_run().
 */
ssql_status ssql_prepare(ssql_db *db, const char *text,
                         ssql_dialect dialect, ssql_stmt **out_stmt);

/* Number of '?' placeholders in a prepared statement. */
int ssql_param_count(const ssql_stmt *stmt);

/*
 * Bind a value to placeholder `index` (1-based, like JDBC/SQLite). The value is
 * copied. Re-binding the same index overwrites. Call ssql_reset to clear all.
 */
ssql_status ssql_bind(ssql_stmt *stmt, int index, const char *value);

/* Clear all bound values so the statement can be re-bound and re-run. */
void ssql_reset(ssql_stmt *stmt);

/*
 * Execute a prepared statement with the currently-bound values. Row output is
 * written to `out` as CSV (NULL to discard). May be called many times; call
 * ssql_reset / ssql_bind between calls to change the bound values.
 */
ssql_status ssql_run(ssql_stmt *stmt, FILE *out);

/* Release a prepared statement. Safe on NULL. */
void ssql_finalize(ssql_stmt *stmt);

/* ---- diagnostics ----------------------------------------------------- */

/* Human-readable message for a status code. */
const char *ssql_strerror(ssql_status s);

/* Name of a dialect ("sql" / "sleelasql" / "auto"). */
const char *ssql_dialect_name(ssql_dialect d);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_SQL_H */
