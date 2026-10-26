/*
 * lib/sleela-sql/tests/test_txn.c
 *
 * Tests the snapshot-based transaction API: begin / commit / rollback.
 * Exit 0 = all pass; a nonzero code identifies the failing check.
 *
 * Usage: test-txn <db-dir>
 */
#include "sleela_sql.h"

#include <stdio.h>
#include <string.h>

/* Return COUNT(*) of a table, or -1 on error / missing table. */
static long count_rows(ssql_db *db, const char *table) {
    char sql[128];
    snprintf(sql, sizeof sql, "SELECT COUNT(*) FROM %s", table);
    FILE *out = tmpfile();
    if (!out) return -1;
    if (ssql_exec(db, sql, out) != SSQL_OK) { fclose(out); return -1; }
    rewind(out);
    char header[64], value[64];
    long n = -1;
    if (fgets(header, sizeof header, out) && fgets(value, sizeof value, out))
        n = strtol(value, NULL, 10);
    fclose(out);
    return n;
}

static int table_present(ssql_db *db, const char *table) {
    char sql[128];
    snprintf(sql, sizeof sql, "SELECT COUNT(*) FROM %s", table);
    return ssql_exec(db, sql, NULL) == SSQL_OK;
}

int main(int argc, char **argv) {
    if (argc != 2) return 2;
    ssql_db db;
    if (ssql_open(&db, argv[1]) != SSQL_OK) return 3;

    /* Baseline: a table with two rows. */
    if (ssql_exec(&db, "CREATE TABLE acct (id, bal)", NULL) != SSQL_OK) return 4;
    if (ssql_exec(&db, "INSERT INTO acct VALUES (1, '100')", NULL) != SSQL_OK) return 5;
    if (ssql_exec(&db, "INSERT INTO acct VALUES (2, '200')", NULL) != SSQL_OK) return 6;

    if (ssql_in_transaction(&db)) return 7;              /* none open yet */

    /* ---- ROLLBACK undoes INSERT/UPDATE/DELETE and new tables ---- */
    if (ssql_begin(&db) != SSQL_OK) return 10;
    if (!ssql_in_transaction(&db)) return 11;
    if (ssql_begin(&db) != SSQL_ERR_ARG) return 12;      /* no nesting */

    if (ssql_exec(&db, "INSERT INTO acct VALUES (3, '300')", NULL) != SSQL_OK) return 13;
    if (ssql_exec(&db, "UPDATE acct SET bal = '999' WHERE id = 1", NULL) != SSQL_OK) return 14;
    if (ssql_exec(&db, "DELETE FROM acct WHERE id = 2", NULL) != SSQL_OK) return 15;
    if (ssql_exec(&db, "CREATE TABLE temp (x)", NULL) != SSQL_OK) return 16;
    if (count_rows(&db, "acct") != 2) return 17;         /* 3 added, 1 deleted */
    if (!table_present(&db, "temp")) return 18;

    if (ssql_rollback(&db) != SSQL_OK) return 19;
    if (ssql_in_transaction(&db)) return 20;             /* journal cleared */
    if (count_rows(&db, "acct") != 2) return 21;         /* back to baseline */
    if (table_present(&db, "temp")) return 22;           /* CREATE undone */
    /* The two original rows are intact with original values. */
    {
        FILE *out = tmpfile();
        if (!out) return 23;
        ssql_exec(&db, "SELECT bal FROM acct WHERE id = 1", out);
        rewind(out);
        char h[64], v[64];
        int ok = fgets(h, sizeof h, out) && fgets(v, sizeof v, out) &&
                 strcmp(v, "100\n") == 0;                /* UPDATE undone */
        fclose(out);
        if (!ok) return 24;
    }

    /* ---- COMMIT keeps the changes ---- */
    if (ssql_begin(&db) != SSQL_OK) return 30;
    if (ssql_exec(&db, "INSERT INTO acct VALUES (4, '400')", NULL) != SSQL_OK) return 31;
    if (count_rows(&db, "acct") != 3) return 32;
    if (ssql_commit(&db) != SSQL_OK) return 33;
    if (ssql_in_transaction(&db)) return 34;
    if (count_rows(&db, "acct") != 3) return 35;         /* change persisted */

    /* ---- commit/rollback with no open txn is an error ---- */
    if (ssql_commit(&db) != SSQL_ERR_ARG) return 40;
    if (ssql_rollback(&db) != SSQL_ERR_ARG) return 41;

    printf("all sleela-sql transaction tests passed\n");
    return 0;
}
