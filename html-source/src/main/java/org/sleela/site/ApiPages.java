package org.sleela.site;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/** Re-skins the generated SleelaDoc API pages: keeps the body, drops the old chrome, rewrites links to site routes. */
public final class ApiPages {
    private ApiPages() {}

    private static final Pattern HREF = Pattern.compile("href=\"([^\"]*)\"");
    private static final Pattern SCRIPT = Pattern.compile("(?is)<script\\b.*?</script>");
    private static final Pattern H1 = Pattern.compile("(?is)<h1[^>]*>(.*?)</h1>");
    private static final Pattern PKG = Pattern.compile("^(?:\\.\\./)?package/([^/#?]+)\\.html(#.*)?$");
    private static final Pattern TOP = Pattern.compile("^(?:\\.\\./)?(index|packages|glossary|authorship)\\.html(#.*)?$");

    public static String title(String html) {
        Matcher m = H1.matcher(html);
        return m.find() ? m.group(1).replaceAll("<[^>]*>", "").trim() : "API";
    }

    public static String body(String html, String ctx) {
        int a = html.indexOf("<main");
        int b = html.lastIndexOf("</main>");
        if (a < 0 || b < 0) return "";
        int open = html.indexOf('>', a);
        String inner = html.substring(open + 1, b);
        inner = SCRIPT.matcher(inner).replaceAll("");
        inner = inner.replace(" oninput=\"flt(this.value)\"", " data-filter=\"u\"")
                     .replace(" oninput=\"fltp(this.value)\"", " data-filter=\"p\"");
        Matcher m = HREF.matcher(inner);
        StringBuilder sb = new StringBuilder();
        while (m.find()) m.appendReplacement(sb, Matcher.quoteReplacement("href=\"" + map(m.group(1), ctx) + "\""));
        m.appendTail(sb);
        return sb.toString();
    }

    static String map(String href, String ctx) {
        String h = href.replace("&amp;", "&");
        Matcher pm = PKG.matcher(h);
        if (pm.matches()) return ctx + "/api/package/" + Routes.seg(pm.group(1)) + (pm.group(2) == null ? "" : pm.group(2));
        Matcher tm = TOP.matcher(h);
        if (tm.matches()) {
            String page = tm.group(1);
            return ctx + (page.equals("index") ? "/api" : "/api/" + page) + (tm.group(2) == null ? "" : tm.group(2));
        }
        if (h.startsWith("../../")) {
            String rest = h.substring(6);
            String frag = "";
            int i = rest.indexOf('#');
            if (i >= 0) { frag = rest.substring(i); rest = rest.substring(0, i); }
            if (rest.endsWith("/")) rest = rest.substring(0, rest.length() - 1);
            return ctx + Routes.mapRepoPath(rest) + frag;
        }
        return href;
    }
}
