import java.lang.reflect.Method;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Tiny stand-in for a JSP container, only for offline previews and screenshots.
 * Supports exactly the subset used by this site: directives (ignored), EL, c:set/forEach/if/choose/when/otherwise/out,
 * fn:length/escapeXml, the t:layout tag file and jsp:doBody. Tomcat remains the real runtime.
 */
final class MiniJsp {
    private final Path webapp;
    private final Map<String, Node> cache = new HashMap<>();
    MiniJsp(Path webapp) { this.webapp = webapp; }

    // ------------------------------------------------------------ parse
    static final class Node {
        String text; String name; Map<String, String> attrs = new LinkedHashMap<>(); List<Node> kids = new ArrayList<>();
    }
    private static final Pattern TOK = Pattern.compile(
        "<%@.*?%>|<jsp:doBody\\s*/>|<(/?)(c|t):(\\w+)((?:[^>\"]|\"[^\"]*\")*?)(/?)>", Pattern.DOTALL);
    private static final Pattern ATTR = Pattern.compile("(\\w+)\\s*=\\s*\"([^\"]*)\"");

    Node parse(String src) {
        Node root = new Node(); root.name = "#root";
        Deque<Node> st = new ArrayDeque<>(); st.push(root);
        Matcher m = TOK.matcher(src);
        int last = 0;
        while (m.find()) {
            if (m.start() > last) text(st.peek(), src.substring(last, m.start()));
            last = m.end();
            String all = m.group();
            if (all.startsWith("<%@")) continue;
            if (all.startsWith("<jsp:doBody")) { Node n = new Node(); n.name = "doBody"; st.peek().kids.add(n); continue; }
            boolean close = !m.group(1).isEmpty(); boolean self = !m.group(5).isEmpty();
            String full = m.group(2) + ":" + m.group(3);
            if (close) { st.pop(); continue; }
            Node n = new Node(); n.name = full;
            Matcher a = ATTR.matcher(m.group(4));
            while (a.find()) n.attrs.put(a.group(1), a.group(2));
            st.peek().kids.add(n);
            if (!self) st.push(n);
        }
        if (last < src.length()) text(st.peek(), src.substring(last));
        return root;
    }
    private void text(Node parent, String t) { Node n = new Node(); n.name = "#text"; n.text = t; parent.kids.add(n); }

    // ------------------------------------------------------------ render
    String renderView(String view, Map<String, Object> request, Map<String, String> params) throws Exception {
        String prelude = Files.readString(webapp.resolve("WEB-INF/views/_prelude.jspf"), StandardCharsets.UTF_8);
        String page = Files.readString(webapp.resolve("WEB-INF/views/" + view + ".jsp"), StandardCharsets.UTF_8);
        Node root = parse(prelude + "\n" + page);
        Map<String, Object> scope = new HashMap<>(request);
        scope.put("param", params);
        scope.put("pageContext", Map.of("request", Map.of("contextPath", "")));
        StringBuilder out = new StringBuilder();
        render(root, scope, out, null);
        return out.toString();
    }

