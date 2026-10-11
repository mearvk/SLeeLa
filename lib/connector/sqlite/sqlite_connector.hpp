#ifndef SLEELA_SQLITE_CONNECTOR_HPP
#define SLEELA_SQLITE_CONNECTOR_HPP
#include "sqlite_connector.h"
#include <stdexcept>
#include <string>
#include <utility>
namespace sleela::connector {
class SQLiteConnection {
 sleela_sqlite c_{};
public:
 explicit SQLiteConnection(const std::string& path){int rc=sleela_sqlite_open(&c_,path.c_str());if(rc!=SQLITE_OK)throw std::runtime_error(sleela_sqlite_error(&c_));}
 ~SQLiteConnection(){sleela_sqlite_close(&c_);}
 SQLiteConnection(const SQLiteConnection&)=delete; SQLiteConnection& operator=(const SQLiteConnection&)=delete;
 SQLiteConnection(SQLiteConnection&& o) noexcept : c_(o.c_){o.c_.db=nullptr;}
 const sleela_sqlite* native()const noexcept{return &c_;}
 sleela_sqlite* native()noexcept{return &c_;}
 std::string error()const{return sleela_sqlite_error(&c_);}
 long long changes()const{return (long long)sleela_sqlite_changes(&c_);}
};
}
#endif
