/*
 * lib/db-connector/tests/test_connector.c
 *
 * Exercises the connector's JDBC-style lifecycle through the bundled
 * "sleelasql" driver, plus the honest-stub behaviour for external backends.
 * Usage: test-connector <db-dir>. Exit 0 = all pass.
 */
#include "sldbc.h"

#include <stdio.h>
#include <string.h>

static int fails = 0;
#define CHECK(c, m) do { if (!(c)) { printf("FAIL: %s\n", m); ++fails; } \
                         else printf("ok:   %s\n", m); } while (0)

int main(int argc, char **argv) {
  if (argc != 2) return 2;
  char url[512];
  snprintf(url, sizeof url, "sleelasql:%s", argv[1]);
  printf("sldbc version %s\n", sldbc_version());

  /* driver manager */
  CHECK(sldbc_driver_available("sleelasql"), "sleelasql driver available");
  CHECK(!sldbc_driver_available("postgresql"), "postgresql is a stub (not available)");

  sldbc_conn *c = NULL;
  CHECK(sldbc_open(url, &c) == SLDBC_OK && c, "open sleelasql connection");
  CHECK(strcmp(sldbc_driver_name(c), "sleelasql") == 0, "driver name is sleelasql");

  /* honest stub: a known external scheme is refused, not faked */
  sldbc_conn *pg = NULL;
  CHECK(sldbc_open("postgresql:localhost/db", &pg) == SLDBC_UNSUPPORTED,
        "external backend refused (UNSUPPORTED)");
  CHECK(pg == NULL, "no connection handed back for a stub");
  CHECK(sldbc_open("bogus:x", &pg) == SLDBC_ERR_NODRIVER, "unknown scheme -> NODRIVER");
  CHECK(sldbc_open("noscheme", &pg) == SLDBC_ERR_URL, "missing scheme -> URL error");

  /* DDL + DML through prepared statements */
  sldbc_stmt *s = NULL;
  CHECK(sldbc_prepare(c, "CREATE TABLE person (id, name)", &s) == SLDBC_OK, "prepare CREATE");
  CHECK(sldbc_execute(s) == SLDBC_OK, "execute CREATE");
  sldbc_finalize(s);

  /* parameterised insert (JDBC-style '?' binds) */
  CHECK(sldbc_prepare(c, "INSERT INTO person VALUES (?, ?)", &s) == SLDBC_OK, "prepare INSERT ?");
  CHECK(sldbc_bind(s, 1, "1") == SLDBC_OK, "bind id=1");
  CHECK(sldbc_bind(s, 2, "Ada") == SLDBC_OK, "bind name=Ada");
  CHECK(sldbc_execute(s) == SLDBC_OK, "execute INSERT (1,Ada)");
  sldbc_clear(s);
  CHECK(sldbc_bind(s, 1, "2") == SLDBC_OK, "bind id=2");
  CHECK(sldbc_bind(s, 2, "Linus") == SLDBC_OK, "bind name=Linus");
  CHECK(sldbc_execute(s) == SLDBC_OK, "execute INSERT (2,Linus)");
  sldbc_finalize(s);

  /* query + result set */
  CHECK(sldbc_prepare(c, "SELECT id, name FROM person", &s) == SLDBC_OK, "prepare SELECT");
  sldbc_rows *rows = NULL;
  CHECK(sldbc_query(s, &rows) == SLDBC_OK && rows, "query returns rows");
  CHECK(sldbc_rows_columns(rows) == 2, "two columns");
  CHECK(sldbc_rows_count(rows) == 2, "two rows");
  CHECK(strcmp(sldbc_rows_column_name(rows, 1), "name") == 0, "column 1 is name");
  CHECK(strcmp(sldbc_rows_value(rows, 0, 1), "Ada") == 0, "row0.name = Ada");
  CHECK(strcmp(sldbc_rows_value_named(rows, 1, "name"), "Linus") == 0, "row1.name by name = Linus");
  CHECK(sldbc_rows_value(rows, 9, 0) == NULL, "out-of-range row -> NULL");
  sldbc_rows_free(rows);
  sldbc_finalize(s);

  /* transaction: rollback undoes a write */
  CHECK(sldbc_begin(c) == SLDBC_OK, "begin txn");
  CHECK(sldbc_prepare(c, "INSERT INTO person VALUES (3, 'Temp')", &s) == SLDBC_OK, "prepare txn insert");
  CHECK(sldbc_execute(s) == SLDBC_OK, "execute txn insert");
  sldbc_finalize(s);
  CHECK(sldbc_rollback(c) == SLDBC_OK, "rollback txn");
  CHECK(sldbc_prepare(c, "SELECT COUNT(*) FROM person", &s) == SLDBC_OK, "prepare count");
  CHECK(sldbc_query(s, &rows) == SLDBC_OK, "query count");
  CHECK(strcmp(sldbc_rows_value(rows, 0, 0), "2") == 0, "rollback undid the insert (count=2)");
  sldbc_rows_free(rows);
  sldbc_finalize(s);

  sldbc_close(c);

  if (fails) { printf("\n%d test(s) FAILED\n", fails); return 1; }
  printf("\nall db-connector tests passed\n");
  return 0;
}
