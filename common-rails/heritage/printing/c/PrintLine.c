#include "PrintLine.h"
#include <string.h>
static void sp(FILE*o,int n){while(n-->0)fputc(' ',o);} void cr_print_line(FILE*o,const char*s,int w){const char*p=s;while(*p){size_t r=strlen(p),n=r<=(size_t)w?r:(size_t)w;if(r>(size_t)w){size_t c=n;while(c&&p[c-1]!=' '&&p[c]!=' ')--c;if(c)n=c;}fwrite(p,1,n,o);sp(o,w-(int)n);fputc('\n',o);p+=n;while(*p==' ')p++;}}
