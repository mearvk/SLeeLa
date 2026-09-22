#include "sleela_database.h"
#include <stdio.h>
static size_t blen(const char*s,size_t m){size_t n=0;if(!s)return m+1;while(n<=m&&s[n])++n;return n;}
static int ne(const char*s,size_t m){return s&&*s&&blen(s,m)<=m;}
const char *sleela_database_kind_name(sleela_database_kind_t k){switch(k){case SLEELA_DB_POSTGRESQL:return "postgresql";case SLEELA_DB_MYSQL:return "mysql";case SLEELA_DB_MARIADB:return "mariadb";case SLEELA_DB_SQLITE:return "sqlite";case SLEELA_DB_SQLSERVER:return "sqlserver";case SLEELA_DB_ORACLE:return "oracle";case SLEELA_DB_ODBC:return "odbc";default:return NULL;}}
int sleela_database_validate(const sleela_database_config_t*c,char*e,size_t es){if(e&&es)e[0]=0;if(!c||!sleela_database_kind_name(c->kind)){if(e&&es)snprintf(e,es,"invalid database configuration");return -1;}if(c->kind==SLEELA_DB_SQLITE){if(!ne(c->database,4096)){if(e&&es)snprintf(e,es,"SQLite database path required");return -1;}}else if(!ne(c->host,255)||!c->port||!ne(c->database,4096)){if(e&&es)snprintf(e,es,"host, port, and database are required");return -1;}if(!ne(c->password_env,256)){if(e&&es)snprintf(e,es,"password environment variable is required");return -1;}return 0;}
