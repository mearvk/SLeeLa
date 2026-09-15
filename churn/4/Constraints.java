package com.mearvk.sleela.churn;

/**
 * The Constitution's congrains / limits applied to a churn relevance set.
 *
 * <p>This is the shared constraint/theory layer carried by every churn set
 * (/1 Longs, /2 Structures, /3 Facets, /4 Areas). It encodes the governing
 * constraints from the repository's Constitution (see the top-level README,
 * "The Constitution", and SHEET.sheet's {@code congrains} / {@code limits} /
 * {@code system} sections) as a small, dependency-free Java class so the same
 * theory can be checked from the JVM side as from Sleela.
 *
 * <p>Two system invariants are always in force:
 * <ul>
 *   <li>{@code DEPTH_LIMIT} — system depth &le; 3024</li>
 *   <li>{@code DEGREE_MAX} — connection order (complexity degree) &le; 4</li>
 * </ul>
 *
 * <p>A limit here (Bound / Threshold / Capacity) checks whether a set's
 * magnitude — a Long's length, a Structure's value, a Facet's value, or an
 * Area's extent — honors a stated constraint. Failing a limit records that a
 * value falls outside a bound; it is a modelling condition to be checked and
 * argued with, not a claim about reality.
 */
public final class Constraints {

    /** Article I — maximum system depth (SHEET.sheet system.depth). */
    public static final int DEPTH_LIMIT = 3024;

    /** Article I — maximum complexity degree (SHEET.sheet complexity-degree-max). */
    public static final int DEGREE_MAX = 4;

    /**
     * Article I — the connect rule: {@code connect(n)} is permitted iff
     * {@code 0 <= n <= DEPTH_LIMIT} and {@code degree <= DEGREE_MAX}.
     */
    public static boolean connectPermitted(int n, int degree) {
        return n >= 0 && n <= DEPTH_LIMIT && degree <= DEGREE_MAX;
    }

    /** Limit: Bound — an inclusive extent {@code [lo, hi]} on a value. */
    public static boolean withinBound(double value, double lo, double hi) {
        return value >= lo && value <= hi;
    }

    /** Limit: Threshold — a boundary value crossed when {@code value >= threshold}. */
    public static boolean belowThreshold(double value, double threshold) {
        return value < threshold;
    }

    /** Limit: Capacity — the maximum a container/resource may hold. */
    public static boolean withinCapacity(double used, double capacity) {
        return capacity >= 0.0 && used <= capacity;
    }

    private Constraints() {
        // static utility; not instantiable
    }

    /** Demonstration: the Constitution applied to /4 Areas. */
    public static void main(String[] args) {
        System.out.println("=== /4 constraints (Java) -- the Constitution applied to Areas ===");
        System.out.println("System invariants: DEPTH_LIMIT=" + DEPTH_LIMIT
                + ", DEGREE_MAX=" + DEGREE_MAX + ".");

        final double earthLand = 148_940_000.0; // km^2, the land Capacity

        System.out.println("[limits] geographic Areas must fit within Earth's land Capacity");
        checkArea("Russia", 17_098_242.0, 0.0, earthLand);
        checkArea("United States", 9_833_517.0, 0.0, earthLand);
        checkArea("hypothetical over-Earth region", 200_000_000.0, 0.0, earthLand);

        double combinedTop4 = 17_098_242.0 + 9_984_670.0 + 9_833_517.0 + 9_596_960.0;
        System.out.println("[capacity] top-4 countries combined = " + combinedTop4
                + " km^2 : " + (withinCapacity(combinedTop4, earthLand) ? "WITHIN" : "OVER")
                + " Earth land capacity");

        System.out.println("[invariant] the connect rule (n<=3024, degree<=4)");
        System.out.println("  connect(3024, 4) permitted : " + connectPermitted(3024, 4));
        System.out.println("  connect(3025, 4) permitted : " + connectPermitted(3025, 4));
        System.out.println("  connect(100, 5) permitted  : " + connectPermitted(100, 5));

        System.out.println("Note: a constraint is a modelling condition to be checked "
                + "and argued with, not a claim about reality.");
    }

    private static void checkArea(String name, double extent, double lo, double hi) {
        System.out.println("  Bound[" + lo + ", " + hi + "] on " + name
                + " (extent=" + extent + "): " + (withinBound(extent, lo, hi) ? "HOLDS" : "VIOLATES"));
    }
}
