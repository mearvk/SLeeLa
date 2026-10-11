#ifndef SLEELA_MYSQL_CONNECTOR_HPP
#define SLEELA_MYSQL_CONNECTOR_HPP
#include "mysql_connector.h"
#include <stdexcept>
#include <string>
#include <utility>
namespace sleela::connector {
class MySqlStatement {
 MYSQL_STMT *stmt_=nullptr;
public:
 explicit MySqlStatement(MYSQL_STMT*s):stmt_(s){if(!stmt_)throw std::runtime_error("MySQL prepared statement creation failed");}
 ~MySqlStatement(){sleela_mysql_stmt_close(stmt_);}
 MySqlStatement(const MySqlStatement&)=delete;MySqlStatement&operator=(const MySqlStatement&)=delete;
 MySqlStatement(MySqlStatement&&o)noexcept:stmt_(std::exchange(o.stmt_,nullptr)){}
 MySqlStatement&operator=(MySqlStatement&&o)noexcept{if(this!=&o){sleela_mysql_stmt_close(stmt_);stmt_=std::exchange(o.stmt_,nullptr);}return *this;}
 MYSQL_STMT*native()noexcept{return stmt_;}
 void bindParams(MYSQL_BIND*b){if(sleela_mysql_stmt_bind_params(stmt_,b)!=0)throw std::runtime_error(sleela_mysql_stmt_error(stmt_));}
 void bindResults(MYSQL_BIND*b){if(sleela_mysql_stmt_bind_results(stmt_,b)!=0)throw std::runtime_error(sleela_mysql_stmt_error(stmt_));}
 void execute(){if(sleela_mysql_stmt_execute(stmt_)!=0)throw std::runtime_error(sleela_mysql_stmt_error(stmt_));}
 int fetch(){int r=sleela_mysql_stmt_fetch(stmt_);if(r!=0&&r!=MYSQL_NO_DATA&&r!=MYSQL_DATA_TRUNCATED)throw std::runtime_error(sleela_mysql_stmt_error(stmt_));return r;}
};
class MySqlConnection {
 sleela_mysql_connection*handle_=nullptr;
public:
 explicit MySqlConnection(const sleela_mysql_config&cfg){char e[512]={};handle_=sleela_mysql_open(&cfg,e,sizeof e);if(!handle_)throw std::runtime_error(e[0]?e:"MySQL connection failed");}
 ~MySqlConnection(){sleela_mysql_close(handle_);}
 MySqlConnection(const MySqlConnection&)=delete;MySqlConnection&operator=(const MySqlConnection&)=delete;
 MySqlConnection(MySqlConnection&&o)noexcept:handle_(std::exchange(o.handle_,nullptr)){}
 MySqlConnection&operator=(MySqlConnection&&o)noexcept{if(this!=&o){sleela_mysql_close(handle_);handle_=std::exchange(o.handle_,nullptr);}return *this;}
 bool ping()const{return handle_&&sleela_mysql_ping(handle_)==SLEELA_MYSQL_OK;}
 void execute(const std::string&sql){auto s=sleela_mysql_execute(handle_,sql.c_str());if(s!=SLEELA_MYSQL_OK){char e[512]={};sleela_mysql_last_error(handle_,e,sizeof e);throw std::runtime_error(e[0]?e:"MySQL execution failed");}}
 MySqlStatement prepare(const std::string&sql){return MySqlStatement(sleela_mysql_prepare(handle_,sql.c_str()));}
 unsigned long long affectedRows()const{return sleela_mysql_affected_rows(handle_);}
 explicit operator bool()const noexcept{return handle_!=nullptr;}
};
}
#endif
