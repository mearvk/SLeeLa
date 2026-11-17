package org.sleela.site.web;

import java.io.IOException;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/** Container-level error pages (unmatched URLs, uncaught exceptions). Never echoes exception details. */
@WebServlet(name = "error", urlPatterns = "/error")
public class ErrorServlet extends BaseServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        Object code = req.getAttribute("jakarta.servlet.error.status_code");
        int status = code instanceof Integer ? (Integer) code : 500;
        if (status == 404) error(req, resp, 404, "Not found", "That page does not exist in this edition of the site.");
        else if (status == 405) error(req, resp, 405, "Method not allowed", "This site is read-only.");
        else error(req, resp, status, "Something went wrong", "The server could not complete the request.");
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        doGet(req, resp);
    }
}
