package org.sleela.site.web;

import java.io.IOException;

import jakarta.servlet.Filter;
import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.ServletRequest;
import jakarta.servlet.ServletResponse;
import jakarta.servlet.annotation.WebFilter;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/** Baseline security headers plus caching for static assets. */
@WebFilter(urlPatterns = "/*")
public class SecurityHeadersFilter implements Filter {
    @Override
    public void doFilter(ServletRequest req, ServletResponse res, FilterChain chain) throws IOException, ServletException {
        HttpServletRequest rq = (HttpServletRequest) req;
        HttpServletResponse rs = (HttpServletResponse) res;
        rs.setHeader("X-Content-Type-Options", "nosniff");
        rs.setHeader("X-Frame-Options", "SAMEORIGIN");
        rs.setHeader("Referrer-Policy", "same-origin");
        rs.setHeader("Content-Security-Policy",
                "default-src 'self'; img-src 'self' https: data:; style-src 'self' 'unsafe-inline'; script-src 'self'; base-uri 'self'; form-action 'self'; frame-ancestors 'self'");
        if (rq.getRequestURI().startsWith(rq.getContextPath() + "/assets/")) {
            rs.setHeader("Cache-Control", "public, max-age=3600");
        }
        chain.doFilter(req, res);
    }
}
