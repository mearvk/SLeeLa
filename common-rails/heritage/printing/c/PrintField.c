#include "PrintField.h"
#include "PrintLayout.h"
#include <string.h>
void cr_print_field(FILE *out,const char *text,int width){size_t n=strlen(text);fputs(text,out);for(int i=(int)n;i<width;i++)fputc(' ',out);}
