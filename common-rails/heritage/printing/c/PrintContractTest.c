#include "PrintProgress.h"
#include <assert.h>
int main(void){assert(cr_clamp_percent(-1)==0);assert(cr_clamp_percent(101)==100);assert(cr_percent_cells(0)==0);assert(cr_percent_cells(50)==220);assert(cr_percent_cells(100)==441);return 0;}
