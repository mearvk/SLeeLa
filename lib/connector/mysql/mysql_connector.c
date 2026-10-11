#include "mysql_connector.h"
#include <stdlib.h>
#include <string.h>
struct sleela_mysql_connection { MYSQL *handle; char last_error[512]; unsigned long long affected_rows; };
static void copy_text(char*d,size_t cap,const char*s){size_t n;if(!d||!cap)return;if(!s)s="";n=strlen(s);if(n>=cap)n=cap-1;memcpy(d,s,n);d[n]='\0';}
static void remember_error(sleela_mysql_connection*c,const char*s){if(c)copy_text(c->last_error,sizeof c->last_error,s);}
sleela_mysql_connection *sleela_mysql_open(const sleela_mysql_config*cfg,char*err,size_t cap){
 sleela_mysql_connection*c;MYSQL*h;unsigned int timeout;if(err&&cap)err[0]='\0';
 if(!cfg||!cfg->user||!cfg->password){copy_text(err,cap,"config, user, and password are required");return NULL;}
 c=(sleela_mysql_connection*)calloc(1,sizeof *c);if(!c){copy_text(err,cap,"out of memory");return NULL;}
 h=mysql_init(NULL);if(!h){copy_text(err,cap,"mysql_init failed");free(c);return NULL;}
 timeout=cfg->connect_timeout_seconds;if(timeout&&mysql_options(h,MYSQL_OPT_CONNECT_TIMEOUT,&timeout)!=0){copy_text(err,cap,mysql_error(h));mysql_close(h);free(c);return NULL;}
 c->handle=mysql_real_connect(h,(cfg->host&&cfg->host[0])?cfg->host:"localhost",cfg->user,cfg->password,(cfg->database&&cfg->database[0])?cfg->database:NULL,cfg->port,(cfg->unix_socket&&cfg->unix_socket[0])?cfg->unix_socket:NULL,cfg->client_flags);
 if(!c->handle){copy_text(err,cap,mysql_error(h));mysql_close(h);free(c);return NULL;}return c;
}
void sleela_mysql_close(sleela_mysql_connection*c){if(!c)return;if(c->handle)mysql_close(c->handle);memset(c,0,sizeof *c);free(c);}
sleela_mysql_status sleela_mysql_ping(sleela_mysql_connection*c){if(!c||!c->handle)return SLEELA_MYSQL_NOT_CONNECTED;if(mysql_ping(c->handle)!=0){remember_error(c,mysql_error(c->handle));return SLEELA_MYSQL_CLIENT_ERROR;}c->last_error[0]='\0';return SLEELA_MYSQL_OK;}
sleela_mysql_status sleela_mysql_execute(sleela_mysql_connection*c,const char*sql){if(!c||!c->handle)return SLEELA_MYSQL_NOT_CONNECTED;if(!sql||!sql[0]){remember_error(c,"SQL statement is empty");return SLEELA_MYSQL_INVALID_ARGUMENT;}if(mysql_real_query(c->handle,sql,(unsigned long)strlen(sql))!=0){remember_error(c,mysql_error(c->handle));return SLEELA_MYSQL_CLIENT_ERROR;}c->affected_rows=(unsigned long long)mysql_affected_rows(c->handle);c->last_error[0]='\0';MYSQL_RES*r=mysql_store_result(c->handle);if(r)mysql_free_result(r);else if(mysql_field_count(c->handle)!=0){remember_error(c,mysql_error(c->handle));return SLEELA_MYSQL_CLIENT_ERROR;}return SLEELA_MYSQL_OK;}
MYSQL_STMT *sleela_mysql_prepare(sleela_mysql_connection*c,const char*sql){if(!c||!c->handle||!sql||!sql[0]){if(c)remember_error(c,"invalid prepared SQL");return NULL;}MYSQL_STMT*s=mysql_stmt_init(c->handle);if(!s){remember_error(c,"prepared statement allocation failed");return NULL;}if(mysql_stmt_prepare(s,sql,(unsigned long)strlen(sql))!=0){remember_error(c,mysql_stmt_error(s));mysql_stmt_close(s);return NULL;}return s;}
int sleela_mysql_stmt_bind_params(MYSQL_STMT*s,MYSQL_BIND*b){return s&&b?mysql_stmt_bind_param(s,b):1;}
int sleela_mysql_stmt_bind_results(MYSQL_STMT*s,MYSQL_BIND*b){return s&&b?mysql_stmt_bind_result(s,b):1;}
int sleela_mysql_stmt_execute(MYSQL_STMT*s){return s?mysql_stmt_execute(s):1;}
int sleela_mysql_stmt_store_result(MYSQL_STMT*s){return s?mysql_stmt_store_result(s):1;}
int sleela_mysql_stmt_fetch(MYSQL_STMT*s){return s?mysql_stmt_fetch(s):1;}
const char*sleela_mysql_stmt_error(MYSQL_STMT*s){return s?mysql_stmt_error(s):"statement is NULL";}
void sleela_mysql_stmt_close(MYSQL_STMT*s){if(s)mysql_stmt_close(s);}
unsigned long long sleela_mysql_affected_rows(const sleela_mysql_connection*c){return c?c->affected_rows:0ULL;}
size_t sleela_mysql_last_error(const sleela_mysql_connection*c,char*b,size_t cap){const char*s=c?c->last_error:"connection is NULL";size_t n=strlen(s)+1;copy_text(b,cap,s);return n;}
const char*sleela_mysql_client_version(void){return mysql_get_client_info();}
