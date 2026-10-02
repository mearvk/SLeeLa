// SQL connector boundary. Provider client installation/loading is deliberately deferred.
#include "sql_connector.h"
namespace nordshrift {
SQLConnectionResult validateSQLConfiguration(const SQLConnectionConfig& c) {
 SQLConnectionResult r; r.supported=(c.provider==SQLProvider::MySQL||c.provider==SQLProvider::PostgreSQL);
 if(!r.supported) r.diagnostic="unsupported SQL provider"; else if(c.host.empty()||c.database.empty()) r.diagnostic="SQL host and database are required";
 return r;
}
SQLConnectionResult connectSQL(const SQLConnectionConfig& c) { SQLConnectionResult r=validateSQLConfiguration(c); if(r.supported) r.diagnostic="SQL client connection is not installed in this connector layer yet"; return r; }
SQLConnectionResult disconnectSQL(const SQLConnectionConfig& c) { SQLConnectionResult r=validateSQLConfiguration(c); if(r.supported) r.diagnostic="SQL disconnect requires an installed provider client"; return r; }
SQLConnectionResult installSQLClient(SQLProvider) { SQLConnectionResult r; r.diagnostic="SQL client installation is planned but not implemented"; return r; }
}
