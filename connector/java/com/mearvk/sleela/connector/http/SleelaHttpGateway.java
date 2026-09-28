package com.mearvk.sleela.connector.http;

import com.mearvk.sleela.gui.SleelaRuntime;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpServer;

import java.io.IOException;
import java.io.InputStream;
import java.net.InetSocketAddress;
import java.net.URI;
import java.nio.charset.StandardCharsets;
import java.util.Objects;

/**
 * Small JDK-only HTTP gateway for browser and web-driven artifacts.
 *
 * Endpoints:
 *   GET  /sleela/health
 *   POST /sleela/invoke?operation=name
 *
 * The request body is the operation argument string. No Java serialization is
 * exposed over HTTP; SLeeLa remains authoritative for the operation itself.
 */
public final class SleelaHttpGateway implements AutoCloseable {
    /** Hard cap on an accepted request body, to bound memory on a POST. */
    private static final int MAX_BODY_BYTES = 1 << 20; // 1 MiB

    private final HttpServer server;
    private final SleelaRuntime runtime;
    private final String allowOrigin;

    public SleelaHttpGateway(String host, int port, SleelaRuntime runtime) throws IOException {
        this(host, port, runtime, "");
    }

    public SleelaHttpGateway(String host, int port, SleelaRuntime runtime, String allowOrigin) throws IOException {
        Objects.requireNonNull(host, "host");
        this.runtime = Objects.requireNonNull(runtime, "runtime");
        this.allowOrigin = allowOrigin == null ? "" : allowOrigin;
        this.server = HttpServer.create(new InetSocketAddress(host, port), 0);
        this.server.createContext("/sleela/health", this::health);
        this.server.createContext("/sleela/invoke", this::invoke);
    }

    public void start() {
        server.start();
    }

    public int port() {
        return server.getAddress().getPort();
    }

    public URI healthUri() {
        return URI.create("http://" + server.getAddress().getHostString() + ":" + port() + "/sleela/health");
    }

    private void health(HttpExchange exchange) throws IOException {
        if (!"GET".equalsIgnoreCase(exchange.getRequestMethod())) {
            send(exchange, 405, "GET required");
            return;
        }
        send(exchange, 200, "SLeeLa HTTP gateway = healthy");
    }

    private void invoke(HttpExchange exchange) throws IOException {
        if ("OPTIONS".equalsIgnoreCase(exchange.getRequestMethod())) {
            addCors(exchange);
            send(exchange, 204, "");
            return;
        }
        if (!"POST".equalsIgnoreCase(exchange.getRequestMethod())) {
            send(exchange, 405, "POST required");
            return;
        }

        String operation = queryParameter(exchange.getRequestURI(), "operation");
        if (operation == null || operation.isBlank()) {
            send(exchange, 400, "operation is required");
            return;
        }

        String arguments;
        try (InputStream input = exchange.getRequestBody()) {
            byte[] body = readBounded(input, MAX_BODY_BYTES);
            if (body == null) {
                send(exchange, 413, "request body exceeds " + MAX_BODY_BYTES + " bytes");
                return;
            }
            arguments = new String(body, StandardCharsets.UTF_8);
        }

        try {
            Object result = runtime.call(operation, arguments);
            send(exchange, 200, result == null ? "" : String.valueOf(result));
        } catch (Throwable failure) {
            // Never let a runtime fault (RuntimeException OR Error) escape the
            // handler; a leaked throwable would drop the connection without a
            // status. Report a 500 with a bounded message.
            String msg = failure.getMessage();
            send(exchange, 500, msg == null ? failure.toString() : msg);
        }
    }

    /**
     * Reads at most {@code limit} bytes from {@code input}. Returns null if the
     * stream would exceed the cap (so the caller can answer 413), guarding
     * against an unbounded request body (DoS).
     */
    private static byte[] readBounded(InputStream input, int limit) throws IOException {
        java.io.ByteArrayOutputStream acc = new java.io.ByteArrayOutputStream();
        byte[] buf = new byte[8192];
        int total = 0;
        int n;
        while ((n = input.read(buf)) != -1) {
            total += n;
            if (total > limit) {
                return null;
            }
            acc.write(buf, 0, n);
        }
        return acc.toByteArray();
    }

    private void send(HttpExchange exchange, int status, String body) throws IOException {
        addCors(exchange);
        byte[] bytes = body.getBytes(StandardCharsets.UTF_8);
        exchange.getResponseHeaders().set("Content-Type", "text/plain; charset=utf-8");
        exchange.sendResponseHeaders(status, bytes.length);
        try (var output = exchange.getResponseBody()) {
            output.write(bytes);
        }
    }

    private void addCors(HttpExchange exchange) {
        if (!allowOrigin.isBlank()) {
            exchange.getResponseHeaders().set("Access-Control-Allow-Origin", allowOrigin);
            exchange.getResponseHeaders().set("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
            exchange.getResponseHeaders().set("Access-Control-Allow-Headers", "Content-Type");
        }
    }

    private static String queryParameter(URI uri, String name) {
        String query = uri.getRawQuery();
        if (query == null) {
            return null;
        }
        for (String part : query.split("&")) {
            int separator = part.indexOf('=');
            String key = separator < 0 ? part : part.substring(0, separator);
            if (name.equals(java.net.URLDecoder.decode(key, StandardCharsets.UTF_8))) {
                String value = separator < 0 ? "" : part.substring(separator + 1);
                return java.net.URLDecoder.decode(value, StandardCharsets.UTF_8);
            }
        }
        return null;
    }

    @Override
    public void close() {
        server.stop(0);
    }
}
