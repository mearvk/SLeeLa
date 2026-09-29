#ifndef COMMONRAILS_PRINT_FORMATTER_H
#define COMMONRAILS_PRINT_FORMATTER_H
#include <stdio.h>
#include "PrintState.h"
void cr_format_status(FILE*out,CrPrintState state,const char*message);
#endif
