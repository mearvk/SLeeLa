#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "sociology_math.h"
int main(void){double x[]={1,2,3,4};double w[]={1,1,2,2};assert(fabs(sleela_soc_proportion(25,100)-.25)<1e-12);assert(fabs(sleela_soc_rate(5,100,1000)-50)<1e-12);assert(fabs(sleela_soc_mean(x,4)-2.5)<1e-12);assert(fabs(sleela_soc_variance(x,4)-1.6666666666666667)<1e-12);assert(fabs(sleela_soc_weighted_mean(x,w,4)-3.0)<1e-12);assert(fabs(sleela_soc_growth_rate(100,110)-.1)<1e-12);assert(fabs(sleela_soc_transition_probability(30,100)-.3)<1e-12);assert(fabs(sleela_soc_dissimilarity(40,30,100,100)-.05)<1e-12);puts("sociology: PASS");return 0;}
