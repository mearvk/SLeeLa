// ===========================================================================
// financial.h -- Sleela financial mathematics integration library.
// ===========================================================================
#ifndef SLEELA_FINANCIAL_H
#define SLEELA_FINANCIAL_H

namespace sleela { namespace financial {

double futureValue(double principal, double rate, double periods);
double presentValue(double future, double rate, double periods);
double annuityPresent(double payment, double rate, double periods);
double annuityFuture(double payment, double rate, double periods);
double npv(double rate, double cashflow1, double cashflow2, double cashflow3, double cashflow4);
double bondPrice(double face, double couponRate, double yield, double periods);
double bondDuration(double face, double couponRate, double yield, double periods);
double capm(double riskFree, double beta, double marketReturn);
double wacc(double equityWeight, double debtWeight, double costEquity, double costDebt, double taxRate);
double determinant2(double a, double b, double c, double d);
double linearSolve2X2X(double a, double b, double c, double d, double e, double f);
double linearSolve2X2Y(double a, double b, double c, double d, double e, double f);
double quadraticDiscriminant(double a, double b, double c);
double quadraticRootPlus(double a, double b, double c);
double quadraticRootMinus(double a, double b, double c);
double ratio(double numerator, double denominator);

}} // namespace sleela::financial
#endif
