#ifndef SLEELA_ODBC_CONNECTOR_HPP
#define SLEELA_ODBC_CONNECTOR_HPP
#include "odbc_connector.h"
#include <stdexcept>
#include <string>
namespace sleela::connector {
class OdbcConnection {
 sleela_odbc c_{};
public:
 explicit OdbcConnection(const std::string& cs){if(!sleela_odbc_connect(&c_,cs.c_str()))throw std::runtime_error(sleela_odbc_error(&c_));}
 ~OdbcConnection(){sleela_odbc_close(&c_);}
 OdbcConnection(const OdbcConnection&)=delete; OdbcConnection& operator=(const OdbcConnection&)=delete;
 int execute(const std::string& sql){if(!sleela_odbc_exec(&c_,sql.c_str()))throw std::runtime_error(sleela_odbc_error(&c_));return 1;}
 long long rowCount()const{return (long long)sleela_odbc_row_count(const_cast<sleela_odbc*>(&c_));}
};
}
#endif
