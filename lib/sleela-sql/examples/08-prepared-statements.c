/*
 * Prepared-statement example: bind values rather than assembling SQL text.
 *
 * Build the package first with: make -C lib/sleela-sql
 * This is a standalone usage example, not part of the default test suite.
 */
#include "sleela_sql.h"
#include <stdio.h>

int main(void) {
    ssql_db db;
    ssql_stmt *stmt = NULL;
    ssql_status status = ssql_open(&db, "./example-data");
    if (status != SSQL_OK) {
        fprintf(stderr, "ssql_open failed: %s\n", ssql_strerror(status));
        return 1;
    }

    status = ssql_exec(&db, "CREATE TABLE IF NOT EXISTS people (id, name)", NULL);
    if (status != SSQL_OK) {
        fprintf(stderr, "CREATE TABLE failed: %s\n", ssql_strerror(status));
        return 2;
    }

    status = ssql_prepare(&db, "INSERT INTO people VALUES (?, ?)", SSQL_DIALECT_SQL, &stmt);
    if (status != SSQL_OK) {
        fprintf(stderr, "ssql_prepare failed: %s\n", ssql_strerror(status));
        return 3;
    }

    status = ssql_bind(stmt, 1, "1");
    if (status == SSQL_OK) status = ssql_bind(stmt, 2, "Ada Lovelace");
    if (status == SSQL_OK) status = ssql_run(stmt, NULL);

    ssql_finalize(stmt);
    if (status != SSQL_OK) {
        fprintf(stderr, "prepared INSERT failed: %s\n", ssql_strerror(status));
        return 4;
    }
    return 0;
}
