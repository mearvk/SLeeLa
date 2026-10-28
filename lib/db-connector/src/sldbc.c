/*
 * lib/db-connector/src/sldbc.c
 *
 * SLeeLa DB Connector implementation: a tiny driver manager plus the bundled
 * "sleelasql" driver wrapping lib/sleela-sql (ssql_*). External-RDBMS schemes
 * are registered by name but return SLDBC_UNSUPPORTED (honest stubs).
 */
/* open_memstream is POSIX.1-2008; request it before any libc header. */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include "sldbc.h"
#include "../../sleela-sql/native/include/sleela_sql.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *sldbc_version(void) { return "1.0.0"; }

/* ---- schemes --------------------------------------------------------- */
/* The one real driver, plus the external backends the connector knows the
 * NAME of but cannot yet reach (mirrors api/database's kinds). */
static const char *k_real_driver = "sleelasql";
static const char *k_stub_drivers[] = {
    "postgresql", "mysql", "mariadb", "sqlite", "sqlserver", "oracle", "odbc"};

int sldbc_driver_available(const char *scheme) {
  if (!scheme) return 0;
  return strcmp(scheme, k_real_driver) == 0 ? 1 : 0;
}

static int scheme_is_known(const char *scheme) {
  if (sldbc_driver_available(scheme)) return 1;
  for (size_t i = 0; i < sizeof k_stub_drivers / sizeof k_stub_drivers[0]; ++i)
    if (strcmp(scheme, k_stub_drivers[i]) == 0) return 1;
  return 0;
}

/* ---- handles --------------------------------------------------------- */
struct sldbc_conn {
  char scheme[32];
  ssql_db db; /* valid when scheme == sleelasql */
};

struct sldbc_stmt {
  sldbc_conn *conn;
  ssql_stmt *s; /* underlying ssql prepared statement */
};

struct sldbc_rows {
  char **cols;
  size_t ncols;
  char ***cells; /* cells[row][col] */
  size_t nrows;
};

/* ---- driver manager -------------------------------------------------- */
sldbc_status sldbc_open(const char *url, sldbc_conn **out) {
  if (!url || !out) return SLDBC_ERR_ARG;
  *out = NULL;
  const char *colon = strchr(url, ':');
  if (!colon || colon == url) return SLDBC_ERR_URL;
  size_t slen = (size_t)(colon - url);
  if (slen >= 32) return SLDBC_ERR_URL;
  char scheme[32];
  memcpy(scheme, url, slen);
  scheme[slen] = 0;
  const char *target = colon + 1;

  if (!scheme_is_known(scheme)) return SLDBC_ERR_NODRIVER;
  if (!sldbc_driver_available(scheme)) return SLDBC_UNSUPPORTED; /* stub backend */

  sldbc_conn *c = (sldbc_conn *)calloc(1, sizeof *c);
  if (!c) return SLDBC_ERR_OOM;
  snprintf(c->scheme, sizeof c->scheme, "%s", scheme);
  if (!*target) target = ".";
  if (ssql_open(&c->db, target) != SSQL_OK) {
    free(c);
    return SLDBC_ERR_OPEN;
  }
  *out = c;
  return SLDBC_OK;
}

void sldbc_close(sldbc_conn *c) { free(c); }

const char *sldbc_driver_name(const sldbc_conn *c) {
  return c ? c->scheme : "";
}

/* ---- statements ------------------------------------------------------ */
sldbc_status sldbc_prepare(sldbc_conn *c, const char *sql, sldbc_stmt **out) {
  if (!c || !sql || !out) return SLDBC_ERR_ARG;
  *out = NULL;
  sldbc_stmt *st = (sldbc_stmt *)calloc(1, sizeof *st);
  if (!st) return SLDBC_ERR_OOM;
  st->conn = c;
  if (ssql_prepare(&c->db, sql, SSQL_DIALECT_AUTO, &st->s) != SSQL_OK) {
    free(st);
    return SLDBC_ERR_EXEC;
  }
  *out = st;
  return SLDBC_OK;
}

