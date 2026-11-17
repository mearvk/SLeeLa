package org.sleela.site.web;

import java.io.IOException;

import org.sleela.site.ApiPages;
import org.sleela.site.ContentIndex;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/** /api, /api/{packages|glossary|authorship}, /api/package/{name}. */
@WebServlet(name = "api", urlPatterns = "/api/*")
public class ApiServlet extends BaseServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ContentIndex ix = index(req, resp);
        if (ix == null) return;
        String[] seg = segments(req);
        String page;
        if (seg.length == 0) page = "index";
        else if (seg.length == 1) page = seg[0];
        else if (seg.length == 2 && seg[0].equals("package")) page = "package/" + seg[1];
        else { notFound(req, resp); return; }

        String html = ix.apiPage(page);
        if (html == null) { notFound(req, resp); return; }
        req.setAttribute("apiTitle", ApiPages.title(html));
        req.setAttribute("apiBody", ApiPages.body(html, req.getContextPath()));
        req.setAttribute("apiPackages", ix.apiPackages());
        req.setAttribute("currentPackage", seg.length == 2 ? seg[1] : "");
        render(req, resp, "api", "API - " + ApiPages.title(html), "api");
    }
}
