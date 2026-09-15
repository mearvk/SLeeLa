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
 * set's magnitude — here, a Long's length and a relevance's Pearson r (bounded
 * to [-1, 1] by construction). Failing a limit records that a value falls
 * outside a bound; it is a modelling condition, not a claim about reality.
 * ASSOCIATION != CAUSATION.
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

    /** Demonstration: the Constitution applied to /1 Longs. */
    public static void main(String[] args) {
        System.out.println("=== /1 constraints (Java) -- the Constitution applied to Longs ===");
        System.out.println("System invariants: DEPTH_LIMIT=" + DEPTH_LIMIT
                + ", DEGREE_MAX=" + DEGREE_MAX + ".");

        System.out.println("[limits] length readings are positive magnitudes");
        checkLong("Nominal GDP 2024 ($T)", 29.2, 0.0, 1000.0);
        checkLong("Total federal debt 2024 ($T)", 35.5, 0.0, 1000.0);

        System.out.println("[invariant] a relevance r must hold within [-1, 1]");
        checkLong("relevance r (GDP~DEBT, illustrative)", 0.98, -1.0, 1.0);
        checkLong("out-of-range r (illustrative violation)", 1.4, -1.0, 1.0);

        System.out.println("[invariant] the connect rule (n<=3024, degree<=4)");
        System.out.println("  connect(3024, 4) permitted : " + connectPermitted(3024, 4));
        System.out.println("  connect(3025, 4) permitted : " + connectPermitted(3025, 4));
        System.out.println("  connect(100, 5) permitted  : " + connectPermitted(100, 5));

        System.out.println("Note: a constraint is a modelling condition to be checked "
                + "and argued with, not a claim about reality. ASSOCIATION != CAUSATION.");
    }

    private static void checkLong(String name, double value, double lo, double hi) {
        System.out.println("  Bound[" + lo + ", " + hi + "] on " + name
                + " (length=" + value + "): " + (withinBound(value, lo, hi) ? "HOLDS" : "VIOLATES"));
    }
}
