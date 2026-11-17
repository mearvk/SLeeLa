package org.sleela.site.web;

import java.io.IOException;
import java.net.URI;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import org.sleela.site.ContentIndex;
import org.sleela.site.Model.ExampleGroup;
import org.sleela.site.runtime.FormToken;
import org.sleela.site.runtime.RuntimeConfig;
import org.sleela.site.runtime.Samples;
import org.sleela.site.runtime.SleelaRunner;
import org.sleela.site.runtime.VmChoice;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

/** /runtime: a form that runs (or syntax-checks) a SLeeLa program in the sandbox. GET shows the form, POST runs. */
@WebServlet(name = "runtime", urlPatterns = "/runtime")
public class RuntimeServlet extends BaseServlet {
    public static final String RUNNER = "sleela.runner";
    public static final String TOKEN = "sleela.runtime.token";
    public static final String LIBS = "sleela.runtime.libs";

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String source = null;
        String sample = req.getParameter("sample");
        if (sample != null && Samples.all().containsKey(sample)) {
            source = Samples.all().get(sample).getSource();
            req.setAttribute("loaded", Samples.all().get(sample).getTitle());
            req.setAttribute("sampleId", sample);
        }
        String from = req.getParameter("from");
        if (source == null && from != null) {
            ContentIndex ix = index(req, resp);
            if (ix == null) return;
            String[] seg = from.split("/", 2);
            ExampleGroup g = seg.length == 2 ? ix.exampleGroup(seg[0]) : null;
            String path = g == null ? null : ix.exampleRepoPath(g, seg[1]);
            String text = path == null || !path.endsWith(".sleela") ? null : ix.readText(path);
            if (text != null) { source = text; req.setAttribute("loaded", path); }
        }
        if (source == null) source = Samples.list().isEmpty() ? "" : Samples.list().get(0).getSource();
        show(req, resp, source, null);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        req.setCharacterEncoding("UTF-8");
        FormToken tokens = tokens();
        if (!sameOrigin(req) || !tokens.valid(req.getParameter("token"))) {
            error(req, resp, 403, "Form expired", "The form token is missing or expired. Reload the runtime page and submit again.");
            return;
        }
        String source = req.getParameter("source");
        if (source == null) source = "";
        if (source.length() > 400_000) source = source.substring(0, 400_000);
        source = source.replace("\r\n", "\n").replace('\r', '\n');
        SleelaRunner runner = runner();
        SleelaRunner.Result result = runner == null
                ? null
                : runner.submit("check".equals(req.getParameter("action")) ? "check" : "run", source, client(req),
                        VmChoice.find(runner.vms(), req.getParameter("vm")));
        req.setAttribute("action", "check".equals(req.getParameter("action")) ? "check" : "run");
        show(req, resp, source, result);
    }

    private void show(HttpServletRequest req, HttpServletResponse resp, String source, SleelaRunner.Result result)
            throws ServletException, IOException {
        SleelaRunner runner = runner();
        String unavailable = runner == null ? "The runtime is not configured on this server." : runner.unavailableReason();
        Map<String, Object> limits = new LinkedHashMap<>();
        if (runner != null) {
            RuntimeConfig c = runner.config();
            limits.put("timeout", c.timeoutSeconds);
            limits.put("cpu", c.cpuSeconds);
            limits.put("memoryMB", c.memoryBytes >> 20);
            limits.put("outputKB", c.maxOutputBytes / 1024);
            limits.put("sourceKB", c.maxSourceBytes / 1024);
            limits.put("perMinute", c.runsPerMinutePerClient);
        }
        req.setAttribute("source", source);
        req.setAttribute("result", result);
        req.setAttribute("unavailable", unavailable);
        req.setAttribute("limits", limits);
        req.setAttribute("samples", Samples.list());
        List<VmChoice> vms = runner == null ? VmChoice.catalog(null) : runner.vms();
        req.setAttribute("vms", vms);
        VmChoice picked = VmChoice.find(vms, req.getParameter("vm"));
        req.setAttribute("vmSelected", picked.isAvailable() ? picked.id : VmChoice.DEFAULT_ID);
        req.setAttribute("token", tokens().issue());
        resp.setHeader("Cache-Control", "no-store");
        render(req, resp, "runtime", "Runtime", "runtime");
    }

    private SleelaRunner runner() {
        Object o = getServletContext().getAttribute(RUNNER);
        return o instanceof SleelaRunner ? (SleelaRunner) o : null;
    }

    private FormToken tokens() {
        Object o = getServletContext().getAttribute(TOKEN);
        if (o instanceof FormToken) return (FormToken) o;
        synchronized (getServletContext()) {
            o = getServletContext().getAttribute(TOKEN);
            if (o instanceof FormToken) return (FormToken) o;
            FormToken t = new FormToken();
            getServletContext().setAttribute(TOKEN, t);
            return t;
        }
    }

    /** Behind Apache the peer is loopback and mod_proxy appends the real client as the last X-Forwarded-For entry. */
    static String client(HttpServletRequest req) {
        String peer = req.getRemoteAddr();
        if (peer != null && (peer.startsWith("127.") || peer.equals("::1") || peer.equals("0:0:0:0:0:0:0:1"))) {
            String xff = req.getHeader("X-Forwarded-For");
            if (xff != null && !xff.isBlank()) {
                String[] parts = xff.split(",");
                String last = parts[parts.length - 1].trim();
                if (!last.isEmpty() && last.length() < 64) return last;
            }
        }
        return peer == null ? "unknown" : peer;
    }

    /** Browsers send Origin (or Referer) on form posts; if present, its host must be ours. */
    static boolean sameOrigin(HttpServletRequest req) {
        String host = req.getHeader("Host");
        String src = req.getHeader("Origin");
        if (src == null || src.equals("null")) src = req.getHeader("Referer");
        if (src == null) return true;
        try {
            String h = URI.create(src).getAuthority();
            return h != null && host != null && h.equalsIgnoreCase(host);
        } catch (IllegalArgumentException e) {
            return false;
        }
    }
}
