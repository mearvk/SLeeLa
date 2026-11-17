package org.sleela.site.web;

import java.io.IOException;
import java.util.List;

import org.sleela.site.ContentIndex;
import org.sleela.site.Model.Tutorial;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

@WebServlet(name = "tutorials", urlPatterns = "/tutorials/*")
public class TutorialServlet extends BaseServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ContentIndex ix = index(req, resp);
        if (ix == null) return;
        String[] seg = segments(req);
        if (seg.length == 0) {
            String readme = ix.readText("tutorial/README.md");
            req.setAttribute("intro", readme == null ? "" : markdown(req, readme, "tutorial"));
            req.setAttribute("tutorials", ix.tutorials());
            render(req, resp, "tutorials", "Tutorials", "tutorials");
            return;
        }
        if (seg.length != 1) { notFound(req, resp); return; }
        Tutorial t = ix.tutorial(seg[0]);
        if (t == null) { notFound(req, resp); return; }
        String md = ix.readText(t.getRelPath());
        if (md == null) { notFound(req, resp); return; }
        List<Tutorial> all = ix.tutorials();
        int i = all.indexOf(t);
        req.setAttribute("tutorial", t);
        req.setAttribute("prev", i > 0 ? all.get(i - 1) : null);
        req.setAttribute("next", i < all.size() - 1 ? all.get(i + 1) : null);
        req.setAttribute("tutorials", all);
        req.setAttribute("html", markdown(req, md, "tutorial"));
        render(req, resp, "tutorial", t.getTitle(), "tutorials");
    }
}
