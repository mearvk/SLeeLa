package com.mearvk.sleela.connector.http;

import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.Objects;

/**
 * Tomcat / Apache-servable HTTP module that LISTENS for HTTP/2+ and carries the
 * SLeeLa HTTP 3.0 envelope inside a standard HTTP request/response body.
 *
 * <p>This is the server side of the custom packet scheme implemented by
 * {@code http-3.0/http3_transport.py}. A client wraps the exact H3 envelope
 * bytes (integrity gate and all) in a standards-shaped HTTP/1.1 or HTTP/2
 * message; Tomcat terminates HTTP/2 and hands us the request body; we feed the
 * body straight into the H3 pipeline and return the H3 response bytes as the
 * HTTP response body. The H3 MAC / NONCE / INTACTX / BASKET are never touched,
 * so the H3 integrity guarantees survive end to end.</p>
 *
 * <p>HTTP/2 is negotiated by the container (Tomcat's {@code UpgradeProtocol} /
 * ALPN, or Apache {@code mod_http2} in front). This servlet is transport-form
 * agnostic: it works identically whether Tomcat delivered the request over
 * HTTP/1.1 or HTTP/2, because the H3 envelope rides in the body either way.</p>
 *
 * <p>Wire contract (matches {@code http3_transport.py}):</p>
 * <ul>
 *   <li>Request body = exact H3 envelope bytes (textual {@code H3 ...} line or
 *       the negotiated binary form).</li>
 *   <li>{@code Content-Type: application/vnd.sleela.h3+octet-stream; version=3}</li>
 *   <li>{@code X-SLeeLa-H3: 3;text} or {@code 3;binary} marks the embedded form.</li>
 *   <li>Response body = H3 response line ({@code H3R <status> ...}).</li>
 * </ul>
 */
public final class SleelaH3Servlet extends HttpServlet {

    /** Media type identifying an embedded H3 envelope body. */
    public static final String H3_MEDIA_TYPE =
            "application/vnd.sleela.h3+octet-stream; version=3";
    /** Marker header naming the embedded protocol generation + wire form. */
    public static final String H3_MARKER_HEADER = "X-SLeeLa-H3";

    /** Maximum accepted request body, matching HTTP3_ENVELOPE limits with headroom. */
    private static final int MAX_BODY_BYTES = 64 * 1024;

    /**
     * The pipeline sink: given the raw H3 envelope bytes, return the raw H3
     * response bytes. Wire this to the native pipeline (via the SLeeLa runtime /
     * JNI or process connector) or to a test double. Kept as a functional
     * interface so the servlet has no hard dependency on a specific backend.
     */
    @FunctionalInterface
    public interface H3Pipeline {
        byte[] handleWire(byte[] wire);
    }

    private final transient H3Pipeline pipeline;

    public SleelaH3Servlet(H3Pipeline pipeline) {
        this.pipeline = Objects.requireNonNull(pipeline, "pipeline");
    }

    /** Health probe: GET returns a plain-text liveness string. */
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        byte[] body = ("SLeeLa H3 module healthy; protocol=" + req.getProtocol())
                .getBytes(StandardCharsets.UTF_8);
        resp.setStatus(HttpServletResponse.SC_OK);
        resp.setContentType("text/plain; charset=utf-8");
        resp.setContentLength(body.length);
        try (OutputStream out = resp.getOutputStream()) {
            out.write(body);
        }
    }

    /** Carry an H3 envelope: POST body -> pipeline -> H3 response body. */
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        String marker = req.getHeader(H3_MARKER_HEADER);
        if (marker == null || marker.isBlank()) {
            fail(resp, HttpServletResponse.SC_BAD_REQUEST,
                    "missing " + H3_MARKER_HEADER + " header; not an H3-carrying request");
            return;
        }

        byte[] wire = readBoundedBody(req);
        if (wire == null) {
            fail(resp, HttpServletResponse.SC_REQUEST_ENTITY_TOO_LARGE,
                    "request body exceeds " + MAX_BODY_BYTES + " bytes");
            return;
        }

        final byte[] responseWire;
        try {
            responseWire = pipeline.handleWire(wire);
        } catch (RuntimeException failure) {
            // A backend fault is an HTTP 502; the H3 integrity gate itself never
            // throws -- a bad packet comes back as an H3R error body with 200.
            fail(resp, HttpServletResponse.SC_BAD_GATEWAY,
                    failure.getMessage() == null ? failure.toString() : failure.getMessage());
            return;
        }

        // HTTP transport accepted the packet; H3-level status (OK / BAD_DIGEST /
        // TAMPERED / REPLAYED / ...) is inside the H3R response body, per the
        // spec's "transport OK never implies business success" rule.
        resp.setStatus(HttpServletResponse.SC_OK);
        resp.setContentType(H3_MEDIA_TYPE);
        resp.setHeader(H3_MARKER_HEADER, wireFormOf(responseWire));
        resp.setContentLength(responseWire.length);
        try (OutputStream out = resp.getOutputStream()) {
            out.write(responseWire);
        }
    }

    /** Reads the request body up to the cap; returns null if the cap is exceeded. */
    private static byte[] readBoundedBody(HttpServletRequest req) throws IOException {
        try (InputStream in = req.getInputStream()) {
            byte[] buf = new byte[8192];
            var acc = new java.io.ByteArrayOutputStream();
            int total = 0;
            int n;
            while ((n = in.read(buf)) != -1) {
                total += n;
                if (total > MAX_BODY_BYTES) {
                    return null;
                }
                acc.write(buf, 0, n);
            }
            return acc.toByteArray();
        }
    }

    /** Textual H3 lines begin with "H3 "; everything else is the binary form. */
    private static String wireFormOf(byte[] wire) {
        if (wire.length >= 3 && wire[0] == 'H' && wire[1] == '3' && wire[2] == ' ') {
            return "3;text";
        }
        return "3;binary";
    }

    private static void fail(HttpServletResponse resp, int status, String message) throws IOException {
        byte[] body = message.getBytes(StandardCharsets.UTF_8);
        resp.setStatus(status);
        resp.setContentType("text/plain; charset=utf-8");
        resp.setContentLength(body.length);
        try (OutputStream out = resp.getOutputStream()) {
            out.write(body);
        }
    }
}
