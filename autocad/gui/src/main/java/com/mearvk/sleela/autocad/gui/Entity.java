package com.mearvk.sleela.autocad.gui;

/**
 * A lightweight geometry primitive the GUI draws in its on-screen preview.
 *
 * The GUI does not compute geometry — it receives a flat list of these from the
 * SLeeLa renderer (lines for walls/dimensions, text for notes/measurements) and
 * paints them. The fields mirror the DXF entities the renderer emits.
 */
public sealed interface Entity permits Entity.Line, Entity.Text {

    /** Layer the entity belongs to, matching the renderer's layers. */
    String layer();

    /** A straight segment (wall or dimension line). */
    record Line(String layer, double x1, double y1, double x2, double y2) implements Entity {}

    /** A text label (note, title, or measured dimension value). */
    record Text(String layer, double x, double y, double height, String value) implements Entity {}
}
