#ifndef COMMONRAILS_PRINT_PROGRESS_H
#define COMMONRAILS_PRINT_PROGRESS_H
#include <stdio.h>
int cr_clamp_percent(int percent); int cr_percent_cells(int percent); void cr_print_progress(FILE *out,int percent);
#endif
