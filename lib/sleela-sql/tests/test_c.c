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
    if (ssql_exec(&db, "ALTER TABLE items ADD COLUMN category DEFAULT 'general'", NULL) != SSQL_OK) return 19;
    if (ssql_exec(&db, "ALTER TABLE items ADD COLUMN category", NULL) != SSQL_ERR_DUPCOL) return 20;
    out = tmpfile();
    if (!out) return 21;
    status = ssql_exec(&db, "SELECT category FROM items WHERE id = 1", out);
    rewind(out);
    char alter_header[128], alter_value[128];
    int alter_ok = status == SSQL_OK &&
                   fgets(alter_header, sizeof alter_header, out) != NULL &&
                   fgets(alter_value, sizeof alter_value, out) != NULL &&
                   strcmp(alter_value, "general\n") == 0;
    fclose(out);
    if (!alter_ok) return 22;
    out = tmpfile();
    if (!out) return 11;
    status = ssql_exec(&db, "SELECT COUNT(*) FROM items WHERE id = 1", out);
    rewind(out);
    char header[128], count[128];
    int count_ok = status == SSQL_OK && fgets(header, sizeof header, out) != NULL &&
                   fgets(count, sizeof count, out) != NULL && strcmp(count, "1\n") == 0;
    fclose(out);
    if (!count_ok) return 12;

    if (ssql_exec(&db, "UPDATE items SET name = 'gamma' WHERE id = 1", NULL) != SSQL_OK) return 13;
    out = tmpfile();
    if (!out) return 14;
    status = ssql_exec(&db, "SELECT name FROM items WHERE id = 1", out);
    rewind(out);
    char updated_header[128], updated_value[128];
    int update_ok = status == SSQL_OK &&
                    fgets(updated_header, sizeof updated_header, out) != NULL &&
                    fgets(updated_value, sizeof updated_value, out) != NULL &&
                    strcmp(updated_value, "gamma\n") == 0;
    fclose(out);
    if (!update_ok) return 15;

    if (ssql_exec(&db, "DELETE FROM items WHERE id = 1", NULL) != SSQL_OK) return 16;
    out = tmpfile();
    if (!out) return 17;
    status = ssql_exec(&db, "SELECT COUNT(*) FROM items", out);
    rewind(out);
    char delete_header[128], delete_count[128];
    int delete_ok = status == SSQL_OK &&
                    fgets(delete_header, sizeof delete_header, out) != NULL &&
                    fgets(delete_count, sizeof delete_count, out) != NULL &&
                    strcmp(delete_count, "0\n") == 0;
    fclose(out);
    return delete_ok ? 0 : 18;
}
