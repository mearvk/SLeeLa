#include "postgres_connector.h"
#include <stdio.h>
#include <string.h>
int sleela_postgres_connect(sleela_postgres*c,const char*info){if(!c||!info)return 0;memset(c,0,sizeof *c);c->conn=PQconnectdb(info);if(!c->conn||PQstatus(c->conn)!=CONNECTION_OK){snprintf(c->error,sizeof c->error,"%s",c->conn?PQerrorMessage(c->conn):"libpq connection allocation failed");if(c->conn)PQfinish(c->conn);c->conn=NULL;return 0;}return 1;}
void sleela_postgres_close(sleela_postgres*c){if(c&&c->conn){PQfinish(c->conn);c->conn=NULL;}}
const char*sleela_postgres_error(const sleela_postgres*c){return c?c->error:"invalid connector";}
PGresult*sleela_postgres_query_params(sleela_postgres*c,const char*sql,int n,const char*const*v){if(!c||!c->conn||!sql||n<0)return NULL;PGresult*r=PQexecParams(c->conn,sql,n,NULL,v,NULL,NULL,0);if(!r)snprintf(c->error,sizeof c->error,"%s",PQerrorMessage(c->conn));else if(PQresultStatus(r)!=PGRES_COMMAND_OK&&PQresultStatus(r)!=PGRES_TUPLES_OK)snprintf(c->error,sizeof c->error,"%s",PQresultErrorMessage(r));return r;}
int sleela_postgres_result_ok(const PGresult*r){if(!r)return 0;ExecStatusType s=PQresultStatus((PGresult*)r);return s==PGRES_COMMAND_OK||s==PGRES_TUPLES_OK;}
void sleela_postgres_result_clear(PGresult*r){if(r)PQclear(r);}
