package org.sleela.site;

import java.util.ArrayList;
import java.util.List;
import java.util.function.UnaryOperator;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Small, dependency-free Markdown to HTML renderer covering what the SLeeLa docs use:
 * headings, fenced code, tables, nested lists, block quotes, rules, inline code, emphasis, links.
 * Raw HTML in the source is escaped, never passed through.
 */
public final class Markdown {
    private static final Pattern FENCE = Pattern.compile("^\\s*(`{3,}|~{3,})\\s*([\\w+#.-]*).*$");
    private static final Pattern HEADING = Pattern.compile("^\\s{0,3}(#{1,6})\\s+(.*?)\\s*#*\\s*$");
    private static final Pattern HR = Pattern.compile("^\\s{0,3}([-*_])(\\s*\\1){2,}\\s*$");
    private static final Pattern QUOTE = Pattern.compile("^\\s{0,3}>\\s?(.*)$");
    private static final Pattern ITEM = Pattern.compile("^(\\s*)([-*+]|\\d+[.)])\\s+(.*)$");
    private static final Pattern TABLE_SEP = Pattern.compile("^\\s*\\|?\\s*:?-{2,}:?\\s*(\\|\\s*:?-{2,}:?\\s*)*\\|?\\s*$");
    private static final Pattern CODE = Pattern.compile("(`+)(.+?)\\1");
    private static final Pattern IMG = Pattern.compile("!\\[([^\\]]*)\\]\\(([^)\\s]+)[^)]*\\)");
    private static final Pattern LINK = Pattern.compile("\\[([^\\]]+)\\]\\(([^)\\s]+)(?:\\s+&quot;.*?&quot;)?\\)");
    private static final Pattern AUTOLINK = Pattern.compile("&lt;(https?://[^\\s&]+)&gt;");
    private static final Pattern BOLD = Pattern.compile("(?s)(\\*\\*|__)(?=\\S)(.+?)(?<=\\S)\\1");
    private static final Pattern ITAL_STAR = Pattern.compile("(?s)(?<![\\w*])\\*(?!\\s)(.+?)(?<!\\s)\\*(?![\\w*])");
    private static final Pattern ITAL_UND = Pattern.compile("(?s)(?<![\\w])_(?!\\s)(.+?)(?<!\\s)_(?![\\w])");
    private static final Pattern SCHEME = Pattern.compile("^([a-zA-Z][a-zA-Z0-9+.-]*):.*");

    private final UnaryOperator<String> links;

    private Markdown(UnaryOperator<String> links) { this.links = links; }

    public static String render(String src, UnaryOperator<String> links) {
        String norm = src.replace("\r\n", "\n").replace('\r', '\n');
        List<String> lines = new ArrayList<>(List.of(norm.split("\n", -1)));
        return new Markdown(links).blocks(lines);
    }

    /** Text of the first level-1 heading, or null. */
    public static String firstHeading(String src) {
        for (String line : src.split("\n")) {
            Matcher m = HEADING.matcher(line);
            if (m.matches() && m.group(1).length() == 1) return m.group(2).replaceAll("[`*_]", "");
        }
        return null;
    }

