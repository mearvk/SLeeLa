import java.io.IOException;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Proxy;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;

import jakarta.servlet.*;
import jakarta.servlet.http.*;
import org.sleela.site.ContentIndex;
import org.sleela.site.web.*;

/**
 * Offline preview: runs the real servlets against a content tree with a mocked servlet API, renders the real JSP views with
 * MiniJsp, and writes static HTML (one index.html per route) plus the assets, ready for any static file server.
 *
 * usage: Preview <content-root> <webapp-dir> <out-dir> <url>...      (url = /path[?query])
 */
public final class Preview {
    @SuppressWarnings("unchecked")
    static <T> T proxy(Class<T> c, InvocationHandler h) { return (T) Proxy.newProxyInstance(Preview.class.getClassLoader(), new Class<?>[] {c}, h); }

    static final Map<String, HttpServlet> SERVLETS = new LinkedHashMap<>();

    public static void main(String[] a) throws Exception {
        Path content = Paths.get(a[0]), webapp = Paths.get(a[1]), out = Paths.get(a[2]);
        Map<String, Object> ctxAttrs = new HashMap<>();
        ctxAttrs.put(SiteListener.INDEX, new ContentIndex(content));
        ServletContext ctx = proxy(ServletContext.class, (p, m, x) -> m.getName().equals("getAttribute") ? ctxAttrs.get(x[0]) : null);
        ServletConfig cfg = proxy(ServletConfig.class, (p, m, x) -> m.getName().equals("getServletContext") ? ctx : null);
        SERVLETS.put("", new HomeServlet()); SERVLETS.put("tutorials", new TutorialServlet()); SERVLETS.put("examples", new ExampleServlet());
        SERVLETS.put("modules", new ModuleServlet()); SERVLETS.put("servers", new ServersServlet()); SERVLETS.put("api", new ApiServlet());
        SERVLETS.put("search", new SearchServlet());
        for (HttpServlet s : SERVLETS.values()) s.init(cfg);

        copyAssets(webapp, out);
        MiniJsp jsp = new MiniJsp(webapp);
        Map<String, String> routes = new LinkedHashMap<>();
        for (int i = 3; i < a.length; i++) {
            String url = a[i];
            String path = url, query = "";
            int q = url.indexOf('?');
            if (q >= 0) { path = url.substring(0, q); query = url.substring(q + 1); }
            Result r = invoke(path, query);
            String html = jsp.renderView(r.view, r.attrs, r.params);
            // routes with a query string get a distinct file name so several searches can coexist
            String fileName = query.isEmpty() ? "index.html" : "index__" + URLEncoder.encode(query, StandardCharsets.UTF_8).replace("%", "_") + ".html";
            Path target = out.resolve((path.equals("/") ? "" : path.substring(1)).replace("%20", " ")).resolve(fileName);
            routes.put(url, out.relativize(target).toString());
            Files.createDirectories(target.getParent());
            Files.writeString(target, html, StandardCharsets.UTF_8);
            System.out.println(r.status + " " + url + " -> " + out.relativize(target) + " (" + r.view + ")");
            routes.put("status:" + url, String.valueOf(r.status));
        }
        StringBuilder js = new StringBuilder("{");
        for (Map.Entry<String, String> e : routes.entrySet()) js.append(js.length() > 1 ? "," : "").append('"').append(e.getKey().replace("\"", "\\\"")).append("\":\"").append(e.getValue().replace("\\", "/")).append('"');
        Files.writeString(out.resolve("routes.json"), js.append('}').toString(), StandardCharsets.UTF_8);
    }

    static final class Result { int status = 200; String view; Map<String, Object> attrs; Map<String, String> params = new HashMap<>(); }

    static Result invoke(String path, String query) throws Exception {
        String[] parts = path.equals("/") ? new String[0] : path.substring(1).split("/", 2);
        String first = parts.length == 0 ? "" : parts[0];
        HttpServlet s = SERVLETS.get(first);
        Result r = new Result();
        if (query != null) for (String kv : query.split("&")) {
            if (kv.isEmpty()) continue;
            int e = kv.indexOf('=');
            r.params.put(dec(e < 0 ? kv : kv.substring(0, e)), e < 0 ? "" : dec(kv.substring(e + 1)));
        }
        Map<String, Object> attrs = new HashMap<>();
        r.attrs = attrs;
        HttpServletResponse resp = proxy(HttpServletResponse.class, (p, m, x) -> {
            if (m.getName().equals("setStatus")) r.status = (Integer) x[0];
            return null;
        });
        RequestDispatcher rd = proxy(RequestDispatcher.class, (p, m, x) -> null);
        String pathInfo = parts.length > 1 ? "/" + dec(parts[1]) : null;
        HttpServletRequest req = proxy(HttpServletRequest.class, (p, m, x) -> {
            switch (m.getName()) {
                case "getMethod": return "GET";
                case "getPathInfo": return pathInfo;
                case "getServletPath": return "/" + first;
                case "getContextPath": return "";
                case "setAttribute": attrs.put((String) x[0], x[1]); return null;
                case "getAttribute": return attrs.get(x[0]);
                case "getParameter": return r.params.get(x[0]);
                case "getRequestDispatcher": r.view = ((String) x[0]).replace("/WEB-INF/views/", "").replace(".jsp", ""); return rd;
                default: return null;
            }
        });
        if (s == null) throw new IOException("no servlet for " + path);
        s.service(req, resp);
        if (r.view == null) throw new IOException("servlet did not forward: " + path);
        return r;
    }

    static String dec(String s) { return URLDecoder.decode(s.replace("+", "%2B"), StandardCharsets.UTF_8).replace("%2B", "+"); }

    static void copyAssets(Path webapp, Path out) throws IOException {
        for (String d : new String[] {"assets"}) {
            Path src = webapp.resolve(d);
            try (var w = Files.walk(src)) {
                for (Path p : (Iterable<Path>) w::iterator) {
                    Path t = out.resolve(webapp.relativize(p).toString());
                    if (Files.isDirectory(p)) Files.createDirectories(t); else { Files.createDirectories(t.getParent()); Files.copy(p, t, java.nio.file.StandardCopyOption.REPLACE_EXISTING); }
                }
            }
        }
        Files.copy(webapp.resolve("favicon.svg"), out.resolve("favicon.svg"), java.nio.file.StandardCopyOption.REPLACE_EXISTING);
    }
}
