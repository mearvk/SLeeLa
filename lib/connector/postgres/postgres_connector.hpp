#ifndef SLEELA_POSTGRES_CONNECTOR_HPP
#define SLEELA_POSTGRES_CONNECTOR_HPP
#include "postgres_connector.h"
#include <stdexcept>
#include <string>
namespace sleela::connector {
class PostgresConnection {
 sleela_postgres c_{};
public:
 explicit PostgresConnection(const std::string& conninfo){if(!sleela_postgres_connect(&c_,conninfo.c_str()))throw std::runtime_error(sleela_postgres_error(&c_));}
 ~PostgresConnection(){sleela_postgres_close(&c_);}
 PostgresConnection(const PostgresConnection&)=delete; PostgresConnection& operator=(const PostgresConnection&)=delete;
 sleela_postgres* native() noexcept{return &c_;}
 std::string error()const{return sleela_postgres_error(&c_);}
};
}
#endif
