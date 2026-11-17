package org.sleela.site.web;

import java.io.IOException;

import org.sleela.site.ContentIndex;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

@WebServlet(name = "home", urlPatterns = {"", "/home"})
public class HomeServlet extends BaseServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ContentIndex ix = index(req, resp);
        if (ix == null) return;
        req.setAttribute("tutorials", ix.tutorials());
        req.setAttribute("exampleGroupCount", ix.exampleGroups().size());
        req.setAttribute("libCount", ix.libModules().size());
        req.setAttribute("sysCount", ix.sysModules().size());
        req.setAttribute("apiCount", ix.apiPackages().size());
        req.setAttribute("httpModules", ix.httpModules());
        render(req, resp, "home", "SLeeLa - The Now. The Ingredient.", "home");
    }
}
