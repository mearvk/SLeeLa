#include "sociology_math.h"
#include <stddef.h>
#include <math.h>
double sleela_soc_proportion(double p,double t){return t!=0.0?p/t:0.0;}
double sleela_soc_rate(double e,double p,double scale){return p!=0.0?e/p*scale:0.0;}
double sleela_soc_mean(const double*x,size_t n){if(!x||!n)return 0.0;double s=0;for(size_t i=0;i<n;i++)s+=x[i];return s/(double)n;}
double sleela_soc_variance(const double*x,size_t n){if(!x||n<2)return 0.0;double m=sleela_soc_mean(x,n),s=0;for(size_t i=0;i<n;i++){double d=x[i]-m;s+=d*d;}return s/(double)(n-1);}
double sleela_soc_weighted_mean(const double*x,const double*w,size_t n){if(!x||!w||!n)return 0.0;double sx=0,sw=0;for(size_t i=0;i<n;i++){sx+=x[i]*w[i];sw+=w[i];}return sw!=0.0?sx/sw:0.0;}
double sleela_soc_growth_rate(double oldv,double newv){return oldv!=0.0?(newv-oldv)/oldv:0.0;}
double sleela_soc_risk_ratio(double ec,double et,double cc,double ct){double a=et!=0.0?ec/et:0.0,b=ct!=0.0?cc/ct:0.0;return b!=0.0?a/b:0.0;}
double sleela_soc_odds_ratio(double a,double b,double c,double d){return b!=0.0&&c!=0.0?(a*d)/(b*c):0.0;}
double sleela_soc_gini(const double*x,size_t n){if(!x||!n)return 0.0;double sum=0,absdiff=0;for(size_t i=0;i<n;i++)sum+=x[i];if(sum==0.0)return 0.0;for(size_t i=0;i<n;i++)for(size_t j=0;j<n;j++)absdiff+=fabs(x[i]-x[j]);return absdiff/(2.0*(double)n*sum);}
double sleela_soc_transition_probability(double tr,double origin){return origin!=0.0?tr/origin:0.0;}
double sleela_soc_dissimilarity(double a,double b,double at,double bt){if(at==0.0||bt==0.0)return 0.0;return 0.5*fabs(a/at-b/bt);}
