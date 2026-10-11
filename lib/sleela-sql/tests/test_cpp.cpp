#include "sleela_sql.hpp"
#include <cstdio>
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
    return status == SSQL_OK ? 0 : 10;
}
