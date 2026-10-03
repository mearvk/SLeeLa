#include "../include/sleela_regex_natural.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
static int eq(const char*s,size_t n,const char*w){return strlen(w)==n&&strncmp(s,w,n)==0;}
int sleela_regex_natural_is_word(const char*s,size_t n){return eq(s,n,"any")||eq(s,n,"digit")||eq(s,n,"letter")||eq(s,n,"space")||eq(s,n,"one")||eq(s,n,"optional")||eq(s,n,"some")||eq(s,n,"many")||eq(s,n,"begin")||eq(s,n,"end")||eq(s,n,"or");}
const char*sleela_regex_natural_symbol_name(const char*s,size_t n){if(eq(s,n,"any"))return ".";if(eq(s,n,"digit"))return "[0-9]";if(eq(s,n,"letter"))return "[[:alpha:]]";if(eq(s,n,"space"))return "[[:space:]]";if(eq(s,n,"one"))return "=";if(eq(s,n,"optional"))return "?";if(eq(s,n,"some"))return "+";if(eq(s,n,"many"))return "*";if(eq(s,n,"begin"))return "^";if(eq(s,n,"end"))return "$";if(eq(s,n,"or"))return "|";return NULL;}
static void error(char*e,size_t z,const char*m){if(e&&z){(void)snprintf(e,z,"%s",m);e[z-1]='\0';}}
sleela_regex_natural_status sleela_regex_natural_validate(const char*s,char*e,size_t z){int d=0,q=0;if(!s){error(e,z,"null pattern");return SLEELA_REGEX_NATURAL_NULL;}for(size_t i=0;s[i];){unsigned char c=(unsigned char)s[i];if(c=='"'){q=!q;++i;continue;}if(q){++i;continue;}if(c=='('||c=='['||c=='<')++d;else if(c==')'||c==']'||c=='>'){if(d==0){error(e,z,"unbalanced closing delimiter");return SLEELA_REGEX_NATURAL_UNBALANCED;}--d;}if(isalpha(c)){size_t j=i+1;while(isalpha((unsigned char)s[j]))++j;if(!sleela_regex_natural_is_word(s+i,j-i)){error(e,z,"unknown Natural Form word");return SLEELA_REGEX_NATURAL_UNKNOWN_WORD;}i=j;continue;}++i;}if(q||d){error(e,z,"unbalanced Natural Form delimiter");return SLEELA_REGEX_NATURAL_UNBALANCED;}if(e&&z)e[0]='\0';return SLEELA_REGEX_NATURAL_OK;}