    private String blocks(List<String> lines) {
        StringBuilder out = new StringBuilder();
        int n = lines.size();
        int i = 0;
        while (i < n) {
            String line = lines.get(i);
            if (line.isBlank()) { i++; continue; }

            Matcher m = FENCE.matcher(line);
            if (m.matches()) {
                String marker = m.group(1);
                String lang = m.group(2);
                StringBuilder code = new StringBuilder();
                i++;
                while (i < n && !lines.get(i).trim().startsWith(marker)) {
                    code.append(lines.get(i)).append('\n');
                    i++;
                }
                i++;
                out.append("<pre class=\"code\"><code").append(lang.isEmpty() ? "" : " class=\"language-" + Html.esc(lang) + "\"")
                   .append('>').append(Html.esc(code.toString())).append("</code></pre>\n");
                continue;
            }
            m = HEADING.matcher(line);
            if (m.matches()) {
                int lvl = m.group(1).length();
                String text = m.group(2);
                out.append("<h").append(lvl).append(" id=\"").append(Html.slug(text)).append("\">")
                   .append(inline(text)).append("</h").append(lvl).append(">\n");
                i++;
                continue;
            }
            if (HR.matcher(line).matches()) { out.append("<hr>\n"); i++; continue; }
            if (QUOTE.matcher(line).matches()) {
                List<String> q = new ArrayList<>();
                while (i < n) {
                    Matcher qm = QUOTE.matcher(lines.get(i));
                    if (!qm.matches()) break;
                    q.add(qm.group(1));
                    i++;
                }
                out.append("<blockquote>").append(blocks(q)).append("</blockquote>\n");
                continue;
            }
            if (line.indexOf('|') >= 0 && i + 1 < n && TABLE_SEP.matcher(lines.get(i + 1)).matches()) {
                i = table(lines, i, out);
                continue;
            }
            if (ITEM.matcher(line).matches()) {
                i = list(lines, i, out);
                continue;
            }
            // paragraph
            StringBuilder p = new StringBuilder();
            while (i < n) {
                String l = lines.get(i);
                if (l.isBlank() || FENCE.matcher(l).matches() || HEADING.matcher(l).matches()
                        || HR.matcher(l).matches() || QUOTE.matcher(l).matches() || ITEM.matcher(l).matches()) break;
                if (p.length() > 0) p.append('\n');
                p.append(l.trim());
                i++;
            }
            if (p.length() == 0) { out.append("<p>").append(inline(line.trim())).append("</p>\n"); i++; }
            else out.append("<p>").append(inline(p.toString())).append("</p>\n");
        }
        return out.toString();
    }

    private int table(List<String> lines, int i, StringBuilder out) {
        List<String> head = cells(lines.get(i));
        List<String> sep = cells(lines.get(i + 1));
        String[] align = new String[head.size()];
        for (int c = 0; c < align.length && c < sep.size(); c++) {
            String s = sep.get(c).trim();
            boolean l = s.startsWith(":"), r = s.endsWith(":");
            align[c] = l && r ? "center" : r ? "right" : null;
        }
        out.append("<div class=\"table-wrap\"><table class=\"md\"><thead><tr>");
        for (int c = 0; c < head.size(); c++) out.append(cell("th", head.get(c), align[c]));
        out.append("</tr></thead><tbody>\n");
        i += 2;
        while (i < lines.size() && !lines.get(i).isBlank() && lines.get(i).indexOf('|') >= 0) {
            List<String> row = cells(lines.get(i));
            out.append("<tr>");
            for (int c = 0; c < head.size(); c++) out.append(cell("td", c < row.size() ? row.get(c) : "", align[c]));
            out.append("</tr>\n");
            i++;
        }
        out.append("</tbody></table></div>\n");
        return i;
    }

    private String cell(String tag, String text, String align) {
        return "<" + tag + (align == null ? "" : " style=\"text-align:" + align + "\"") + ">" + inline(text.trim()) + "</" + tag + ">";
    }

    private static List<String> cells(String row) {
        String r = row.trim();
        if (r.startsWith("|")) r = r.substring(1);
        if (r.endsWith("|") && !r.endsWith("\\|")) r = r.substring(0, r.length() - 1);
        List<String> out = new ArrayList<>();
        StringBuilder cur = new StringBuilder();
        boolean code = false;
        for (int k = 0; k < r.length(); k++) {
            char ch = r.charAt(k);
            if (ch == '`') code = !code;
            if (ch == '\\' && k + 1 < r.length() && r.charAt(k + 1) == '|') { cur.append('|'); k++; continue; }
            if (ch == '|' && !code) { out.add(cur.toString()); cur.setLength(0); }
            else cur.append(ch);
        }
        out.add(cur.toString());
        return out;
    }

    private static int indent(String s) {
        int k = 0;
        while (k < s.length() && s.charAt(k) == ' ') k++;
        return k;
    }

