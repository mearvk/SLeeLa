// SQL connector abstraction for SST/Nordshrift. Database client loading is deferred.
#ifndef NORDSHRIFT_SQL_CONNECTOR_H
#define NORDSHRIFT_SQL_CONNECTOR_H
#include <string>
namespace nordshrift {
enum class SQLProvider { MySQL, PostgreSQL };
struct SQLConnectionConfig { SQLProvider provider; std::string host; unsigned port; std::string database; std::string user; };
struct SQLConnectionResult { bool supported=false; bool connected=false; std::string diagnostic; };
SQLConnectionResult validateSQLConfiguration(const SQLConnectionConfig&);
SQLConnectionResult connectSQL(const SQLConnectionConfig&);
SQLConnectionResult disconnectSQL(const SQLConnectionConfig&);
SQLConnectionResult installSQLClient(SQLProvider);
}
#endif
