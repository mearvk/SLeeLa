package com.mearvk.sleela.autocad.gui;

import java.util.ArrayList;
import java.util.List;

/**
 * Java-side presentation model for the SLeeLa AutoCAD Renderer.
 *
 * The model holds the plan text (description, dimensions, plan, notes). It does
 * <em>not</em> compute geometry or write DXF: {@link #render()} submits the plan
 * to the SLeeLa renderer through the common Java connector and returns the DXF,
 * and {@link #previewEntities()} asks the renderer for a flat entity list the
 * GUI can draw.
 *
 * <p><strong>JavaFX presents; SLeeLa decides.</strong></p>
 *
 * <p>The live connector transport (process / RMI / HTTP via
 * {@code SleelaJavaConnector}) is the next integration point. Until it is wired,
 * a faithful local fallback parses the same plan so the preview and DXF export
 * work offline; that fallback is clearly marked below and mirrors — but never
 * replaces — the SLeeLa pipeline (cad/plan.sleela + cad/dxf.sleela).</p>
 */
public final class CadPlanModel {

    private String planText;

    public CadPlanModel() { this.planText = DEFAULT_PLAN; }

    public String planText() { return planText; }
    public void setPlanText(String text) { planText = text == null ? "" : text; }

    /**
     * Render the plan to AutoCAD DXF.
     *
     * <p>Authoritative path (next integration point): submit the plan to the
     * SLeeLa renderer through {@code SleelaJavaConnector}, e.g.
     * {@code connector.invoke("autocad.render", planText)}, and return the DXF
     * verbatim.</p>
     *
     * <p>Fallback path (until the connector is wired): build DXF locally from
     * the parsed entities so export works offline.</p>
     */
    public String render() {
        // --- SLeeLa integration point -------------------------------------
        // try (SleelaJavaConnector c = SleelaConnectors.open()) {
        //     return c.invoke("autocad.render", planText).text();
        // }
        // ------------------------------------------------------------------
        return LocalRenderer.toDxf(previewEntities());
    }

    /**
     * The flat entity list used to draw the on-screen preview.
     *
     * <p>Authoritative path would ask the renderer
     * ({@code connector.invoke("autocad.entities", planText)}); the fallback
     * parses the plan locally with the same directive semantics as
     * cad/plan.sleela.</p>
     */
    public List<Entity> previewEntities() {
        return LocalRenderer.parse(planText);
    }

    public String summary() {
        int lines = 0;
        for (String line : planText.split("\n", -1)) {
            String t = line.trim();
            if (!t.isEmpty() && !t.startsWith("#")) { lines++; }
        }
        return lines + " directive(s) \u2022 " + previewEntities().size() + " entities";
    }

    private static final String DEFAULT_PLAN = String.join("\n",
            "title Office Floor",
            "units mm",
            "at 0 0",
            "room Lobby 6000 4000",
            "at 6000 0",
            "room Office 4000 4000",
            "door 6000 1500 900",
            "dim 0 -600 10000 -600",
            "dim -600 0 -600 4000",
            "note Fire exit on north wall",
            "note All dimensions in millimetres");

    // ------------------------------------------------------------------
    // Offline fallback: mirrors cad/plan.sleela + cad/dxf.sleela so the GUI is
    // usable without a live SLeeLa runtime. Bypassed once the connector is wired.
    // ------------------------------------------------------------------
    static final class LocalRenderer {
        private LocalRenderer() {}

        static List<Entity> parse(String plan) {
            List<Entity> out = new ArrayList<>();
            double curX = 0, curY = 0;
            double noteY = -1200;
            for (String raw : plan.split("\n", -1)) {
                String line = raw.trim();
                if (line.isEmpty() || line.startsWith("#")) { continue; }
                String[] t = line.split("\\s+");
                String op = t[0];
                switch (op) {
                    case "at" -> { curX = d(t, 1); curY = d(t, 2); }
                    case "room" -> {
                        // t: room <name> <w> <h>
                        double w = d(t, 2), h = d(t, 3);
                        rect(out, curX, curY, w, h);
                        out.add(new Entity.Text("notes", curX + w / 2, curY + h / 2, 150, token(t, 1)));
                    }
                    case "wall" -> out.add(new Entity.Line("walls", d(t, 1), d(t, 2), d(t, 3), d(t, 4)));
                    case "door" -> out.add(new Entity.Line("walls", d(t, 1), d(t, 2), d(t, 1) + d(t, 3), d(t, 2)));
                    case "dim" -> {
                        double x1 = d(t, 1), y1 = d(t, 2), x2 = d(t, 3), y2 = d(t, 4);
                        out.add(new Entity.Line("dims", x1, y1, x2, y2));
                        double dist = Math.hypot(x2 - x1, y2 - y1);
                        out.add(new Entity.Text("dims", (x1 + x2) / 2, (y1 + y2) / 2, 120,
                                String.valueOf(Math.round(dist))));
                    }
                    case "title" -> out.add(new Entity.Text("notes", 0, 5000, 150, rest(line)));
                    case "note" -> { out.add(new Entity.Text("notes", 0, noteY, 150, rest(line))); noteY -= 300; }
                    default -> { /* units + unknown: metadata only */ }
                }
            }
            return out;
        }

        private static void rect(List<Entity> out, double x, double y, double w, double h) {
            out.add(new Entity.Line("walls", x, y, x + w, y));
            out.add(new Entity.Line("walls", x + w, y, x + w, y + h));
            out.add(new Entity.Line("walls", x + w, y + h, x, y + h));
            out.add(new Entity.Line("walls", x, y + h, x, y));
        }

        static String toDxf(List<Entity> entities) {
            StringBuilder s = new StringBuilder();
            s.append("0\nSECTION\n2\nENTITIES\n");
            for (Entity e : entities) {
                if (e instanceof Entity.Line l) {
                    s.append("0\nLINE\n8\n").append(l.layer()).append("\n");
                    s.append("10\n").append(l.x1()).append("\n20\n").append(l.y1()).append("\n30\n0.0\n");
                    s.append("11\n").append(l.x2()).append("\n21\n").append(l.y2()).append("\n31\n0.0\n");
                } else if (e instanceof Entity.Text x) {
                    s.append("0\nTEXT\n8\n").append(x.layer()).append("\n");
                    s.append("10\n").append(x.x()).append("\n20\n").append(x.y()).append("\n30\n0.0\n");
                    s.append("40\n").append(x.height()).append("\n1\n").append(x.value()).append("\n");
                }
            }
            s.append("0\nENDSEC\n0\nEOF\n");
            return s.toString();
        }

        private static double d(String[] t, int i) {
            try { return i < t.length ? Double.parseDouble(t[i]) : 0.0; }
            catch (NumberFormatException ex) { return 0.0; }
        }

        private static String token(String[] t, int i) { return i < t.length ? t[i] : ""; }

        private static String rest(String line) {
            int sp = line.indexOf(' ');
            return sp < 0 ? "" : line.substring(sp + 1).trim();
        }
    }
}
