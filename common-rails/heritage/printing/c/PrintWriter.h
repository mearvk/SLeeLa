#ifndef CR_PRINT_WRITER_H
#define CR_PRINT_WRITER_H
#include <stdio.h>
typedef struct{FILE*stream;}CrPrintWriter;CrPrintWriter cr_writer(FILE*);
#endif
