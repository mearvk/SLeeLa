// ===========================================================================
// financial.cpp -- dependency-light financial mathematics reference layer.
// ===========================================================================
#include "financial.h"
#include <cmath>

namespace sleela { namespace financial {

double futureValue(double p,double r,double n){ return p*std::pow(1.0+r,n); }
double presentValue(double f,double r,double n){ return f/std::pow(1.0+r,n); }
double annuityPresent(double p,double r,double n){ if(r==0.0)return p*n; return p*(1.0-1.0/std::pow(1.0+r,n))/r; }
double annuityFuture(double p,double r,double n){ if(r==0.0)return p*n; return p*(std::pow(1.0+r,n)-1.0)/r; }
double npv(double r,double c1,double c2,double c3,double c4){ return c1/(1+r)+c2/std::pow(1+r,2)+c3/std::pow(1+r,3)+c4/std::pow(1+r,4); }
double bondPrice(double face,double coupon,double yield,double n){ double c=face*coupon; double s=0; for(int i=1;i<=static_cast<int>(n);++i)s+=c/std::pow(1+yield,i); return s+face/std::pow(1+yield,n); }
double bondDuration(double face,double coupon,double yield,double n){ double c=face*coupon,pv=0,w=0; for(int i=1;i<=static_cast<int>(n);++i){double x=c/std::pow(1+yield,i);pv+=x;w+=i*x;} double last=face/std::pow(1+yield,n);pv+=last;w+=n*last;return pv==0?0:w/pv; }
double capm(double rf,double beta,double rm){ return rf+beta*(rm-rf); }
double wacc(double ew,double dw,double ce,double cd,double tax){ return ew*ce+dw*cd*(1-tax); }
double determinant2(double a,double b,double c,double d){ return a*d-b*c; }
double linearSolve2X2X(double a,double b,double c,double d,double e,double f){double det=determinant2(a,b,c,d);return det==0?0:determinant2(e,b,f,d)/det;}
double linearSolve2X2Y(double a,double b,double c,double d,double e,double f){double det=determinant2(a,b,c,d);return det==0?0:determinant2(a,e,c,f)/det;}
double quadraticDiscriminant(double a,double b,double c){return b*b-4*a*c;}
double quadraticRootPlus(double a,double b,double c){double d=quadraticDiscriminant(a,b,c);return a==0?0:(-b+std::sqrt(d))/(2*a);}
double quadraticRootMinus(double a,double b,double c){double d=quadraticDiscriminant(a,b,c);return a==0?0:(-b-std::sqrt(d))/(2*a);}
double ratio(double n,double d){return d==0?0:n/d;}

}} // namespace sleela::financial