sldbc_status sldbc_bind(sldbc_stmt *s, int index, const char *value) {
  if (!s || !s->s) return SLDBC_ERR_ARG;
  return ssql_bind(s->s, index, value) == SSQL_OK ? SLDBC_OK : SLDBC_ERR_ARG;
}

void sldbc_clear(sldbc_stmt *s) {
  if (s && s->s) ssql_reset(s->s);
}

sldbc_status sldbc_execute(sldbc_stmt *s) {
  if (!s || !s->s) return SLDBC_ERR_ARG;
  return ssql_run(s->s, NULL) == SSQL_OK ? SLDBC_OK : SLDBC_ERR_EXEC;
}

void sldbc_finalize(sldbc_stmt *s) {
  if (!s) return;
  if (s->s) ssql_finalize(s->s);
  free(s);
}

/* ---- CSV -> row set -------------------------------------------------- */
static void split_csv_line(const char *line, char ***out_fields, size_t *out_n) {
  size_t cap = 4, n = 0;
  char **f = (char **)malloc(cap * sizeof *f);
  if (!f) { *out_fields = NULL; *out_n = 0; return; }
  const char *p = line;
  const char *start = p;
  for (;; ++p) {
    if (*p == ',' || *p == 0) {
      size_t len = (size_t)(p - start);
      char *field = (char *)malloc(len + 1);
      if (field) { memcpy(field, start, len); field[len] = 0; }
      if (n >= cap) {
        cap *= 2;
        char **nf = (char **)realloc(f, cap * sizeof *f);
        if (!nf) { break; }
        f = nf;
      }
      f[n++] = field;
      start = p + 1;
      if (*p == 0) break;
    }
  }
  *out_fields = f;
  *out_n = n;
}

static sldbc_status parse_csv(const char *csv, sldbc_rows **out) {
  sldbc_rows *r = (sldbc_rows *)calloc(1, sizeof *r);
  if (!r) return SLDBC_ERR_OOM;

  /* Split into lines. */
  size_t cap_lines = 8, nlines = 0;
  char **lines = (char **)malloc(cap_lines * sizeof *lines);
  if (!lines) { free(r); return SLDBC_ERR_OOM; }
  const char *p = csv;
  while (*p) {
    const char *nl = strchr(p, '\n');
    size_t len = nl ? (size_t)(nl - p) : strlen(p);
    if (len && p[len - 1] == '\r') len--; /* tolerate CRLF */
    char *line = (char *)malloc(len + 1);
    if (line) { memcpy(line, p, len); line[len] = 0; }
    if (nlines >= cap_lines) {
      cap_lines *= 2;
      char **nls = (char **)realloc(lines, cap_lines * sizeof *lines);
      if (!nls) break;
      lines = nls;
    }
    lines[nlines++] = line;
    if (!nl) break;
    p = nl + 1;
  }

  if (nlines == 0) {
    /* empty result: zero columns, zero rows */
    free(lines);
    *out = r;
    return SLDBC_OK;
  }

  /* First line = header. */
  split_csv_line(lines[0], &r->cols, &r->ncols);
  if (nlines > 1) {
    r->cells = (char ***)calloc(nlines - 1, sizeof *r->cells);
    if (r->cells) {
      for (size_t i = 1; i < nlines; ++i) {
        char **fields = NULL;
        size_t nf = 0;
        split_csv_line(lines[i], &fields, &nf);
        /* normalize each row to ncols cells */
        char **row = (char **)calloc(r->ncols ? r->ncols : 1, sizeof *row);
        if (row) {
          for (size_t c = 0; c < r->ncols; ++c) {
            const char *v = c < nf ? fields[c] : "";
            char *dup = (char *)malloc(strlen(v) + 1);
            if (dup) strcpy(dup, v);
            row[c] = dup;
          }
        }
        for (size_t c = 0; c < nf; ++c) free(fields[c]);
        free(fields);
        r->cells[r->nrows++] = row;
      }
    }
  }

  for (size_t i = 0; i < nlines; ++i) free(lines[i]);
  free(lines);
  *out = r;
  return SLDBC_OK;
}

