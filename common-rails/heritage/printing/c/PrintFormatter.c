#include "PrintFormatter.h"
#include "PrintLine.h"
#include <stdio.h>
void cr_format_status(FILE*o,CrPrintState s,const char*m){char b[1024];snprintf(b,sizeof b,"[%s] %s",cr_print_state_name(s),m);cr_print_line(o,b,80);}
