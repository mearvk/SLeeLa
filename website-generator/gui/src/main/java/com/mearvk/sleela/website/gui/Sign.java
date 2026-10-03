package com.mearvk.sleela.website.gui;

/**
 * A semantic content block the user composes a page from.
 *
 * This mirrors the SLeeLa {@code Signs.Sign} struct exactly: a kind plus three
 * free-text slots. The GUI only collects these values; the SLeeLa generator
 * decides how a sign renders under the active design.
 */
public record Sign(Kind kind, String a, String b, String c) {

    /** Sign kinds, kept in lockstep with signs.sleela (same ordinal order). */
    public enum Kind {
        NAV("nav", "Brand", "Links", ""),
        HERO("hero", "Headline", "Subhead", "CTA label"),
        FEATURE("feature", "Title", "Items (semicolon-separated)", ""),
        GALLERY("gallery", "Title", "Images (semicolon-separated)", ""),
        PRICING("pricing", "Title", "Tiers", ""),
        TESTIMONIAL("testimonial", "Quote", "Author", ""),
        CTA("cta", "Headline", "Button label", "Target"),
        RICHTEXT("richtext", "HTML / prose", "", ""),
        FOOTER("footer", "Brand", "Columns", "");

        private final String id;
        private final String slotA;
        private final String slotB;
        private final String slotC;

        Kind(String id, String slotA, String slotB, String slotC) {
            this.id = id;
            this.slotA = slotA;
            this.slotB = slotB;
            this.slotC = slotC;
        }

        public String id() { return id; }
        public String slotA() { return slotA; }
        public String slotB() { return slotB; }
        public String slotC() { return slotC; }
    }

    /** A short, human-readable summary for list display. */
    public String label() {
        String head = a == null || a.isBlank() ? "(empty)" : a;
        return kind.id() + " \u2014 " + head;
    }
}
