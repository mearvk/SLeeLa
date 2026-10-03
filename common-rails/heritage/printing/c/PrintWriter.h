#ifndef COMMONRAILS_PRINT_WRITER_H
#define COMMONRAILS_PRINT_WRITER_H
#include <stdio.h>
typedef struct { FILE *stream; } CrPrintWriter; CrPrintWriter cr_writer(FILE *stream);
#endif
