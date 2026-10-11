#include "sqlite_connector.h"
#include <stdio.h>
#include <string.h>
static void seterr(sleela_sqlite *c, const char *s) { if (!c) return; snprintf(c->error,sizeof c->error,"%s",s?s:"unknown error"); }
int sleela_sqlite_open(sleela_sqlite *c, const char *path) {
 if (!c || !path || !*path) return SQLITE_MISUSE;
 memset(c,0,sizeof *c);
 int rc=sqlite3_open_v2(path,&c->db,SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|SQLITE_OPEN_FULLMUTEX,NULL);
 if(rc!=SQLITE_OK){seterr(c,c->db?sqlite3_errmsg(c->db):"sqlite open failed"); if(c->db) sqlite3_close(c->db); c->db=NULL;}
 else sqlite3_busy_timeout(c->db,5000);
 return rc;
}
void sleela_sqlite_close(sleela_sqlite *c){if(c&&c->db){sqlite3_close_v2(c->db);c->db=NULL;}}
const char *sleela_sqlite_error(const sleela_sqlite *c){return c?c->error:"invalid connector";}
int sleela_sqlite_prepare(sleela_sqlite *c,const char *sql,sqlite3_stmt **out){if(!c||!c->db||!sql||!out)return SQLITE_MISUSE;*out=NULL;int rc=sqlite3_prepare_v2(c->db,sql,-1,out,NULL);if(rc!=SQLITE_OK)seterr(c,sqlite3_errmsg(c->db));return rc;}
int sleela_sqlite_bind_text(sqlite3_stmt*s,int i,const char*v){return s?sqlite3_bind_text(s,i,v,-1,SQLITE_TRANSIENT):SQLITE_MISUSE;}
int sleela_sqlite_bind_int64(sqlite3_stmt*s,int i,int64_t v){return s?sqlite3_bind_int64(s,i,(sqlite3_int64)v):SQLITE_MISUSE;}
int sleela_sqlite_bind_double(sqlite3_stmt*s,int i,double v){return s?sqlite3_bind_double(s,i,v):SQLITE_MISUSE;}
int sleela_sqlite_bind_null(sqlite3_stmt*s,int i){return s?sqlite3_bind_null(s,i):SQLITE_MISUSE;}
int sleela_sqlite_step(sqlite3_stmt*s){return s?sqlite3_step(s):SQLITE_MISUSE;}
void sleela_sqlite_finalize(sqlite3_stmt*s){if(s)sqlite3_finalize(s);}
int64_t sleela_sqlite_changes(const sleela_sqlite*c){return c&&c->db?(int64_t)sqlite3_changes(c->db):0;}