    private void render(Node n, Map<String, Object> sc, StringBuilder out, String body) throws Exception {
        switch (n.name) {
            case "#text": out.append(interp(n.text, sc)); return;
            case "doBody": if (body != null) out.append(body); return;
            case "#root": kids(n, sc, out, body); return;
            case "c:set": sc.put(n.attrs.get("var"), attr(n, "value", sc)); return;
            case "c:out": {
                Object v = attr(n, "value", sc);
                out.append(esc(v == null ? "" : String.valueOf(v))); return;
            }
            case "c:if": if (truthy(attr(n, "test", sc))) kids(n, sc, out, body); return;
            case "c:choose": {
                for (Node k : n.kids) {
                    if (k.name.equals("c:when") && truthy(attr(k, "test", sc))) { kids(k, sc, out, body); return; }
                    if (k.name.equals("c:otherwise")) { kids(k, sc, out, body); return; }
                }
                return;
            }
            case "c:forEach": {
                Object items = attr(n, "items", sc);
                Collection<?> col = items instanceof Collection ? (Collection<?>) items : items instanceof Map ? ((Map<?, ?>) items).values() : List.of();
                Object endO = attr(n, "end", sc);
                int end = endO == null ? Integer.MAX_VALUE : ((Number) num(endO)).intValue();
                String var = n.attrs.get("var"), vs = n.attrs.get("varStatus");
                int i = 0;
                for (Object o : col) {
                    if (i > end) break;
                    sc.put(var, o);
                    if (vs != null) sc.put(vs, Map.of("index", i, "count", i + 1));
                    kids(n, sc, out, body);
                    i++;
                }
                sc.remove(var); if (vs != null) sc.remove(vs);
                return;
            }
            case "t:layout": {
                Map<String, Object> tsc = new HashMap<>();
                for (String k : new String[] {"param", "pageContext"}) tsc.put(k, sc.get(k));
                for (Map.Entry<String, String> e : n.attrs.entrySet()) tsc.put(e.getKey(), attr(n, e.getKey(), sc));
                StringBuilder inner = new StringBuilder();
                kids(n, sc, inner, body);
                String tag = Files.readString(webapp.resolve("WEB-INF/tags/layout.tag"), StandardCharsets.UTF_8);
                StringBuilder tout = new StringBuilder();
                render(parse(tag), tsc, tout, inner.toString());
                out.append(tout);
                return;
            }
            default: throw new IllegalStateException("unsupported tag " + n.name);
        }
    }
    private void kids(Node n, Map<String, Object> sc, StringBuilder out, String body) throws Exception {
        for (Node k : n.kids) render(k, sc, out, body);
    }
    private Object attr(Node n, String a, Map<String, Object> sc) {
        String v = n.attrs.get(a);
        if (v == null) return null;
        if (v.startsWith("${") && v.endsWith("}") && v.indexOf("${", 2) < 0) return new El(v.substring(2, v.length() - 1), sc).eval();
        return interp(v, sc);
    }
    private String interp(String s, Map<String, Object> sc) {
        if (s.indexOf("${") < 0) return s;
        StringBuilder sb = new StringBuilder();
        int i = 0;
        while (true) {
            int a = s.indexOf("${", i);
            if (a < 0) { sb.append(s, i, s.length()); break; }
            sb.append(s, i, a);
            int b = s.indexOf('}', a);
            Object v = new El(s.substring(a + 2, b), sc).eval();
            sb.append(v == null ? "" : fmt(v));
            i = b + 1;
        }
        return sb.toString();
    }
    static String fmt(Object v) { return v instanceof Double && ((Double) v) % 1 == 0 ? String.valueOf(((Double) v).longValue()) : String.valueOf(v); }
    static boolean truthy(Object v) { return v instanceof Boolean ? (Boolean) v : v != null && "true".equalsIgnoreCase(String.valueOf(v)); }
    static String esc(String s) { return s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;").replace("\"", "&quot;").replace("'", "&#039;"); }
    static Object num(Object o) { if (o instanceof Number) return o; try { return Double.valueOf(String.valueOf(o)); } catch (Exception e) { return 0; } }

    // ------------------------------------------------------------ EL
    static final class El {
        final String s; int p; final Map<String, Object> sc;
        El(String s, Map<String, Object> sc) { this.s = s; this.sc = sc; }
        Object eval() { Object v = ternary(); ws(); if (p < s.length()) throw new IllegalStateException("EL trailing: " + s.substring(p) + " in " + s); return v; }
        void ws() { while (p < s.length() && Character.isWhitespace(s.charAt(p))) p++; }
        boolean eat(String t) { ws(); if (s.startsWith(t, p)) { p += t.length(); return true; } return false; }
        boolean eatWord(String w) {
            ws();
            if (s.startsWith(w, p) && (p + w.length() >= s.length() || !Character.isLetterOrDigit(s.charAt(p + w.length())))) { p += w.length(); return true; }
            return false;
        }
        Object ternary() {
            Object c = or();
            if (eat("?")) { Object a = ternary(); if (!eat(":")) throw new IllegalStateException("EL ':' missing in " + s); Object b = ternary(); return truthy(c) ? a : b; }
            return c;
        }
        Object or() { Object l = and(); while (eat("||") || eatWord("or")) { Object r = and(); l = truthy(l) || truthy(r); } return l; }
        Object and() { Object l = eq(); while (eat("&&") || eatWord("and")) { Object r = eq(); l = truthy(l) && truthy(r); } return l; }
        Object eq() {
            Object l = rel();
            while (true) {
                if (eat("==") || eatWord("eq")) l = same(l, rel());
                else if (eat("!=") || eatWord("ne")) l = !same(l, rel());
                else return l;
            }
        }
        Object rel() {
            Object l = add();
            while (true) {
                if (eat("<=") || eatWord("le")) l = cmp(l, add()) <= 0;
                else if (eat(">=") || eatWord("ge")) l = cmp(l, add()) >= 0;
                else if (eatWord("lt") || (peekIs('<') && eat("<"))) l = cmp(l, add()) < 0;
                else if (eatWord("gt") || (peekIs('>') && eat(">"))) l = cmp(l, add()) > 0;
                else return l;
            }
        }
        boolean peekIs(char c) { ws(); return p < s.length() && s.charAt(p) == c; }
        Object add() {
            Object l = unary();
            while (true) {
                ws();
                if (p < s.length() && s.charAt(p) == '+') { p++; l = ((Number) num(l)).longValue() + ((Number) num(unary())).longValue(); }
                else if (p < s.length() && s.charAt(p) == '-') { p++; l = ((Number) num(l)).longValue() - ((Number) num(unary())).longValue(); }
                else return l;
            }
        }
        Object unary() {
            if (eat("!") || eatWord("not")) return !truthy(unary());
            if (eatWord("empty")) { Object v = unary(); return v == null || "".equals(v) || (v instanceof Collection && ((Collection<?>) v).isEmpty()) || (v instanceof Map && ((Map<?, ?>) v).isEmpty()); }
            return primary();
        }
        Object primary() {
            ws();
            if (eat("(")) { Object v = ternary(); eat(")"); return v; }
            char c = s.charAt(p);
            if (c == '\'' || c == '"') { int e = s.indexOf(c, p + 1); String v = s.substring(p + 1, e); p = e + 1; return v; }
            if (Character.isDigit(c)) { int b = p; while (p < s.length() && Character.isDigit(s.charAt(p))) p++; return Long.parseLong(s.substring(b, p)); }
            int b = p;
            while (p < s.length() && (Character.isLetterOrDigit(s.charAt(p)) || s.charAt(p) == '_' || s.charAt(p) == ':')) p++;
            String id = s.substring(b, p);
            if (id.equals("true")) return true; if (id.equals("false")) return false; if (id.equals("null")) return null;
            Object cur;
            if (id.startsWith("fn:")) {
                eat("("); Object a = ternary(); eat(")");
                switch (id) {
                    case "fn:length": return a == null ? 0 : a instanceof Collection ? ((Collection<?>) a).size() : a instanceof Map ? ((Map<?, ?>) a).size() : String.valueOf(a).length();
                    case "fn:escapeXml": return a == null ? "" : esc(String.valueOf(a));
                    default: throw new IllegalStateException("unsupported fn " + id);
                }
            }
            cur = sc.get(id);
            while (p < s.length() && s.charAt(p) == '.') {
                p++; int b2 = p;
                while (p < s.length() && (Character.isLetterOrDigit(s.charAt(p)) || s.charAt(p) == '_')) p++;
                cur = prop(cur, s.substring(b2, p));
            }
            return cur;
        }
        static Object prop(Object o, String name) {
            if (o == null) return null;
            if (o instanceof Map) return ((Map<?, ?>) o).get(name);
            String cap = Character.toUpperCase(name.charAt(0)) + name.substring(1);
            for (String pre : new String[] {"get", "is"}) {
                try { Method m = o.getClass().getMethod(pre + cap); m.setAccessible(true); return m.invoke(o); } catch (NoSuchMethodException ignored) { } catch (Exception e) { throw new RuntimeException(e); }
            }
            throw new IllegalStateException("no property " + name + " on " + o.getClass());
        }
        static boolean same(Object a, Object b) {
            if (a == null || b == null) return a == b;
            if (a instanceof Number || b instanceof Number) return ((Number) num(a)).doubleValue() == ((Number) num(b)).doubleValue();
            if (a instanceof Boolean || b instanceof Boolean) return truthy(a) == truthy(b);
            return String.valueOf(a).equals(String.valueOf(b));
        }
        static int cmp(Object a, Object b) {
            if (a instanceof Number || b instanceof Number) return Double.compare(((Number) num(a)).doubleValue(), ((Number) num(b)).doubleValue());
            return String.valueOf(a).compareTo(String.valueOf(b));
        }
    }
}
