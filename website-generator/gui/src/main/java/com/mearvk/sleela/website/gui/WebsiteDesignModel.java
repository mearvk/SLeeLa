package com.mearvk.sleela.website.gui;

import java.util.ArrayList;
import java.util.List;

/**
 * Java-side presentation model for the SLeeLa Website Generator.
 *
 * The model holds the design intent (preset + the "energy" dial + palette) and
 * the ordered list of signs. It does <em>not</em> generate markup: when the
 * user asks to generate, {@link #generate()} hands the design and signs to the
 * SLeeLa generator through the common Java connector and returns whatever the
 * generator produced.
 *
 * <p><strong>JavaFX presents; SLeeLa decides.</strong></p>
 *
 * <p>The live connector transport (process / RMI / HTTP via
 * {@code com.mearvk.sleela.connector.SleelaJavaConnector}) is the next
 * integration point. Until it is wired, {@link #generate()} returns a faithful
 * local preview built from the same design/sign inputs so the authoring surface
 * is fully usable; that fallback is clearly marked below.</p>
 */
public final class WebsiteDesignModel {

    /** Named design presets, mirroring design.sleela. */
    public enum Preset { AURORA, NEON, BRUTALIST, EDITORIAL }

    private Preset preset = Preset.NEON;
    private int energy = 95;            // the "exciting" dial, 0..100
    private String brand = "#7c3aed";
    private String base = "#0b0714";
    private String accent = "#22d3ee";
    private final List<Sign> signs = new ArrayList<>();

    public WebsiteDesignModel() {
        // A sensible starting page so the Studio opens on something real.
        signs.add(new Sign(Sign.Kind.NAV, "Lumen", "Features  Pricing  Contact", ""));
        signs.add(new Sign(Sign.Kind.HERO,
                "Launch something unforgettable",
                "A website generated from design intent and semantic signs.",
                "Start building"));
        signs.add(new Sign(Sign.Kind.FEATURE, "Why it works",
                "Design-driven; Sign-based; Zero dependencies", ""));
        signs.add(new Sign(Sign.Kind.CTA, "Ready to generate your site?", "Open the Studio", "#studio"));
        signs.add(new Sign(Sign.Kind.FOOTER, "Lumen", "Made with the SLeeLa Website Generator", ""));
    }

    public Preset preset() { return preset; }
    public int energy() { return energy; }
    public String brand() { return brand; }
    public String base() { return base; }
    public String accent() { return accent; }
    public List<Sign> signs() { return List.copyOf(signs); }

    public void setPreset(Preset p) { preset = p; applyPresetPalette(p); }
    public void setEnergy(int e) { energy = Math.max(0, Math.min(100, e)); }
    public void setBrand(String hex) { brand = hex; }
    public void setBase(String hex) { base = hex; }
    public void setAccent(String hex) { accent = hex; }

    public void addSign(Sign s) { signs.add(s); }
    public void removeSign(int index) {
        if (index >= 0 && index < signs.size()) { signs.remove(index); }
    }
    public void moveSign(int index, int delta) {
        int target = index + delta;
        if (index < 0 || index >= signs.size() || target < 0 || target >= signs.size()) { return; }
        Sign s = signs.remove(index);
        signs.add(target, s);
    }

    public String summary() {
        return String.format("%s \u2022 energy %d \u2022 %d signs \u2022 brand %s / accent %s",
                preset.name().toLowerCase(), energy, signs.size(), brand, accent);
    }

    private void applyPresetPalette(Preset p) {
        switch (p) {
            case NEON -> { brand = "#7c3aed"; base = "#0b0714"; accent = "#22d3ee"; energy = 95; }
            case BRUTALIST -> { brand = "#111111"; base = "#f5f5f5"; accent = "#ff3b30"; energy = 70; }
            case EDITORIAL -> { brand = "#1f2937"; base = "#fbfbf8"; accent = "#b45309"; energy = 15; }
            case AURORA -> { brand = "#2563eb"; base = "#0f172a"; accent = "#38bdf8"; energy = 60; }
        }
    }

    /**
     * Generate the site HTML.
     *
     * <p>Authoritative path (next integration point): submit the design + signs
     * to the SLeeLa generator through {@code SleelaJavaConnector}, e.g.
     * {@code connector.invoke("website.emit", encode(design, signs))}, and
     * return the generator's HTML verbatim. The GUI must not substitute its own
     * markup for the generator's.</p>
     *
     * <p>Fallback path (used until the connector is wired): produce a faithful
     * local preview from the identical inputs so the Studio remains usable
     * offline. This mirrors — but never replaces — the SLeeLa emitter.</p>
     */
    public String generate() {
        // --- SLeeLa integration point -------------------------------------
        // try (SleelaJavaConnector c = SleelaConnectors.open()) {
        //     return c.invoke("website.emit", encode()).text();
        // }
        // ------------------------------------------------------------------
        return LocalPreview.render(this);
    }
}
