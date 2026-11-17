package org.sleela.site;

import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.util.ArrayDeque;
import java.util.Deque;

/** URL building and repo-path to site-route mapping. All returned URLs are context-relative. */
public final class Routes {
    private Routes() {}

    public static String seg(String s) {
        return URLEncoder.encode(s, StandardCharsets.UTF_8).replace("+", "%20");
    }

    public static String path(String rel) {
        StringBuilder sb = new StringBuilder();
        for (String p : rel.split("/")) {
            if (p.isEmpty()) continue;
            if (sb.length() > 0) sb.append('/');
            sb.append(seg(p));
        }
        return sb.toString();
    }

    public static String exampleGroupSlug(String key) { return key.replace('/', '-'); }

    /** Map a repo-relative path (no leading slash, normalised) to a site route. */
    public static String mapRepoPath(String p) {
        if (p.isEmpty()) return "/";
        if (p.equals("tutorial") || p.equals("tutorial/README.md")) return "/tutorials";
        if (p.startsWith("tutorial/") && p.endsWith(".md") && p.indexOf('/', 9) < 0) {
            return "/tutorials/" + seg(p.substring(9, p.length() - 3));
        }
        if (p.equals("examples") || p.startsWith("examples/")) return exampleRoute("examples", p);
        if (p.equals("lib/examples") || p.startsWith("lib/examples/")) return exampleRoute("lib/examples", p);
        if (p.equals("docs/api") || p.equals("docs/api/index.html")) return "/api";
        if (p.startsWith("docs/api/package/") && p.endsWith(".html")) {
            return "/api/package/" + seg(p.substring(17, p.length() - 5));
        }
        if (p.startsWith("docs/api/") && p.endsWith(".html") && p.indexOf('/', 9) < 0) {
            return "/api/" + seg(p.substring(9, p.length() - 5));
        }
        if (p.equals("lib")) return "/modules";
        if (p.startsWith("lib/")) return moduleRoute("lib", p.substring(4));
        return moduleRoute("sys", p);
    }

    private static String moduleRoute(String kind, String rest) {
        int i = rest.indexOf('/');
        String name = i < 0 ? rest : rest.substring(0, i);
        String tail = i < 0 ? "" : rest.substring(i + 1);
        String base = "/modules/" + kind + "/" + seg(name);
        return tail.isEmpty() ? base : base + "/f/" + path(tail);
    }

    private static String exampleRoute(String root, String p) {
        String rest = p.length() <= root.length() ? "" : p.substring(root.length() + 1);
        if (rest.isEmpty()) return "/examples";
        int i = rest.indexOf('/');
        if (i < 0) return "/examples/" + seg(exampleGroupSlug(root)) + "/" + seg(rest);
        String first = rest.substring(0, i);
        String tail = rest.substring(i + 1);
        String slug = exampleGroupSlug(root + "/" + first);
        return tail.isEmpty() ? "/examples/" + seg(slug) : "/examples/" + seg(slug) + "/" + path(tail);
    }

    /** Resolve a markdown link found in a document living in {@code baseDir} (repo-relative). */
    public static String mapLink(String baseDir, String url) {
        if (url == null || url.isEmpty() || url.startsWith("#") || url.startsWith("//")) return url;
        if (url.matches("^[a-zA-Z][a-zA-Z0-9+.-]*:.*")) return url;
        String frag = "";
        int h = url.indexOf('#');
        if (h >= 0) { frag = url.substring(h); url = url.substring(0, h); }
        int q = url.indexOf('?');
        if (q >= 0) url = url.substring(0, q);
        if (url.isEmpty()) return frag.isEmpty() ? "#" : frag;
        String joined = url.startsWith("/") ? url.substring(1) : (baseDir.isEmpty() ? url : baseDir + "/" + url);
        Deque<String> stack = new ArrayDeque<>();
        for (String part : joined.split("/")) {
            if (part.isEmpty() || part.equals(".")) continue;
            if (part.equals("..")) {
                if (stack.isEmpty()) return "#";
                stack.removeLast();
            } else stack.addLast(part);
        }
        return mapRepoPath(String.join("/", stack)) + frag;
    }
}
