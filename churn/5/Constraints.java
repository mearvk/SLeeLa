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
 * <p>This is the shared constraint/theory layer carried by every churn set
 * (/1 Longs, /2 Structures, /3 Facets, /4 Areas, /5 Lines). It encodes the
 * governing constraints from the repository's Constitution (see the top-level
 * README, "The Constitution", and SHEET.sheet's {@code congrains} /
 * {@code limits} / {@code system} sections) as a small, dependency-free Java
 * class so the same theory can be checked from the JVM side as from Sleela.
 *
 * <p>Two system invariants are always in force:
 * <ul>
 *   <li>{@code DEPTH_LIMIT} — system depth &le; 3024</li>
 *   <li>{@code DEGREE_MAX} — connection order (complexity degree) &le; 4</li>
 * </ul>
 *
 * <p>A limit here (Bound / Threshold / Capacity) checks whether a set's
 * magnitude — a Long's length, a Structure's value, a Facet's value, an Area's
 * extent, or a <b>Line's value (a count or a length)</b> — honors a stated
 * constraint. Failing a limit records that a value falls outside a bound; it is
 * a modelling condition to be checked and argued with, not a claim about
 * reality.
 *
 * <p>Running {@code main} prints the report to stdout and also writes a
 * well-formed Markdown report to a local {@code DD-MM-YYYY/} folder (named for
 * the run date), as {@code churn-5-lines.md}.
 */
public final class Constraints {

    /** This set's identifier and human title, for the report header. */
    public static final String SET_ID = "5";
    public static final String SET_TITLE = "Lines";
    public static final String REPORT_FILE = "churn-5-lines.md";

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

    /** One evaluated bound check, shared by the console and Markdown outputs. */
    private record BoundCheck(String name, double value, double lo, double hi) {
        boolean holds() {
            return withinBound(value, lo, hi);
        }
        String verdict() {
            return holds() ? "HOLDS" : "VIOLATES";
        }
    }

    private Constraints() {
        // static utility; not instantiable
    }

    public static void main(String[] args) {
        // A count is a non-negative Line; each structural count honors a Bound.
        List<BoundCheck> bounds = new ArrayList<>();
        bounds.add(new BoundCheck("US states", 50.0, 0.0, 100.0));
        bounds.add(new BoundCheck("US senators", 100.0, 0.0, 100.0));
        bounds.add(new BoundCheck("negative tally", -3.0, 0.0, 1000.0));

        // Structural counts must sit within the DepthLimit Line: a count cannot
        // out-reach the system that holds it.
        double depth = DEPTH_LIMIT;
        double votingCongress = 435.0 + 100.0;
        boolean congressWithinDepth = withinCapacity(votingCongress, depth);

        // ---- stdout ----
        System.out.println("=== /" + SET_ID + " constraints (Java) -- the Constitution applied to "
                + SET_TITLE + " ===");
        System.out.println("System invariants: DEPTH_LIMIT=" + DEPTH_LIMIT
                + ", DEGREE_MAX=" + DEGREE_MAX + ".");
        System.out.println("[limits] counts are non-negative Lines within a stated Bound");
        for (BoundCheck b : bounds) {
            System.out.println("  Bound[" + b.lo() + ", " + b.hi() + "] on " + b.name()
                    + " (value=" + b.value() + "): " + b.verdict());
        }
        System.out.println("[capacity] US voting Congress (435 + 100) = " + votingCongress
                + " : " + (congressWithinDepth ? "WITHIN" : "OVER") + " the DepthLimit (" + DEPTH_LIMIT + ")");
        System.out.println("[invariant] the connect rule (n<=3024, degree<=4)");
        System.out.println("  connect(3024, 4) permitted : " + connectPermitted(3024, 4));
        System.out.println("  connect(3025, 4) permitted : " + connectPermitted(3025, 4));
        System.out.println("  connect(100, 5) permitted  : " + connectPermitted(100, 5));

        // ---- well-formed Markdown report into a DD-MM-YYYY/ folder ----
        String capacityNote = "US voting Congress (435 House + 100 Senate) = " + fmt(votingCongress)
                + " counts — " + (congressWithinDepth ? "WITHIN" : "OVER")
                + " the DepthLimit Line (" + fmt(depth) + ")";
        try {
            Path out = writeReport(bounds, capacityNote);
            System.out.println("wrote report: " + out);
        } catch (IOException e) {
            System.err.println("error: could not write Markdown report: " + e.getMessage());
        }
    }

    /**
     * Build the well-formed Markdown report and write it to {@code DD-MM-YYYY/REPORT_FILE},
     * returning the path written. The date folder is the run date (local).
     */
    static Path writeReport(List<BoundCheck> bounds, String capacityNote) throws IOException {
        LocalDate today = LocalDate.now();
        String folder = today.format(DateTimeFormatter.ofPattern("dd-MM-yyyy"));
        Path dir = Path.of(folder);
        Files.createDirectories(dir);
        Path file = dir.resolve(REPORT_FILE);
        Files.writeString(file, buildMarkdown(bounds, capacityNote, today), StandardCharsets.UTF_8);
        return file;
    }

    /** Assemble a well-formed Markdown document for this run. */
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
        md.append("A count is a non-negative **Line**; each must honor its stated Bound.\n\n");
        md.append("| Subject | Value | Bound | Verdict |\n");
        md.append("|---|---:|---|:--:|\n");
        for (BoundCheck b : bounds) {
            md.append("| ").append(b.name())
              .append(" | ").append(fmt(b.value()))
              .append(" | [").append(fmt(b.lo())).append(", ").append(fmt(b.hi())).append("]")
              .append(" | ").append(b.verdict()).append(" |\n");
        }
        md.append('\n');

        md.append("## Capacity\n\n");
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

    /** Format a double without scientific notation or a trailing ".0" for integers. */
    private static String fmt(double v) {
        if (v == Math.floor(v) && !Double.isInfinite(v)) {
            return String.format("%,d", (long) v);
        }
        return String.valueOf(v);
    }
}
