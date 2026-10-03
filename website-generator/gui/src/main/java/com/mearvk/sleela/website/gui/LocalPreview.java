package com.mearvk.sleela.website.gui;

/**
 * Offline fallback renderer used only until the SLeeLa connector transport is
 * wired into {@link WebsiteDesignModel#generate()}.
 *
 * <p>This intentionally mirrors the authoritative SLeeLa emitter
 * ({@code website/emitter.sleela}) at a high level so the JavaFX preview is
 * faithful when running without a live SLeeLa runtime. It is <em>not</em> the
 * source of truth: once the connector is active, generation returns the
 * generator's HTML and this class is bypassed.</p>
 */
final class LocalPreview {
    private LocalPreview() {}

    static String render(WebsiteDesignModel m) {
        int motion = m.energy() >= 75 ? 2 : (m.energy() >= 35 ? 1 : 0);
        String heroBg = switch (motion) {
            case 2 -> "linear-gradient(120deg,var(--brand),var(--accent),var(--brand))";
            case 1 -> "linear-gradient(135deg,var(--brand),var(--accent))";
            default -> "var(--brand)";
        };

        StringBuilder css = new StringBuilder();
        css.append("<style>:root{--brand:").append(m.brand())
           .append(";--base:").append(m.base())
           .append(";--accent:").append(m.accent()).append(";}");
        css.append("*{box-sizing:border-box;margin:0;padding:0}");
        css.append("body{font-family:system-ui,sans-serif;background:var(--base);color:#e5e7eb;line-height:1.55}");
        css.append("section{padding:72px 24px}.wrap{max-width:1080px;margin:0 auto}");
        css.append("nav{position:sticky;top:0;display:flex;justify-content:space-between;align-items:center;padding:16px 24px;background:rgba(0,0,0,.35)}");
        css.append("nav .brand{font-weight:800;color:#fff}");
        css.append(".hero{background:").append(heroBg).append(";padding:120px 32px;text-align:center;color:#fff;border-radius:14px}");
        css.append(".hero h1{font-size:clamp(2.2rem,6vw,4rem)}.hero p{opacity:.9;margin:16px auto 28px;max-width:46ch}");
        css.append(".btn{display:inline-block;background:var(--accent);color:#001;font-weight:700;padding:14px 26px;border-radius:12px;text-decoration:none}");
        css.append(".grid{display:grid;gap:20px;grid-template-columns:repeat(auto-fit,minmax(220px,1fr))}");
        css.append(".card{background:rgba(255,255,255,.06);border:1px solid rgba(255,255,255,.08);border-radius:12px;padding:24px}");
        css.append(".cta{background:var(--accent);color:#001;text-align:center;border-radius:14px}");
        css.append("blockquote{font-size:1.5rem;max-width:40ch;margin:0 auto;text-align:center;color:#fff}");
        css.append("footer{padding:48px 24px;background:rgba(0,0,0,.4)}");
        if (motion == 2) {
            css.append(".hero{background-size:200% 200%;animation:flow 12s ease infinite}");
            css.append("@keyframes flow{0%{background-position:0% 50%}50%{background-position:100% 50%}100%{background-position:0% 50%}}");
        }
        css.append("</style>");

        StringBuilder body = new StringBuilder();
        for (Sign s : m.signs()) {
            body.append(renderSign(s));
        }

        return "<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\">"
                + "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
                + "<title>Preview</title>" + css + "</head><body>" + body + "</body></html>";
    }

    private static String renderSign(Sign s) {
        String a = esc(s.a());
        String b = esc(s.b());
        String c = esc(s.c());
        return switch (s.kind()) {
            case NAV -> "<nav><span class=\"brand\">" + a + "</span><span>" + b + "</span></nav>";
            case HERO -> "<section class=\"hero\"><div class=\"wrap\"><h1>" + a + "</h1><p>" + b
                    + "</p><a class=\"btn\" href=\"#\">" + c + "</a></div></section>";
            case FEATURE, GALLERY, PRICING -> "<section><div class=\"wrap\"><h2>" + a
                    + "</h2><div class=\"grid\">" + cards(s.b()) + "</div></div></section>";
            case TESTIMONIAL -> "<section><blockquote>\u201c" + a + "\u201d<br><small>\u2014 " + b
                    + "</small></blockquote></section>";
            case CTA -> "<section class=\"cta\"><div class=\"wrap\"><h2>" + a
                    + "</h2><a class=\"btn\" href=\"" + c + "\">" + b + "</a></div></section>";
            case RICHTEXT -> "<section><div class=\"wrap\">" + s.a() + "</div></section>";
            case FOOTER -> "<footer><div class=\"wrap\"><strong>" + a + "</strong><div>" + b + "</div></footer>";
        };
    }

    private static String cards(String items) {
        if (items == null || items.isBlank()) { return ""; }
        StringBuilder sb = new StringBuilder();
        for (String item : items.split(";")) {
            sb.append("<div class=\"card\"><h3>").append(esc(item.trim())).append("</h3></div>");
        }
        return sb.toString();
    }

    private static String esc(String v) {
        if (v == null) { return ""; }
        return v.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
    }
}
