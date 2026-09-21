package com.mearvk.sleela.munction.channels;

import com.mearvk.sleela.munction.ChannelReceipt;
import com.mearvk.sleela.munction.Reception;
import com.mearvk.sleela.munction.SendCoherence;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.time.Duration;
import java.util.ArrayDeque;
import java.util.Deque;

/**
 * Internet (HTTP/HTTPS) channel ({@code http:} / {@code https:} scheme,
 * MUNCTION.md §4), built on the JDK {@link HttpClient} like the connector HTTP
 * path (CONNECTOR.md). {@code send} POSTs the datum as the request body and
 * queues the response body for {@code consume}. Coherent send treats a 2xx
 * response as full acknowledgment; a transport bump is recorded, not thrown.
 */
public final class HttpChannel extends AbstractChannel {

    private HttpClient client;
    private String url;
    private final Deque<byte[]> responses = new ArrayDeque<>();
    private String note = "";

    @Override
    public String scheme() {
        return "http";
    }

    @Override
    public void open(String address) {
        this.address = address;
        // Rebuild a full URL; addressOf() stripped the scheme + leading //.
        this.url = (address.startsWith("http") ? address : "http://" + address);
        this.client = HttpClient.newBuilder()
            .connectTimeout(Duration.ofSeconds(4))
            .build();
    }

    @Override
    public SendCoherence send(byte[] datum) {
        byte[] payload = applyInterims(datum);
        String d = digest(payload);
        try {
            HttpRequest req = HttpRequest.newBuilder(URI.create(url))
                .timeout(Duration.ofSeconds(4))
                .header("Content-Type", "application/octet-stream")
                .POST(HttpRequest.BodyPublishers.ofByteArray(payload))
                .build();
            HttpResponse<byte[]> resp = client.send(req, HttpResponse.BodyHandlers.ofByteArray());
            if (resp.body() != null && resp.body().length > 0) {
                responses.addLast(resp.body());
            }
            boolean ok = resp.statusCode() >= 200 && resp.statusCode() < 300;
            if (!ok) {
                note = "http status " + resp.statusCode();
            }
            return new SendCoherence(d, payload.length, ok ? payload.length : 0L, ok);
        } catch (Exception e) {
            note = "http bump: " + e.getMessage();
            return new SendCoherence(d, payload.length, 0L, false);
        }
    }

    @Override
    public Reception consume() {
        byte[] body = responses.pollFirst();
        if (body == null) {
            return Reception.absent();
        }
        byte[] out = applyInterims(body);
        return Reception.of(out, digest(out), ++sequence);
    }

    @Override
    public String observe() {
        return "http:pending=" + responses.size() + (latched ? ":latched" : "");
    }

    @Override
    public ChannelReceipt close() {
        responses.clear();
        return new ChannelReceipt(scheme(), address, note.isEmpty(), note);
    }
}
