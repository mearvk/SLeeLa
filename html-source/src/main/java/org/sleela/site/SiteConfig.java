package org.sleela.site;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import jakarta.servlet.ServletContext;

/**
 * Locates the SLeeLa source tree. Order: system property sleela.home, env SLEELA_HOME,
 * content bundled in the WAR (WEB-INF/content), then a checkout found by walking up from the working directory.
 */
public final class SiteConfig {
    private SiteConfig() {}

    public static Path resolve(ServletContext ctx) {
        String[] explicit = {System.getProperty("sleela.home"), System.getenv("SLEELA_HOME"), ctx.getInitParameter("sleela.home")};
        for (String e : explicit) {
            if (e != null && !e.isBlank() && looksLikeRepo(Paths.get(e))) return Paths.get(e);
        }
        String bundled = ctx.getRealPath("/WEB-INF/content");
        if (bundled != null && looksLikeRepo(Paths.get(bundled))) return Paths.get(bundled);
        Path cur = Paths.get("").toAbsolutePath();
        for (int i = 0; i < 6 && cur != null; i++, cur = cur.getParent()) {
            if (looksLikeRepo(cur)) return cur;
        }
        return null;
    }

    static boolean looksLikeRepo(Path p) {
        return Files.isDirectory(p.resolve("tutorial")) || Files.isDirectory(p.resolve("lib"));
    }
}
