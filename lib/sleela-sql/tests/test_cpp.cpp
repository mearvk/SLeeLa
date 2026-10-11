#include "sleela_sql.hpp"
#include <cstdio>
#include <string>
int main(int argc, char** argv) {
    if (argc != 2) return 2;
    sleela::sql::Database db(argv[1]);
    if (db.status() != SSQL_OK) return 3;
    if (db.execute("table('items').create(id, name)", nullptr) != SSQL_OK) return 4;
    sleela::sql::Statement insert;
    if (db.prepare("into('items').insert(?, ?)", insert, SSQL_DIALECT_SLEELA) != SSQL_OK) return 5;
    if (insert.parameter_count() != 2) return 6;
    if (insert.bind(1, "7") != SSQL_OK || insert.bind(2, "beta") != SSQL_OK) return 7;
    if (insert.run(nullptr) != SSQL_OK) return 8;
    FILE* out = std::tmpfile();
    if (!out) return 9;
    auto status = db.execute("SELECT COUNT(*) FROM items WHERE id = '7'", out);
    std::fclose(out);
    if (status != SSQL_OK) return 10;
    if (db.execute("UPDATE items SET name = 'delta' WHERE id = '7'", nullptr) != SSQL_OK) return 11;
    if (db.execute("DELETE FROM items WHERE id = '7'", nullptr) != SSQL_OK) return 12;
    out = std::tmpfile();
    if (!out) return 13;
    status = db.execute("SELECT COUNT(*) FROM items", out);
    std::rewind(out);
    char header[128], count[128];
    bool ok = status == SSQL_OK &&
              std::fgets(header, sizeof header, out) != nullptr &&
              std::fgets(count, sizeof count, out) != nullptr &&
              std::string(count) == "0\n";
    std::fclose(out);
    return ok ? 0 : 14;
}
