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

/** Java HTTP client for a SLeeLa HTTP gateway. */
public final class SleelaHttpConnector implements SleelaJavaConnector {
    private final HttpClient client;
    private final URI baseUri;
    private final Duration timeout;

    public SleelaHttpConnector(URI baseUri) {
        this(baseUri, HttpClient.newHttpClient(), Duration.ofSeconds(30));
    }

    public SleelaHttpConnector(URI baseUri, HttpClient client, Duration timeout) {
        this.baseUri = normalize(Objects.requireNonNull(baseUri, "baseUri"));
        this.client = Objects.requireNonNull(client, "client");
        this.timeout = Objects.requireNonNull(timeout, "timeout");
    }

    @Override
    public SleelaResult invoke(SleelaInvocation invocation) {
        try {
            URI uri = baseUri.resolve("invoke?operation=" +
                    URLEncoder.encode(invocation.operation(), StandardCharsets.UTF_8));
            HttpRequest request = HttpRequest.newBuilder(uri)
                    .timeout(timeout)
                    .header("Content-Type", "text/plain; charset=utf-8")
                    .POST(HttpRequest.BodyPublishers.ofString(invocation.arguments(), StandardCharsets.UTF_8))
                    .build();
            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString(StandardCharsets.UTF_8));
            if (response.statusCode() / 100 != 2) {
                return SleelaResult.failure("HTTP " + response.statusCode() + ": " + response.body());
            }
            return SleelaResult.success(response.body());
        } catch (Exception failure) {
            return SleelaResult.failure(messageOf(failure));
        }
    }

    @Override
    public String health() throws Exception {
        HttpRequest request = HttpRequest.newBuilder(baseUri.resolve("health"))
                .timeout(timeout)
                .GET()
                .build();
        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString(StandardCharsets.UTF_8));
        if (response.statusCode() / 100 != 2) {
            throw new IllegalStateException("HTTP " + response.statusCode() + ": " + response.body());
        }
        return response.body();
    }

    private static URI normalize(URI uri) {
        String text = uri.toString();
        return URI.create(text.endsWith("/") ? text : text + "/");
    }

    private static String messageOf(Throwable failure) {
        return failure.getMessage() == null ? failure.toString() : failure.getMessage();
    }
}
