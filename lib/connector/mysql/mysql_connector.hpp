#ifndef SLEELA_MYSQL_CONNECTOR_HPP
#define SLEELA_MYSQL_CONNECTOR_HPP
#include "mysql_connector.h"
#include <stdexcept>
#include <string>
#include <utility>
namespace sleela::connector {
class MySqlConnection {
public:
    explicit MySqlConnection(const sleela_mysql_config& cfg) {
        char error[512] = {};
        handle_ = sleela_mysql_open(&cfg, error, sizeof(error));
        if (!handle_) throw std::runtime_error(error[0] ? error : "MySQL connection failed");
    }
    ~MySqlConnection() { sleela_mysql_close(handle_); }
    MySqlConnection(const MySqlConnection&) = delete;
    MySqlConnection& operator=(const MySqlConnection&) = delete;
    MySqlConnection(MySqlConnection&& other) noexcept : handle_(std::exchange(other.handle_, nullptr)) {}
    MySqlConnection& operator=(MySqlConnection&& other) noexcept {
        if (this != &other) { sleela_mysql_close(handle_); handle_ = std::exchange(other.handle_, nullptr); }
        return *this;
    }
    bool ping() const { return handle_ && sleela_mysql_ping(handle_) == SLEELA_MYSQL_OK; }
    void execute(const std::string& sql) {
        auto status = sleela_mysql_execute(handle_, sql.c_str());
        if (status != SLEELA_MYSQL_OK) {
            char error[512] = {};
            sleela_mysql_last_error(handle_, error, sizeof(error));
            throw std::runtime_error(error[0] ? error : "MySQL execution failed");
        }
    }
    unsigned long long affectedRows() const { return sleela_mysql_affected_rows(handle_); }
    explicit operator bool() const noexcept { return handle_ != nullptr; }
private:
    sleela_mysql_connection *handle_ = nullptr;
};
}
#endif
