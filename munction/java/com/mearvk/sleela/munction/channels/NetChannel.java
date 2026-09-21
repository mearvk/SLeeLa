package com.mearvk.sleela.munction.channels;

import com.mearvk.sleela.munction.ChannelReceipt;
import com.mearvk.sleela.munction.Reception;
import com.mearvk.sleela.munction.SendCoherence;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;

/**
 * Internet (TCP) channel ({@code tcp:} / {@code net:} scheme, MUNCTION.md §4),
 * the Munction™ counterpart of the NETWORK.md six-primitive TCP layer
 * (connect / sockwrite / sockread / sockclose). {@code send} writes bytes and
 * accounts them (coherent send); {@code consume} performs one bounded read
 * (receivable). A network hatch or bump (connect refused, partial read, reset)
 * is recorded as a residual fact rather than crashing the reach.
 */
public final class NetChannel extends AbstractChannel {

    /** Bounded read, consistent with NETWORK.md's 4096-byte socket read. */
    private static final int READ_BOUND = 4096;
    /** Connect timeout so a bump cannot block the reach forever. */
    private static final int CONNECT_TIMEOUT_MS = 4000;
    private static final int READ_TIMEOUT_MS = 4000;

    private Socket socket;
    private OutputStream out;
    private InputStream in;
    private long bytesSent = 0L;
    private String note = "";

    @Override
    public String scheme() {
        return "tcp";
    }

    @Override
    public void open(String address) {
        this.address = address;
        String host = "127.0.0.1";
        int port = 0;
        int colon = address.lastIndexOf(':');
        if (colon > 0) {
            host = address.substring(0, colon);
            try {
                port = Integer.parseInt(address.substring(colon + 1).replaceAll("[^0-9].*$", ""));
            } catch (NumberFormatException ignored) {
                port = 0;
            }
        }
        try {
            socket = new Socket();
            socket.connect(new InetSocketAddress(host, port), CONNECT_TIMEOUT_MS);
            socket.setSoTimeout(READ_TIMEOUT_MS);
            out = socket.getOutputStream();
            in = socket.getInputStream();
        } catch (IOException e) {
            // A network hatch/bump at connect time: the reach is still shaped,
            // it simply has no live transport. Recorded, not thrown.
            note = "connect bump: " + e.getMessage();
            socket = null;
        }
    }

    @Override
    public SendCoherence send(byte[] datum) {
        byte[] payload = applyInterims(datum);
        String d = digest(payload);
        if (out == null) {
            note = note.isEmpty() ? "send with no live socket" : note;
            return new SendCoherence(d, payload.length, 0L, false);
        }
        try {
            out.write(payload);
            out.flush();
            bytesSent += payload.length;
            return new SendCoherence(d, payload.length, payload.length, true);
        } catch (IOException e) {
            note = "write bump: " + e.getMessage();
            return new SendCoherence(d, payload.length, 0L, false);
        }
    }

    @Override
    public Reception consume() {
        if (in == null) {
            return Reception.absent();
        }
        try {
            byte[] buf = new byte[READ_BOUND];
            int n = in.read(buf);
            if (n <= 0) {
                return Reception.absent();
            }
            byte[] got = new byte[n];
            System.arraycopy(buf, 0, got, 0, n);
            byte[] out2 = applyInterims(got);
            return Reception.of(out2, digest(out2), ++sequence);
        } catch (IOException e) {
            // A read bump (timeout/reset) ends this reception; not fabricated.
            note = "read bump: " + e.getMessage();
            return Reception.absent();
        }
    }

    @Override
    public String observe() {
        boolean live = socket != null && socket.isConnected() && !socket.isClosed();
        return "tcp:live=" + live + ":sent=" + bytesSent + (latched ? ":latched" : "");
    }

    @Override
    public ChannelReceipt close() {
        boolean clean = note.isEmpty();
        try {
            if (socket != null) {
                socket.close();
            }
        } catch (IOException e) {
            note = note.isEmpty() ? "close bump: " + e.getMessage() : note;
            clean = false;
        }
        return new ChannelReceipt(scheme(), address, clean, note);
    }
}
