/* Scientific pattern: bisection for f(x) = x*x - 2 on [1, 2].
 * Build: cc -std=c11 -Wall -Wextra -Wpedantic c_scientific_bisection.c -lm -o c_scientific_bisection
 */
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

static double f(double x) { return x * x - 2.0; }

static bool bisect(double lo, double hi, double tolerance, unsigned depth,
                   unsigned max_depth, double *root) {
    const double flo = f(lo), fhi = f(hi);
    if (root == NULL || !(tolerance > 0.0) || !(lo < hi) ||
        !isfinite(flo) || !isfinite(fhi) || flo * fhi > 0.0)
        return false;
    if (fabs(hi - lo) <= tolerance || depth >= max_depth) {
        *root = lo + (hi - lo) / 2.0;
        return true;
    }
    const double mid = lo + (hi - lo) / 2.0;
    const double fmid = f(mid);
    if (!isfinite(fmid)) return false;
    if (fmid == 0.0) { *root = mid; return true; }
    if (flo * fmid < 0.0)
        return bisect(lo, mid, tolerance, depth + 1U, max_depth, root);
    return bisect(mid, hi, tolerance, depth + 1U, max_depth, root);
}

int main(void) {
    double root = 0.0;
    if (!bisect(1.0, 2.0, 1e-12, 0U, 128U, &root)) {
        fputs("bisection failed: invalid bracket or numerical issue\n", stderr);
        return 1;
    }
    printf("sqrt(2) ~= %.15f; residual = %.3e\n", root, f(root));
    return 0;
}
