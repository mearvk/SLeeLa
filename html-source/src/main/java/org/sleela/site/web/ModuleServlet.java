package org.sleela.site.web;

import java.io.IOException;
import java.util.List;

import org.sleela.site.ContentIndex;
import org.sleela.site.Model.Crumb;
import org.sleela.site.Model.Module;
import org.sleela.site.Routes;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/** /modules, /modules/{lib|sys}/{name}, /modules/{kind}/{name}/f/{path...}; add ?raw=1 for plain text. */
@WebServlet(name = "modules", urlPatterns = "/modules/*")
public class ModuleServlet extends BaseServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        ContentIndex ix = index(req, resp);
        if (ix == null) return;
        String[] seg = segments(req);
        if (seg.length == 0) {
            req.setAttribute("sysModules", ix.sysModules());
            req.setAttribute("libModules", ix.libModules());
            render(req, resp, "modules", "Modules", "modules");
            return;
        }
        if (seg.length < 2) { notFound(req, resp); return; }
        Module m = ix.module(seg[0], seg[1]);
        if (m == null) { notFound(req, resp); return; }

        if (seg.length == 2) {
            String readme = m.isReadme() ? ix.readText(m.getRelDir() + "/README.md") : null;
            req.setAttribute("module", m);
            req.setAttribute("intro", readme == null ? "" : markdown(req, readme, m.getRelDir()));
            List<Crumb> links = new java.util.ArrayList<>();
            for (String f : ix.moduleFiles(m)) links.add(new Crumb(f, m.getUrl() + "/f/" + Routes.path(f)));
            req.setAttribute("files", links);
            req.setAttribute("examples", ix.examplesFor(m.getName()));
            render(req, resp, "module", m.getName(), "modules");
            return;
        }
        if (!seg[2].equals("f") || seg.length < 4) { notFound(req, resp); return; }
        String rel = join(seg, 3);
        String repoPath = m.getRelDir() + "/" + rel;
        String text = ix.readText(repoPath);
        if (text == null) { notFound(req, resp); return; }
        String name = rel.substring(rel.lastIndexOf('/') + 1);
        if ("1".equals(req.getParameter("raw"))) { sendRawText(resp, name, text); return; }
        List<Crumb> crumbs = List.of(new Crumb("Modules", "/modules"), new Crumb(m.getName(), m.getUrl()));
        sourceView(req, resp, name, crumbs, "modules", repoPath, name, text, ContentIndex.langOf(name),
                req.getContextPath() + m.getUrl() + "/f/" + Routes.path(rel) + "?raw=1");
    }
}
