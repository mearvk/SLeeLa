#include "sleela_data_analytics.h"
#include <math.h>
#include <stdio.h>
static void fail(char *e,size_t n,const char*m){if(e&&n){snprintf(e,n,"%s",m);e[n-1]='\0';}}
int sleela_data_analytics_validate(const sleela_data_analytics_set_t*s,char*e,size_t n){if(!s||!s->values){fail(e,n,"analytics values are required");return -1;}if(!s->count||s->count>SLEELA_ANALYTICS_MAX_VALUES){fail(e,n,"analytics value count out of range");return -1;}for(size_t i=0;i<s->count;i++)if(!isfinite(s->values[i])){fail(e,n,"analytics values must be finite");return -1;}return 0;}
static int ready(const sleela_data_analytics_set_t*s,double*out){return !out||sleela_data_analytics_validate(s,0,0)!=0?-1:0;}
int sleela_data_analytics_sum(const sleela_data_analytics_set_t*s,double*out){if(ready(s,out))return -1;double r=0;for(size_t i=0;i<s->count;i++)r+=s->values[i];*out=r;return isfinite(r)?0:-1;}
int sleela_data_analytics_mean(const sleela_data_analytics_set_t*s,double*out){if(ready(s,out))return -1;double r;if(sleela_data_analytics_sum(s,&r))return -1;*out=r/(double)s->count;return 0;}
int sleela_data_analytics_min(const sleela_data_analytics_set_t*s,double*out){if(ready(s,out))return -1;double r=s->values[0];for(size_t i=1;i<s->count;i++)if(s->values[i]<r)r=s->values[i];*out=r;return 0;}
int sleela_data_analytics_max(const sleela_data_analytics_set_t*s,double*out){if(ready(s,out))return -1;double r=s->values[0];for(size_t i=1;i<s->count;i++)if(s->values[i]>r)r=s->values[i];*out=r;return 0;}
int sleela_data_analytics_variance(const sleela_data_analytics_set_t*s,double*out){if(ready(s,out))return -1;double m;if(sleela_data_analytics_mean(s,&m))return -1;double r=0;for(size_t i=0;i<s->count;i++){double d=s->values[i]-m;r+=d*d;}*out=r/(double)s->count;return isfinite(*out)?0:-1;}
int sleela_data_analytics_stddev(const sleela_data_analytics_set_t*s,double*out){if(ready(s,out))return -1;double v;if(sleela_data_analytics_variance(s,&v))return -1;*out=sqrt(v);return isfinite(*out)?0:-1;}
int sleela_data_analytics_correlation(const sleela_data_analytics_set_t*a,const sleela_data_analytics_set_t*b,double*out){if(!out||sleela_data_analytics_validate(a,0,0)||sleela_data_analytics_validate(b,0,0)||a->count!=b->count||a->count<2)return -1;double am,bm;if(sleela_data_analytics_mean(a,&am)||sleela_data_analytics_mean(b,&bm))return -1;double ab=0,aa=0,bb=0;for(size_t i=0;i<a->count;i++){double x=a->values[i]-am,y=b->values[i]-bm;ab+=x*y;aa+=x*x;bb+=y*y;}if(aa==0||bb==0)return -1;*out=ab/sqrt(aa*bb);return isfinite(*out)?0:-1;}
