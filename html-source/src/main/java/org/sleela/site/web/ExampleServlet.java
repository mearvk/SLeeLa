package org.sleela.site.web;

import java.io.IOException;
import java.util.List;

import org.sleela.site.ContentIndex;
import org.sleela.site.Model.Crumb;
import org.sleela.site.Model.ExampleGroup;
import org.sleela.site.Routes;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/** /examples, /examples/{group}, /examples/{group}/{file...}; add ?raw=1 for plain text. */
@WebServlet(name = "examples", urlPatterns = "/examples/*")
public class ExampleServlet extends BaseServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ContentIndex ix = index(req, resp);
        if (ix == null) return;
        String[] seg = segments(req);
        if (seg.length == 0) {
            req.setAttribute("groups", ix.exampleGroups().values());
            render(req, resp, "examples", "Examples by module", "examples");
            return;
        }
        ExampleGroup g = ix.exampleGroup(seg[0]);
        if (g == null) { notFound(req, resp); return; }
        if (seg.length == 1) {
            req.setAttribute("group", g);
            String readme = null;
            for (var f : g.getFiles()) if (f.getRel().equalsIgnoreCase("README.md")) readme = ix.readText(g.getBaseRel() + "/" + f.getRel());
            req.setAttribute("intro", readme == null ? "" : markdown(req, readme, g.getBaseRel()));
            render(req, resp, "example-group", g.getLabel(), "examples");
            return;
        }
        String rel = join(seg, 1);
        String repoPath = ix.exampleRepoPath(g, rel);
        String text = repoPath == null ? null : ix.readText(repoPath);
        if (text == null) { notFound(req, resp); return; }
        String name = rel.substring(rel.lastIndexOf('/') + 1);
        if ("1".equals(req.getParameter("raw"))) { sendRawText(resp, name, text); return; }
        List<Crumb> crumbs = List.of(new Crumb("Examples", "/examples"), new Crumb(g.getLabel(), g.getUrl()));
        sourceView(req, resp, name, crumbs, "examples", repoPath, name, text, ContentIndex.langOf(name),
                req.getContextPath() + g.getUrl() + "/" + Routes.path(rel) + "?raw=1");
    }
}
