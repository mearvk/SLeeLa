/*
 * sleela_sql_cli.c -- command-line front-end for the CSV-backed SQL engine.
 * Part of mearvk/Nintendo.
 *
 * Accepts BOTH surface languages -- classic SQL and the fluent SLeeLaSQL -- and
 * auto-detects which one each statement uses.
 *
 * Usage:
 *   sleela-sql <db-dir> [--sql|--sleela] "<statement>"   run one statement
 *   sleela-sql <db-dir> [--sql|--sleela]                  read from stdin
 *                                                         (one per line)
 *
 * SQL example:
 *   sleela-sql ./data "SELECT title, year FROM games WHERE id = '1'"
 *
 * SLeeLaSQL example (same effect, cleaner surface):
 *   sleela-sql ./data "from('games').select(title, year).where(id == '1')"
 *
 * By default the dialect is sniffed per statement; --sql / --sleela force one.
 * Row output (SELECT / SHOW TABLES / tables()) is printed to stdout as CSV.
 */
#include "sleela_sql.h"

#include <stdio.h>
#include <string.h>

static int run_one(ssql_db *db, ssql_dialect d, const char *text) {
    ssql_status s = ssql_exec_dialect(db, text, d, stdout);
    if (s != SSQL_OK) {
        fprintf(stderr, "sleela-sql: %s\n", ssql_strerror(s));
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr,
            "usage: %s <db-dir> [--sql|--sleela] [\"<statement>\"]\n"
            "  statements may be classic SQL or fluent SLeeLaSQL (auto-detected)\n"
            "  --sql / --sleela force a dialect; omit to sniff per statement\n"
            "  with a statement argument: run one; without: read stdin lines\n",
            argv[0]);
        return 2;
    }

    const char *dir = argv[1];
    ssql_dialect dialect = SSQL_DIALECT_AUTO;
    int ai = 2;
    if (ai < argc && strcmp(argv[ai], "--sql") == 0)    { dialect = SSQL_DIALECT_SQL;    ai++; }
    else if (ai < argc && strcmp(argv[ai], "--sleela") == 0) { dialect = SSQL_DIALECT_SLEELA; ai++; }

    ssql_db db;
    if (ssql_open(&db, dir) != SSQL_OK) {
        fprintf(stderr, "sleela-sql: could not open db at %s\n", dir);
        return 1;
    }

    if (ai < argc) {
        return run_one(&db, dialect, argv[ai]);
    }

    /* stdin mode: one statement per line, blank lines and # comments ignored */
    char line[4096];
    int rc = 0;
    while (fgets(line, sizeof(line), stdin)) {
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '\0' || *p == '\n' || *p == '#') continue;
        if (run_one(&db, dialect, p) != 0) rc = 1;
    }
    return rc;
}
