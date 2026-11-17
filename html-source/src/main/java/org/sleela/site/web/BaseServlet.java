package org.sleela.site.web;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.function.UnaryOperator;

import org.sleela.site.ContentIndex;
import org.sleela.site.Markdown;
import org.sleela.site.Model.Crumb;
import org.sleela.site.Routes;

import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/** Shared helpers: index lookup, JSP forwarding, error pages, markdown rendering, plain-text download. */
abstract class BaseServlet extends HttpServlet {

    /** Returns the index, or sends a 503 page and returns null. */
    protected ContentIndex index(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        Object o = getServletContext().getAttribute(SiteListener.INDEX);
        if (o instanceof ContentIndex) return (ContentIndex) o;
        Object err = getServletContext().getAttribute(SiteListener.ERROR);
        error(req, resp, 503, "Content unavailable", err == null ? "The content index is not initialised." : err.toString());
        return null;
    }

    protected void render(HttpServletRequest req, HttpServletResponse resp, String view, String title, String section)
            throws ServletException, IOException {
        req.setAttribute("pageTitle", title);
        req.setAttribute("section", section);
        resp.setContentType("text/html;charset=UTF-8");
        req.getRequestDispatcher("/WEB-INF/views/" + view + ".jsp").forward(req, resp);
    }

    protected void notFound(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        error(req, resp, 404, "Not found", "That page does not exist in this edition of the site.");
    }

    protected void error(HttpServletRequest req, HttpServletResponse resp, int status, String title, String message)
            throws ServletException, IOException {
        resp.setStatus(status);
        req.setAttribute("status", status);
        req.setAttribute("message", message);
        render(req, resp, "error", title, "");
    }

    protected UnaryOperator<String> links(HttpServletRequest req, String baseDir) {
        String ctx = req.getContextPath();
        return u -> {
            String mapped = Routes.mapLink(baseDir, u);
            return mapped.startsWith("/") && !mapped.startsWith("//") ? ctx + mapped : mapped;
        };
    }

    protected String markdown(HttpServletRequest req, String md, String baseDir) {
        return Markdown.render(md, links(req, baseDir));
    }

    /** Redirect "/x" to "/x/" style canonical forms when the path info is missing. */
    protected static String[] segments(HttpServletRequest req) {
        String pi = req.getPathInfo();
        if (pi == null || pi.equals("/")) return new String[0];
        String trimmed = pi.startsWith("/") ? pi.substring(1) : pi;
        if (trimmed.endsWith("/")) trimmed = trimmed.substring(0, trimmed.length() - 1);
        return trimmed.split("/", -1);
    }

    protected static String join(String[] parts, int from) {
        StringBuilder sb = new StringBuilder();
        for (int i = from; i < parts.length; i++) {
            if (i > from) sb.append('/');
            sb.append(parts[i]);
        }
        return sb.toString();
    }

    protected void sendRawText(HttpServletResponse resp, String name, String text) throws IOException {
        byte[] data = text.getBytes(StandardCharsets.UTF_8);
        resp.setContentType("text/plain;charset=UTF-8");
        resp.setHeader("Content-Disposition", "inline; filename=\"" + name.replaceAll("[^A-Za-z0-9._-]", "_") + "\"");
        resp.setContentLength(data.length);
        resp.getOutputStream().write(data);
    }

    /** Fill the shared source-viewer model and forward. */
    protected void sourceView(HttpServletRequest req, HttpServletResponse resp, String title, List<Crumb> crumbs, String section,
                              String repoPath, String name, String text, String lang, String rawUrl) throws ServletException, IOException {
        boolean md = "markdown".equals(lang);
        req.setAttribute("crumbs", crumbs);
        req.setAttribute("fileName", name);
        req.setAttribute("repoPath", repoPath);
        req.setAttribute("lang", lang);
        req.setAttribute("rawUrl", rawUrl);
        req.setAttribute("lines", text.isEmpty() ? 0 : text.split("\n", -1).length);
        if (md) {
            String base = repoPath.contains("/") ? repoPath.substring(0, repoPath.lastIndexOf('/')) : "";
            req.setAttribute("html", markdown(req, text, base));
        } else {
            req.setAttribute("text", text);
        }
        render(req, resp, "source", title, section);
    }
}