sldbc_status sldbc_query(sldbc_stmt *s, sldbc_rows **out) {
  if (!s || !s->s || !out) return SLDBC_ERR_ARG;
  *out = NULL;
  char *buf = NULL;
  size_t len = 0;
  FILE *mem = open_memstream(&buf, &len);
  ssql_status st = ssql_run(s->s, mem);
  if (mem) fclose(mem);
  if (st != SSQL_OK) {
    free(buf);
    return SLDBC_ERR_EXEC;
  }
  sldbc_status rc = parse_csv(buf ? buf : "", out);
  free(buf);
  return rc;
}

/* ---- row-set accessors ----------------------------------------------- */
size_t sldbc_rows_count(const sldbc_rows *r) { return r ? r->nrows : 0; }
size_t sldbc_rows_columns(const sldbc_rows *r) { return r ? r->ncols : 0; }

const char *sldbc_rows_column_name(const sldbc_rows *r, size_t col) {
  if (!r || col >= r->ncols) return NULL;
  return r->cols[col];
}

const char *sldbc_rows_value(const sldbc_rows *r, size_t row, size_t col) {
  if (!r || row >= r->nrows || col >= r->ncols) return NULL;
  if (!r->cells || !r->cells[row]) return NULL;
  return r->cells[row][col];
}

const char *sldbc_rows_value_named(const sldbc_rows *r, size_t row,
                                   const char *column) {
  if (!r || !column) return NULL;
  for (size_t c = 0; c < r->ncols; ++c)
    if (r->cols[c] && strcmp(r->cols[c], column) == 0)
      return sldbc_rows_value(r, row, c);
  return NULL;
}

void sldbc_rows_free(sldbc_rows *r) {
  if (!r) return;
  for (size_t c = 0; c < r->ncols; ++c) free(r->cols[c]);
  free(r->cols);
  for (size_t i = 0; i < r->nrows; ++i) {
    if (r->cells && r->cells[i]) {
      for (size_t c = 0; c < r->ncols; ++c) free(r->cells[i][c]);
      free(r->cells[i]);
    }
  }
  free(r->cells);
  free(r);
}

/* ---- transactions ---------------------------------------------------- */
sldbc_status sldbc_begin(sldbc_conn *c) {
  if (!c) return SLDBC_ERR_ARG;
  return ssql_begin(&c->db) == SSQL_OK ? SLDBC_OK : SLDBC_ERR_EXEC;
}
sldbc_status sldbc_commit(sldbc_conn *c) {
  if (!c) return SLDBC_ERR_ARG;
  return ssql_commit(&c->db) == SSQL_OK ? SLDBC_OK : SLDBC_ERR_EXEC;
}
sldbc_status sldbc_rollback(sldbc_conn *c) {
  if (!c) return SLDBC_ERR_ARG;
  return ssql_rollback(&c->db) == SSQL_OK ? SLDBC_OK : SLDBC_ERR_EXEC;
}

/* ---- diagnostics ----------------------------------------------------- */
const char *sldbc_strerror(sldbc_status s) {
  switch (s) {
    case SLDBC_OK: return "ok";
    case SLDBC_ERR_ARG: return "bad argument";
    case SLDBC_ERR_URL: return "malformed connection url";
    case SLDBC_ERR_NODRIVER: return "no driver for scheme";
    case SLDBC_UNSUPPORTED: return "driver not yet implemented";
    case SLDBC_ERR_OPEN: return "backend open failed";
    case SLDBC_ERR_EXEC: return "statement failed";
    case SLDBC_ERR_RANGE: return "index out of range";
    case SLDBC_ERR_OOM: return "out of memory";
    default: return "unknown error";
  }
}
