#include "json_connector.h"
#include <assert.h>
int main(void){sleela_json_token t[16];size_t n=sleela_json_tokenize("{\"a\":1,\"b\":[true]}",t,16);assert(n==6);assert(t[0].type==SLEELA_JSON_OBJECT);assert(t[4].type==SLEELA_JSON_ARRAY);assert(t[5].parent==4);assert(sleela_json_tokenize("{",t,16)==0);assert(sleela_json_tokenize("{\"a\":]}",t,16)==0);assert(sleela_json_tokenize("{} {}",t,16)==0);return 0;}
