/*
 * Prepared statement using the SLSQL dialect.
 * Build the package first: make -C lib/sleela-sql
 */
#include "sleela_sql.h"
#include <stdio.h>

int main(void) {
    ssql_db db;
    ssql_stmt *stmt = NULL;
    ssql_status status = ssql_open(&db, "./slsql-example-data");
    if (status != SSQL_OK) {
        fprintf(stderr, "ssql_open failed: %s\n", ssql_strerror(status));
        return 1;
    }

    status = ssql_exec(&db, "table('people').create(id, name)",
                       NULL);
    /* A fresh example directory is expected. If rerunning, remove the
       directory first or change create to the engine's idempotent SQL form. */
    if (status != SSQL_OK) {
        fprintf(stderr, "SLSQL create failed: %s\n", ssql_strerror(status));
        return 2;
    }

    status = ssql_prepare(&db, "into('people').insert(?, ?)",
                          SSQL_DIALECT_SLEELA, &stmt);
    if (status != SSQL_OK) {
        fprintf(stderr, "ssql_prepare failed: %s\n", ssql_strerror(status));
        return 3;
    }

    status = ssql_bind(stmt, 1, "1");
    if (status == SSQL_OK) status = ssql_bind(stmt, 2, "Ada Lovelace");
    if (status == SSQL_OK) status = ssql_run(stmt, NULL);
    ssql_finalize(stmt);

    if (status != SSQL_OK) {
        fprintf(stderr, "SLSQL prepared INSERT failed: %s\n",
                ssql_strerror(status));
        return 4;
    }
    return 0;
}
