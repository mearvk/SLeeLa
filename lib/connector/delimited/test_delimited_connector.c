#include "delimited_connector.h"
#include <assert.h>
#include <string.h>
int main(void){sleela_delimited_row r={0};char e[128];assert(sleela_delimited_parse("\"a,b\",\"c\"\"d\",e",',',&r,e,sizeof e));assert(r.count==3);assert(strcmp(r.fields[0],"a,b")==0);assert(strcmp(r.fields[1],"c\"d")==0);assert(strcmp(r.fields[2],"e")==0);sleela_delimited_row_free(&r);assert(!sleela_delimited_parse("\"unfinished",',',&r,e,sizeof e));assert(!sleela_delimited_parse("a\"b,c",',',&r,e,sizeof e));return 0;}
