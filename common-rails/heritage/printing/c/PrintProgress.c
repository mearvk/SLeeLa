#include "PrintProgress.h"
#include "PrintLayout.h"
#include <stdio.h>
int cr_clamp_percent(int p){return p<0?0:p>100?100:p;} int cr_percent_cells(int p){return cr_clamp_percent(p)*CR_PROGRESS_CELLS/100;}
void cr_print_progress(FILE*o,int p){p=cr_clamp_percent(p);fprintf(o,"  progress %d%% (%d/%d cells)\n",p,cr_percent_cells(p),CR_PROGRESS_CELLS);}
