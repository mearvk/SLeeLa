package com.mearvk.sleela.connector.http;

import com.mearvk.sleela.connector.SleelaInvocation;
import com.mearvk.sleela.connector.SleelaJavaConnector;
import com.mearvk.sleela.connector.SleelaResult;

import java.net.URI;
import java.net.URLEncoder;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.util.Objects;
import java.util.UUID;
import java.util.UUID;

/** Java HTTP client for a SLeeLa HTTP gateway. */
public final class SleelaHttpConnector implements SleelaJavaConnector {
    private final HttpClient client;
    private final URI baseUri;
    private final Duration timeout;
    private final int maxResponseBytes;
    private final int maxResponseBytes;

    public SleelaHttpConnector(URI baseUri) {
        this(baseUri, HttpClient.newHttpClient(), Duration.ofSeconds(30), 1024 * 1024);
    }

    public SleelaHttpConnector(URI baseUri, HttpClient client, Duration timeout) {
        this(baseUri, client, timeout, 1024 * 1024);
    }

    public SleelaHttpConnector(URI baseUri, HttpClient client, Duration timeout, int maxResponseBytes) {
        this.baseUri = normalize(Objects.requireNonNull(baseUri, "baseUri"));
        this.client = Objects.requireNonNull(client, "client");
        this.timeout = Objects.requireNonNull(timeout, "timeout");
        if (maxResponseBytes < 1024) throw new IllegalArgumentException("maxResponseBytes < 1024");
        this.maxResponseBytes = maxResponseBytes;
    }

    @Override
    public SleelaResult invoke(SleelaInvocation invocation) {
        try {
            URI uri = baseUri.resolve("invoke?operation=" +
                    URLEncoder.encode(invocation.operation(), StandardCharsets.UTF_8));
            String requestId = UUID.randomUUID().toString();
            HttpRequest request = HttpRequest.newBuilder(uri)
                    .timeout(timeout)
                    .header("Content-Type", "text/plain; charset=utf-8")
                    .header("Accept", "application/json, text/plain")
                    .header("X-SLeeLa-Protocol-Version", "1")
                    .header("X-SLeeLa-Request-ID", requestId)
                    .POST(HttpRequest.BodyPublishers.ofString(invocation.arguments(), StandardCharsets.UTF_8))
                    .build();
            HttpResponse<byte[]> response = client.send(request, HttpResponse.BodyHandlers.ofByteArray());
            if (response.body().length > maxResponseBytes) {
                return SleelaResult.failure("HTTP response exceeds configured limit of " + maxResponseBytes + " bytes");
            }
            String body = new String(response.body(), StandardCharsets.UTF_8);
            if (response.statusCode() / 100 != 2) {
                return SleelaResult.failure("HTTP " + response.statusCode() + ": " + body);
            }
            return SleelaResult.success(body);
        } catch (Exception failure) {
            return SleelaResult.failure(messageOf(failure));
        }
    }

    @Override
    public String health() throws Exception {
        HttpRequest request = HttpRequest.newBuilder(baseUri.resolve("health"))
                .timeout(timeout)
                .header("Accept", "application/json, text/plain")
                .header("X-SLeeLa-Protocol-Version", "1")
                .header("X-SLeeLa-Request-ID", UUID.randomUUID().toString())
                .GET()
                .build();
        HttpResponse<byte[]> response = client.send(request, HttpResponse.BodyHandlers.ofByteArray());
        if (response.body().length > maxResponseBytes) {
            throw new IllegalStateException("HTTP health response exceeds configured limit of " + maxResponseBytes + " bytes");
        }
        String body = new String(response.body(), StandardCharsets.UTF_8);
        if (response.statusCode() / 100 != 2) {
            throw new IllegalStateException("HTTP " + response.statusCode() + ": " + body);
        }
        return body;
    }

    private static URI normalize(URI uri) {
        String text = uri.toString();
        return URI.create(text.endsWith("/") ? text : text + "/");
    }

    private static String messageOf(Throwable failure) {
        return failure.getMessage() == null ? failure.toString() : failure.getMessage();
    }
}
