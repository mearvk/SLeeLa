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
            arguments = new String(input.readAllBytes(), StandardCharsets.UTF_8);
        }

        try {
            Object result = runtime.call(operation, arguments);
            send(exchange, 200, result == null ? "" : String.valueOf(result));
        } catch (RuntimeException failure) {
            send(exchange, 500, failure.getMessage() == null ? failure.toString() : failure.getMessage());
        }
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
