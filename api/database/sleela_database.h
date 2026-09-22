#ifndef SLEELA_DATABASE_H
#define SLEELA_DATABASE_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SLEELA_DB_POSTGRESQL=1,SLEELA_DB_MYSQL,SLEELA_DB_MARIADB,SLEELA_DB_SQLITE,SLEELA_DB_SQLSERVER,SLEELA_DB_ORACLE,SLEELA_DB_ODBC } sleela_database_kind_t;
typedef struct { sleela_database_kind_t kind; const char *host; unsigned short port; const char *database; const char *username; const char *password_env; const char *connection_options; } sleela_database_config_t;
int sleela_database_validate(const sleela_database_config_t*,char*,size_t);
const char *sleela_database_kind_name(sleela_database_kind_t);
#ifdef __cplusplus
}
#endif
#endif
