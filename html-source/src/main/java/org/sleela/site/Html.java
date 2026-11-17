package org.sleela.site;

public final class Html {
    private Html() {}

    public static String esc(String s) {
        if (s == null) return "";
        StringBuilder sb = new StringBuilder(s.length() + 16);
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            switch (c) {
                case '&': sb.append("&amp;"); break;
                case '<': sb.append("&lt;"); break;
                case '>': sb.append("&gt;"); break;
                case '"': sb.append("&quot;"); break;
                case '\'': sb.append("&#39;"); break;
                default: sb.append(c);
            }
        }
        return sb.toString();
    }

    public static String slug(String s) {
        String t = s.toLowerCase(java.util.Locale.ROOT).replaceAll("<[^>]*>", "").replaceAll("[^a-z0-9]+", "-");
        t = t.replaceAll("^-+|-+$", "");
        return t.isEmpty() ? "section" : t;
    }
}
