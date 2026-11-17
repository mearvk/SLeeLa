package org.sleela.site.web;

import java.io.IOException;

import org.sleela.site.ContentIndex;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

@WebServlet(name = "search", urlPatterns = "/search")
public class SearchServlet extends BaseServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ContentIndex ix = index(req, resp);
        if (ix == null) return;
        String q = req.getParameter("q");
        if (q == null) q = "";
        if (q.length() > 100) q = q.substring(0, 100);
        req.setAttribute("q", q);
        req.setAttribute("hits", ix.search(q));
        render(req, resp, "search", q.isBlank() ? "Search" : "Search: " + q, "search");
    }
}
