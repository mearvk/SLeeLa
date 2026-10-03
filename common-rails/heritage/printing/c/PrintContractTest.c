#include "PrintProgress.h"
#include "PrintLayout.h"
#include <assert.h>
int main(void){assert(CR_PRINT_WIDTH==80);assert(CR_PROGRESS_CELLS==441);assert(cr_clamp_percent(-1)==0);assert(cr_clamp_percent(101)==100);assert(cr_percent_cells(0)==0);assert(cr_percent_cells(100)==441);assert(cr_percent_cells(50)==220);return 0;}
