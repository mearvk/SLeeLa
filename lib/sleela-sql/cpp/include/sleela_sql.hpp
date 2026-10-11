#ifndef SLEELA_SQL_HPP
#define SLEELA_SQL_HPP
#include "sleela_sql.h"
#include <string>
namespace sleela::sql {
class Statement;
class Database {
public:
    explicit Database(const std::string& directory);
    ssql_status status() const noexcept { return status_; }
    ssql_status execute(const std::string& text, FILE* output = stdout,
                        ssql_dialect dialect = SSQL_DIALECT_AUTO);
    ssql_status prepare(const std::string& text, Statement& statement,
                        ssql_dialect dialect = SSQL_DIALECT_AUTO);
private:
    ssql_db db_{};
    ssql_status status_{SSQL_ERR_ARG};
};
class Statement {
public:
    Statement() noexcept = default;
    ~Statement();
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;
    Statement(Statement&& other) noexcept;
    Statement& operator=(Statement&& other) noexcept;
    ssql_status bind(int one_based_index, const std::string& value);
    void reset() noexcept;
    ssql_status run(FILE* output = stdout);
    int parameter_count() const noexcept;
    explicit operator bool() const noexcept { return stmt_ != nullptr; }
private:
    friend class Database;
    ssql_stmt* stmt_{nullptr};
};
const char* error_message(ssql_status status) noexcept;
const char* dialect_name(ssql_dialect dialect) noexcept;
}
#endif