    private int list(List<String> lines, int i, StringBuilder out) {
        int n = lines.size();
        Matcher first = ITEM.matcher(lines.get(i));
        first.matches();
        int base = first.group(1).length();
        boolean ordered = Character.isDigit(first.group(2).charAt(0));
        out.append(ordered ? "<ol>\n" : "<ul>\n");
        while (i < n) {
            Matcher m = ITEM.matcher(lines.get(i));
            if (!m.matches() || m.group(1).length() > base + 1 || m.group(1).length() < base) break;
            List<String> body = new ArrayList<>();
            body.add(m.group(3));
            i++;
            while (i < n) {
                String l = lines.get(i);
                if (l.isBlank()) {
                    int j = i;
                    while (j < n && lines.get(j).isBlank()) j++;
                    if (j < n && indent(lines.get(j)) > base + 1) { body.add(""); i = j; continue; }
                    break;
                }
                int ind = indent(l);
                if (ind > base + 1) { body.add(l); i++; continue; }
                if (ITEM.matcher(l).matches() || FENCE.matcher(l).matches() || HEADING.matcher(l).matches()
                        || HR.matcher(l).matches() || QUOTE.matcher(l).matches()) break;
                body.add(l.trim());
                i++;
            }
            out.append("<li>").append(listItem(body)).append("</li>\n");
        }
        out.append(ordered ? "</ol>\n" : "</ul>\n");
        return i;
    }

    private String listItem(List<String> body) {
        if (body.size() == 1) return inline(body.get(0).trim());
        int min = Integer.MAX_VALUE;
        for (int k = 1; k < body.size(); k++) {
            if (!body.get(k).isBlank()) min = Math.min(min, indent(body.get(k)));
        }
        List<String> dedented = new ArrayList<>();
        dedented.add(body.get(0));
        for (int k = 1; k < body.size(); k++) {
            String l = body.get(k);
            dedented.add(l.length() >= min && min != Integer.MAX_VALUE ? l.substring(Math.min(min, indent(l))) : l.trim());
        }
        String html = blocks(dedented).trim();
        if (html.startsWith("<p>")) {
            int end = html.indexOf("</p>");
            if (end > 0) html = html.substring(3, end) + html.substring(end + 4);
        }
        return html;
    }

    private String inline(String s) {
        List<String> tokens = new ArrayList<>();
        Matcher cm = CODE.matcher(s);
        StringBuilder sb = new StringBuilder();
        while (cm.find()) {
            tokens.add("<code>" + Html.esc(cm.group(2).trim()) + "</code>");
            cm.appendReplacement(sb, Matcher.quoteReplacement("\u0001" + (tokens.size() - 1) + "\u0001"));
        }
        cm.appendTail(sb);
        String t = Html.esc(sb.toString()).replace("&#39;", "'");

        t = replace(IMG, t, m -> {
            String url = unamp(m.group(2));
            return isHttp(url) ? "<img src=\"" + Html.esc(url) + "\" alt=\"" + m.group(1) + "\" loading=\"lazy\">" : m.group(1);
        });
        t = replace(LINK, t, m -> "<a href=\"" + href(unamp(m.group(2))) + "\">" + m.group(1) + "</a>");
        t = replace(AUTOLINK, t, m -> "<a href=\"" + Html.esc(unamp(m.group(1))) + "\">" + m.group(1) + "</a>");
        t = replace(BOLD, t, m -> "<strong>" + m.group(2) + "</strong>");
        t = replace(ITAL_STAR, t, m -> "<em>" + m.group(1) + "</em>");
        t = replace(ITAL_UND, t, m -> "<em>" + m.group(1) + "</em>");

        for (int k = 0; k < tokens.size(); k++) t = t.replace("\u0001" + k + "\u0001", tokens.get(k));
        return t;
    }

    private String href(String url) {
        Matcher sm = SCHEME.matcher(url);
        if (sm.matches()) {
            String sc = sm.group(1).toLowerCase(java.util.Locale.ROOT);
            if (!sc.equals("http") && !sc.equals("https") && !sc.equals("mailto")) return "#";
            return Html.esc(url);
        }
        return Html.esc(links == null ? url : links.apply(url));
    }

    private static boolean isHttp(String u) {
        String l = u.toLowerCase(java.util.Locale.ROOT);
        return l.startsWith("http://") || l.startsWith("https://");
    }

    private static String unamp(String s) { return s.replace("&amp;", "&"); }

    private static String replace(Pattern p, String in, java.util.function.Function<Matcher, String> f) {
        Matcher m = p.matcher(in);
        StringBuilder sb = new StringBuilder();
        while (m.find()) m.appendReplacement(sb, Matcher.quoteReplacement(f.apply(m)));
        m.appendTail(sb);
        return sb.toString();
    }
}
