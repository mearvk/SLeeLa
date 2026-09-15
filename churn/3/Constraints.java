package com.mearvk.sleela.churn;

/**
 * The Constitution's congrains / limits applied to a churn relevance set.
 *
 * <p>Shared constraint/theory layer carried by every churn set (/1 Longs,
 * /2 Structures, /3 Facets, /4 Areas). It encodes the governing constraints
 * from the repository's Constitution (top-level README, "The Constitution",
 * and SHEET.sheet's {@code congrains} / {@code limits} / {@code system}
 * sections) as a small, dependency-free Java class, so the same theory can be
 * checked from the JVM side as from Sleela.
 *
 * <p>System invariants always in force: {@code DEPTH_LIMIT} (&le; 3024) and
 * {@code DEGREE_MAX} (&le; 4). A limit (Bound / Threshold / Capacity) checks a
 * set's magnitude — here, a Facet's value. Failing a limit records that a value
 * falls outside a bound; it is a modelling condition, not a claim about reality.
 */
public final class Constraints {

    public static final int DEPTH_LIMIT = 3024;
    public static final int DEGREE_MAX = 4;

    public static boolean connectPermitted(int n, int degree) {
        return n >= 0 && n <= DEPTH_LIMIT && degree <= DEGREE_MAX;
    }

    public static boolean withinBound(double value, double lo, double hi) {
        return value >= lo && value <= hi;
    }

    public static boolean belowThreshold(double value, double threshold) {
        return value < threshold;
    }

    public static boolean withinCapacity(double used, double capacity) {
        return capacity >= 0.0 && used <= capacity;
    }

    private Constraints() {
    }

    /** Demonstration: the Constitution applied to /3 Facets. */
    public static void main(String[] args) {
        System.out.println("=== /3 constraints (Java) -- the Constitution applied to Facets ===");
        System.out.println("System invariants: DEPTH_LIMIT=" + DEPTH_LIMIT
                + ", DEGREE_MAX=" + DEGREE_MAX + ".");

        System.out.println("[limits] labor-market rate Facets within [0, 100] percent");
        checkFacet("quits rate (US 2024)", 2.0, 0.0, 100.0);
        checkFacet("job openings rate (US 2024)", 4.2, 0.0, 100.0);

        System.out.println("[limits] fine-structure constant (a coupling) within [0, 1]");
        checkFacet("fine-structure constant alpha", 7.2973525643e-3, 0.0, 1.0);

        System.out.println("[invariant] the connect rule (n<=3024, degree<=4)");
        System.out.println("  connect(3024, 4) permitted : " + connectPermitted(3024, 4));
        System.out.println("  connect(3025, 4) permitted : " + connectPermitted(3025, 4));
        System.out.println("  connect(100, 5) permitted  : " + connectPermitted(100, 5));

        System.out.println("Note: a constraint is a modelling condition to be checked "
                + "and argued with, not a claim about reality.");
    }

    private static void checkFacet(String name, double value, double lo, double hi) {
        System.out.println("  Bound[" + lo + ", " + hi + "] on " + name
                + " (value=" + value + "): " + (withinBound(value, lo, hi) ? "HOLDS" : "VIOLATES"));
    }
}
