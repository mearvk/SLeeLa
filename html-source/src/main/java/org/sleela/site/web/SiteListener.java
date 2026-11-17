package org.sleela.site.web;

import java.io.IOException;
import java.nio.file.Path;

import org.sleela.site.ContentIndex;
import org.sleela.site.SiteConfig;
import org.sleela.site.runtime.LibUnits;
import org.sleela.site.runtime.RuntimeConfig;
import org.sleela.site.runtime.SleelaRunner;

import jakarta.servlet.ServletContext;
import jakarta.servlet.ServletContextEvent;
import jakarta.servlet.ServletContextListener;
import jakarta.servlet.annotation.WebListener;

@WebListener
public class SiteListener implements ServletContextListener {
    public static final String INDEX = "sleela.index";
    public static final String ERROR = "sleela.index.error";

    @Override
    public void contextInitialized(ServletContextEvent e) {
        ServletContext ctx = e.getServletContext();
        Path root = SiteConfig.resolve(ctx);
        if (root == null) {
            ctx.setAttribute(ERROR, "SLeeLa source tree not found. Set SLEELA_HOME (or -Dsleela.home=...) to a checkout of mearvk/SLeeLa.");
            ctx.log("[sleela-site] " + ctx.getAttribute(ERROR));
            return;
        }
        try {
            ctx.setAttribute(INDEX, new ContentIndex(root));
            RuntimeConfig rc = RuntimeConfig.load(ctx::getInitParameter, root);
            LibUnits libs = new LibUnits(root);
            ctx.setAttribute(RuntimeServlet.LIBS, libs);
            ctx.setAttribute(RuntimeServlet.RUNNER, new SleelaRunner(rc, libs));
            ctx.log("[sleela-site] runtime mode=" + rc.mode + " bwrap=" + rc.bwrap + " binary=" + rc.binary);
            ctx.log("[sleela-site] content root: " + root.toAbsolutePath());
        } catch (IOException ex) {
            ctx.setAttribute(ERROR, "Cannot read content root " + root + ": " + ex.getMessage());
            ctx.log("[sleela-site] " + ctx.getAttribute(ERROR));
        }
    }
}
