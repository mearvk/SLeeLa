#include "PrintComponent.h"
#include "PrintLine.h"
#include "PrintLayout.h"
#include <stdio.h>
void cr_print_component(FILE*o,const char*n,unsigned long id,unsigned long d,const char*m){char b[2048];snprintf(b,sizeof b,"-- : [Object ID: %010lu] [Date: %lu] [Current: @%s] . %s .",id,d,n,m);cr_print_line(o,b,CR_PRINT_WIDTH);}
