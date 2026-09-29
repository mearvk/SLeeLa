#include "PrintField.h"
#include <string.h>
void cr_print_field(FILE*o,const char*s,int w){size_t n=strlen(s);fputs(s,o);for(int i=(int)n;i<w;i++)fputc(' ',o);}
