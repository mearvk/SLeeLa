#include "sleela_sql.hpp"
namespace sleela::sql {
Database::Database(const std::string& directory) { status_ = ssql_open(&db_, directory.c_str()); }
ssql_status Database::execute(const std::string& text, FILE* output, ssql_dialect dialect) {
    return status_ == SSQL_OK ? ssql_exec_dialect(&db_, text.c_str(), dialect, output) : status_;
}
ssql_status Database::prepare(const std::string& text, Statement& statement, ssql_dialect dialect) {
    if (status_ != SSQL_OK) return status_;
    ssql_finalize(statement.stmt_);
    statement.stmt_ = nullptr;
    return ssql_prepare(&db_, text.c_str(), dialect, &statement.stmt_);
}
Statement::~Statement() { ssql_finalize(stmt_); }
Statement::Statement(Statement&& other) noexcept : stmt_(other.stmt_) { other.stmt_ = nullptr; }
Statement& Statement::operator=(Statement&& other) noexcept {
    if (this != &other) { ssql_finalize(stmt_); stmt_ = other.stmt_; other.stmt_ = nullptr; }
    return *this;
}
ssql_status Statement::bind(int index, const std::string& value) {
    return stmt_ ? ssql_bind(stmt_, index, value.c_str()) : SSQL_ERR_ARG;
}
void Statement::reset() noexcept { if (stmt_) ssql_reset(stmt_); }
ssql_status Statement::run(FILE* output) { return stmt_ ? ssql_run(stmt_, output) : SSQL_ERR_ARG; }
int Statement::parameter_count() const noexcept { return stmt_ ? ssql_param_count(stmt_) : 0; }
const char* error_message(ssql_status s) noexcept { return ssql_strerror(s); }
const char* dialect_name(ssql_dialect d) noexcept { return ssql_dialect_name(d); }
}
