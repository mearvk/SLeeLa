package org.sleela.site.web;

import java.io.IOException;

import org.sleela.site.ContentIndex;

import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/** Liveness/readiness probe: 200 when the content index is loaded, 503 otherwise. */
@WebServlet(name = "health", urlPatterns = "/healthz")
public class HealthServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        Object ix = getServletContext().getAttribute(SiteListener.INDEX);
        resp.setContentType("application/json;charset=UTF-8");
        resp.setHeader("Cache-Control", "no-store");
        if (ix instanceof ContentIndex) {
            resp.getWriter().write("{\"status\":\"ok\",\"tutorials\":" + ((ContentIndex) ix).tutorials().size() + "}");
        } else {
            resp.setStatus(503);
            resp.getWriter().write("{\"status\":\"unavailable\"}");
        }
    }
}
