package org.sleela.site.web;

import java.io.IOException;

import org.sleela.site.ContentIndex;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/** Landing page for the HTTP servers and every HTTP-related module. */
@WebServlet(name = "servers", urlPatterns = "/servers")
public class ServersServlet extends BaseServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ContentIndex ix = index(req, resp);
        if (ix == null) return;
        String readme = ix.readText("http-servers/README.md");
        req.setAttribute("intro", readme == null ? "" : markdown(req, readme, "http-servers"));
        req.setAttribute("httpModules", ix.httpModules());
        render(req, resp, "servers", "HTTP servers", "servers");
    }
}
