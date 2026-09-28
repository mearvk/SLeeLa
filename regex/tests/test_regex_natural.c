#include "../include/sleela_regex_natural.h"
#include <assert.h>
#include <string.h>
int main(void){char e[128];assert(sleela_regex_natural_validate("begin digit+ end",e,sizeof e)==SLEELA_REGEX_NATURAL_OK);assert(strcmp(sleela_regex_natural_symbol_name("digit",5),"[0-9]")==0);assert(sleela_regex_natural_validate("begin mystery end",e,sizeof e)==SLEELA_REGEX_NATURAL_UNKNOWN_WORD);assert(sleela_regex_natural_validate("(digit+",e,sizeof e)==SLEELA_REGEX_NATURAL_UNBALANCED);return 0;}
