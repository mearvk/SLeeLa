#include "sleela_sql.h"
#include <stdio.h>
#include <string.h>
int main(int argc, char **argv) {
    if (argc != 2) return 2;
    ssql_db db;
    if (ssql_open(&db, argv[1]) != SSQL_OK) return 3;
    if (ssql_exec(&db, "CREATE TABLE items (id, name)", NULL) != SSQL_OK) return 4;
    if (ssql_exec(&db, "INSERT INTO items VALUES (1, 'alpha')", NULL) != SSQL_OK) return 5;
    if (ssql_exec(&db, "CREATE TABLE IF NOT EXISTS items (id, name)", NULL) != SSQL_OK) return 10;
    if (ssql_exec(&db, "CREATE TABLE ../escape (x)", NULL) != SSQL_ERR_SYNTAX) return 9;
    FILE *out = tmpfile();
    if (!out) return 6;
    ssql_status status = ssql_exec(&db, "SELECT name FROM items WHERE id = 1", out);
    if (status != SSQL_OK) { fclose(out); return 7; }
    rewind(out);
    char line[128];
    int ok = fgets(line, sizeof line, out) != NULL;
    fclose(out);
    if (!ok) return 8;
    out = tmpfile();
    if (!out) return 11;
    status = ssql_exec(&db, "SELECT COUNT(*) FROM items WHERE id = 1", out);
    rewind(out);
    char header[128], count[128];
    int count_ok = status == SSQL_OK && fgets(header, sizeof header, out) != NULL &&
                   fgets(count, sizeof count, out) != NULL && strcmp(count, "1\n") == 0;
    fclose(out);
    return count_ok ? 0 : 12;
}
