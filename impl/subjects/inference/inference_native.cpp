// inference_native.cpp -- Native Inference subject library.
//
// Synthesizes the __NativeInference class: array-free statistical inference
// over a data series, expressed as ordinary Sleela Core methods. A Sleela
// program loops once over its series accumulating running sums
//   n, sumX, sumY, sumXX, sumYY, sumXY
// (X is typically the time index, Y the observed national/economic quantity)
// and then calls these functions to obtain descriptive statistics, an ordinary
// least-squares trend, correlation / R^2, a z-score, a forecast, and a CAGR.
//
// The classification helpers (trendCode / strengthCode / significanceCode)
// return small integer codes; the Sleela layer maps those to plain-language
// words to produce the paragraph-form evaluation, keeping the numeric model and
// its verbal interpretation cleanly separated.
//
// Inference depends on Math (sqrt, pow, abs) and therefore requires
// `import math` alongside `import inference`. Every result is a computation over
// the supplied data and stated assumptions -- a descriptive statistic, not a
// causal claim or an official forecast.
#include "native_add.h"
#include "native_builders.h"

namespace sleela { namespace native {
using namespace sleela::native::builders;

void addInference(Program& p) {
    ClassDecl c; c.name = "__NativeInference";
    Method m;

    // mean(sum, n) = sum / n
    m = M2("double", "__native_inference_mean", "double", "sum", "double", "n");
    m.body->stmts.push_back(Ret(Bn("/", V("sum"), V("n"))));
    c.methods.push_back(std::move(m));

    // variance(sumSq, sum, n) = sumSq/n - mean^2   (population variance)
    m = M3("double", "__native_inference_variance", "double", "sumSq", "double", "sum", "double", "n");
    m.body->stmts.push_back(Decl("double", "mu", Bn("/", V("sum"), V("n"))));
    m.body->stmts.push_back(Ret(Bn("-", Bn("/", V("sumSq"), V("n")), Bn("*", V("mu"), V("mu")))));
    c.methods.push_back(std::move(m));

    // stdev(sumSq, sum, n) = sqrt(variance)
    m = M3("double", "__native_inference_stdev", "double", "sumSq", "double", "sum", "double", "n");
    m.body->stmts.push_back(Ret(C1("__native_math_sqrt",
        C3("__native_inference_variance", V("sumSq"), V("sum"), V("n")))));
    c.methods.push_back(std::move(m));

    // slope(n, sumX, sumY, sumXX, sumXY) = (n*Sxy - Sx*Sy) / (n*Sxx - Sx^2)
    m = M5("double", "__native_inference_slope",
           "double", "n", "double", "sumX", "double", "sumY", "double", "sumXX", "double", "sumXY");
    m.body->stmts.push_back(Decl("double", "den",
        Bn("-", Bn("*", V("n"), V("sumXX")), Bn("*", V("sumX"), V("sumX")))));
    {
        auto i = std::make_unique<IfStmt>();
        i->cond = Bn("==", V("den"), D(0));
        i->thenS = Ret(D(0));
        m.body->stmts.push_back(std::move(i));
    }
    m.body->stmts.push_back(Ret(Bn("/",
        Bn("-", Bn("*", V("n"), V("sumXY")), Bn("*", V("sumX"), V("sumY"))), V("den"))));
    c.methods.push_back(std::move(m));

    // intercept(n, sumX, sumY, slope) = (sumY - slope*sumX) / n
    m = M4("double", "__native_inference_intercept",
           "double", "n", "double", "sumX", "double", "sumY", "double", "slope");
    m.body->stmts.push_back(Ret(Bn("/",
        Bn("-", V("sumY"), Bn("*", V("slope"), V("sumX"))), V("n"))));
    c.methods.push_back(std::move(m));

    // correlation(n, sumX, sumY, sumXX, sumYY, sumXY) = Pearson r
    m = M6("double", "__native_inference_correlation",
           "double", "n", "double", "sumX", "double", "sumY",
           "double", "sumXX", "double", "sumYY", "double", "sumXY");
    m.body->stmts.push_back(Decl("double", "cov",
        Bn("-", Bn("*", V("n"), V("sumXY")), Bn("*", V("sumX"), V("sumY")))));
    m.body->stmts.push_back(Decl("double", "vx",
        Bn("-", Bn("*", V("n"), V("sumXX")), Bn("*", V("sumX"), V("sumX")))));
    m.body->stmts.push_back(Decl("double", "vy",
        Bn("-", Bn("*", V("n"), V("sumYY")), Bn("*", V("sumY"), V("sumY")))));
    m.body->stmts.push_back(Decl("double", "den",
        C1("__native_math_sqrt", Bn("*", V("vx"), V("vy")))));
    {
        auto i = std::make_unique<IfStmt>();
        i->cond = Bn("==", V("den"), D(0));
        i->thenS = Ret(D(0));
        m.body->stmts.push_back(std::move(i));
    }
    m.body->stmts.push_back(Ret(Bn("/", V("cov"), V("den"))));
    c.methods.push_back(std::move(m));

    // rSquared(r) = r*r
    m = M1("double", "__native_inference_r_squared", "double", "r");
    m.body->stmts.push_back(Ret(Bn("*", V("r"), V("r"))));
    c.methods.push_back(std::move(m));

    // zscore(x, mean, stdev) = (x - mean) / stdev  (0 if stdev==0)
    m = M3("double", "__native_inference_zscore", "double", "x", "double", "mean", "double", "stdev");
    {
        auto i = std::make_unique<IfStmt>();
        i->cond = Bn("==", V("stdev"), D(0));
        i->thenS = Ret(D(0));
        m.body->stmts.push_back(std::move(i));
    }
    m.body->stmts.push_back(Ret(Bn("/", Bn("-", V("x"), V("mean")), V("stdev"))));
    c.methods.push_back(std::move(m));

    // forecast(intercept, slope, x) = intercept + slope*x
    m = M3("double", "__native_inference_forecast", "double", "intercept", "double", "slope", "double", "x");
    m.body->stmts.push_back(Ret(Bn("+", V("intercept"), Bn("*", V("slope"), V("x")))));
    c.methods.push_back(std::move(m));

    // cagr(first, last, periods) = (last/first)^(1/periods) - 1   (0 on bad input)
    m = M3("double", "__native_inference_cagr", "double", "first", "double", "last", "double", "periods");
    {
        auto i = std::make_unique<IfStmt>();
        i->cond = Bn("||", Bn("<=", V("first"), D(0)), Bn("<=", V("periods"), D(0)));
        i->thenS = Ret(D(0));
        m.body->stmts.push_back(std::move(i));
    }
    m.body->stmts.push_back(Ret(Bn("-",
        C2("__native_math_pow", Bn("/", V("last"), V("first")), Bn("/", D(1), V("periods"))), D(1))));
    c.methods.push_back(std::move(m));

    // ---- classification helpers (numeric codes -> words in the Sleela layer) --

    // trendCode(slope, meanY, eps): relative slope vs a small threshold.
    //   -1 declining, 0 flat, +1 rising. eps is a fraction of |meanY| per step.
    m = M3("int", "__native_inference_trend_code", "double", "slope", "double", "meanY", "double", "eps");
    m.body->stmts.push_back(Decl("double", "thresh",
        Bn("*", V("eps"), C1("__native_math_abs", V("meanY")))));
    {
        auto rising = std::make_unique<IfStmt>();
        rising->cond = Bn(">", V("slope"), V("thresh"));
        rising->thenS = Ret(I(1));
        auto falling = std::make_unique<IfStmt>();
        falling->cond = Bn("<", V("slope"), Neg(V("thresh")));
        falling->thenS = Ret(I(-1));
        falling->elseS = Ret(I(0));
        rising->elseS = std::move(falling);
        m.body->stmts.push_back(std::move(rising));
    }
    c.methods.push_back(std::move(m));

    // strengthCode(absR): 0 none(<0.2) 1 weak(<0.4) 2 moderate(<0.7) 3 strong
    m = M1("int", "__native_inference_strength_code", "double", "absR");
    {
        auto s0 = std::make_unique<IfStmt>();
        s0->cond = Bn("<", V("absR"), D(0.2));
        s0->thenS = Ret(I(0));
        auto s1 = std::make_unique<IfStmt>();
        s1->cond = Bn("<", V("absR"), D(0.4));
        s1->thenS = Ret(I(1));
        auto s2 = std::make_unique<IfStmt>();
        s2->cond = Bn("<", V("absR"), D(0.7));
        s2->thenS = Ret(I(2));
        s2->elseS = Ret(I(3));
        s1->elseS = std::move(s2);
        s0->elseS = std::move(s1);
        m.body->stmts.push_back(std::move(s0));
    }
    c.methods.push_back(std::move(m));

    // significanceCode(rSquared): a coarse confidence bucket for the paragraph.
    //   0 low(<0.25) 1 modest(<0.5) 2 substantial(<0.8) 3 high
    m = M1("int", "__native_inference_significance_code", "double", "rSquared");
    {
        auto s0 = std::make_unique<IfStmt>();
        s0->cond = Bn("<", V("rSquared"), D(0.25));
        s0->thenS = Ret(I(0));
        auto s1 = std::make_unique<IfStmt>();
        s1->cond = Bn("<", V("rSquared"), D(0.5));
        s1->thenS = Ret(I(1));
        auto s2 = std::make_unique<IfStmt>();
        s2->cond = Bn("<", V("rSquared"), D(0.8));
        s2->thenS = Ret(I(2));
        s2->elseS = Ret(I(3));
        s1->elseS = std::move(s2);
        s0->elseS = std::move(s1);
        m.body->stmts.push_back(std::move(s0));
    }
    c.methods.push_back(std::move(m));

    p.classes.push_back(std::move(c));
}

}} // namespace sleela::native
