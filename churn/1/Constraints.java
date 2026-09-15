package com.mearvk.sleela.churn;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;

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
 *
 * <p>Running {@code main} prints the report to stdout and also writes a
 * well-formed Markdown report to a local {@code DD-MM-YYYY/} folder (named for
 * the run date), as {@code churn-1-longs.md}.
 */
public final class Constraints {

    public static final String SET_ID = "1";
    public static final String SET_TITLE = "Longs";
    public static final String REPORT_FILE = "churn-1-longs.md";

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

    private record BoundCheck(String name, double value, double lo, double hi) {
        String verdict() {
            return withinBound(value, lo, hi) ? "HOLDS" : "VIOLATES";
        }
    }

    private Constraints() {
    }

    public static void main(String[] args) {
        List<BoundCheck> bounds = new ArrayList<>();
        bounds.add(new BoundCheck("Nominal GDP 2024 ($T)", 29.2, 0.0, 1000.0));
        bounds.add(new BoundCheck("Total federal debt 2024 ($T)", 35.5, 0.0, 1000.0));
        bounds.add(new BoundCheck("relevance r (GDP~DEBT, illustrative)", 0.98, -1.0, 1.0));
        bounds.add(new BoundCheck("out-of-range r (illustrative violation)", 1.4, -1.0, 1.0));

        System.out.println("=== /" + SET_ID + " constraints (Java) -- the Constitution applied to "
                + SET_TITLE + " ===");
        System.out.println("System invariants: DEPTH_LIMIT=" + DEPTH_LIMIT
                + ", DEGREE_MAX=" + DEGREE_MAX + ".");
        System.out.println("[limits] length readings and a relevance r must hold within bounds");
        for (BoundCheck b : bounds) {
            System.out.println("  Bound[" + b.lo() + ", " + b.hi() + "] on " + b.name()
                    + " (value=" + b.value() + "): " + b.verdict());
        }
        System.out.println("[invariant] the connect rule (n<=3024, degree<=4)");
        System.out.println("  connect(3024, 4) permitted : " + connectPermitted(3024, 4));
        System.out.println("  connect(3025, 4) permitted : " + connectPermitted(3025, 4));
        System.out.println("  connect(100, 5) permitted  : " + connectPermitted(100, 5));

        String capacityNote = "Long bounds: length readings are positive magnitudes; "
                + "a relevance r must hold within [-1, 1] (the Correlation invariant)";
        try {
            Path out = writeReport(bounds, capacityNote);
            System.out.println("wrote report: " + out);
        } catch (IOException e) {
            System.err.println("error: could not write Markdown report: " + e.getMessage());
        }
    }

    static Path writeReport(List<BoundCheck> bounds, String capacityNote) throws IOException {
        LocalDate today = LocalDate.now();
        String folder = today.format(DateTimeFormatter.ofPattern("dd-MM-yyyy"));
        Path dir = Path.of(folder);
        Files.createDirectories(dir);
        Path file = dir.resolve(REPORT_FILE);
        Files.writeString(file, buildMarkdown(bounds, capacityNote, today), StandardCharsets.UTF_8);
        return file;
    }

    static String buildMarkdown(List<BoundCheck> bounds, String capacityNote, LocalDate date) {
        StringBuilder md = new StringBuilder();
        md.append("# churn/").append(SET_ID).append(" — ").append(SET_TITLE)
          .append(": Constitution constraints report\n\n");
        md.append("_Generated ").append(date.format(DateTimeFormatter.ofPattern("dd-MM-yyyy")))
          .append(" by `com.mearvk.sleela.churn.Constraints` (churn/").append(SET_ID)
          .append(")._\n\n");

        md.append("## System invariants\n\n");
        md.append("| Invariant | Value |\n");
        md.append("|---|---|\n");
        md.append("| DepthLimit (max system depth) | ").append(DEPTH_LIMIT).append(" |\n");
        md.append("| ComplexityDegree (max) | ").append(DEGREE_MAX).append(" |\n\n");

        md.append("## Limits — Bound checks\n\n");
        md.append("Each **Long** length (and a relevance `r`) must hold within its bound.\n\n");
        md.append("| Subject | Value | Bound | Verdict |\n");
        md.append("|---|---:|---|:--:|\n");
        for (BoundCheck b : bounds) {
            md.append("| ").append(b.name())
              .append(" | ").append(fmt(b.value()))
              .append(" | [").append(fmt(b.lo())).append(", ").append(fmt(b.hi())).append("]")
              .append(" | ").append(b.verdict()).append(" |\n");
        }
        md.append('\n');

        md.append("## Notes on the bounds\n\n");
        md.append("- ").append(capacityNote).append("\n\n");

        md.append("## Invariant — the connect rule\n\n");
        md.append("`connect(n)` is permitted iff `0 <= n <= ").append(DEPTH_LIMIT)
          .append("` and `degree <= ").append(DEGREE_MAX).append("`.\n\n");
        md.append("| Call | Permitted |\n");
        md.append("|---|:--:|\n");
        md.append("| connect(3024, 4) | ").append(connectPermitted(3024, 4)).append(" |\n");
        md.append("| connect(3025, 4) | ").append(connectPermitted(3025, 4)).append(" |\n");
        md.append("| connect(100, 5) | ").append(connectPermitted(100, 5)).append(" |\n\n");

        md.append("---\n\n");
        md.append("_A constraint is a modelling condition to be checked and argued with, ")
          .append("not a claim about reality. `ASSOCIATION != CAUSATION`._\n");
        return md.toString();
    }

    private static String fmt(double v) {
        if (v == Math.floor(v) && !Double.isInfinite(v)) {
            return String.format("%,d", (long) v);
        }
        return String.valueOf(v);
    }
}
