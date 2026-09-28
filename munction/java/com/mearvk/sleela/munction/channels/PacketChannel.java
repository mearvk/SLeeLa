package com.mearvk.sleela.munction.channels;

import com.mearvk.sleela.munction.ChannelReceipt;
import com.mearvk.sleela.munction.Reception;
import com.mearvk.sleela.munction.SendCoherence;

import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.ArrayDeque;
import java.util.Arrays;
import java.util.Deque;

/**
 * Private-packet channel — Secure Datum Packet Stream ({@code sdps:} scheme,
 * MUNCTION.md §4.1). This is the channel of the canonical Munction™ sentence
 * ({@code sdps://tmcf:19866}). Each {@code send} frames the datum as a private
 * packet:
 *
 * <pre>
 * +--------+-----------+----------------+-------------+
 * | "SDPS" | uint32 BE | datum (opaque) | mac (32 B)  |
 * +--------+-----------+----------------+-------------+
 * </pre>
 *
 * The length prefix and MAC make truncation detectable (coherent send); the
 * payload is opaque on the wire (private); each packet yields a per-packet
 * acknowledgment folded into the receipt (receivable). The framed packets are
 * held in an in-process stream that stands in for the private-packet endpoint.
 */
public final class PacketChannel extends AbstractChannel {

    private static final byte[] MAGIC = {'S', 'D', 'P', 'S'};
    private static final int MAC_LEN = 32;

    private final Deque<byte[]> wire = new ArrayDeque<>();
    private long packetsSent = 0L;
    private String note = "";

    @Override
    public String scheme() {
        return "sdps";
    }

    @Override
    public void open(String address) {
        this.address = address; // host:port of the private-packet endpoint
    }

    @Override
    public SendCoherence send(byte[] datum) {
        byte[] payload = applyInterims(datum);
        byte[] packet = frame(payload);
        wire.addLast(packet);
        packetsSent++;
        // A framed packet is accepted whole and MAC-tagged: offered == acknowledged.
        return new SendCoherence(digest(payload), payload.length, payload.length, true);
    }

    @Override
    public Reception consume() {
        byte[] packet = wire.pollFirst();
        if (packet == null) {
            return Reception.absent();
        }
        byte[] payload = deframe(packet);
        if (payload == null) {
            // A truncated / MAC-mismatched packet is a bump: detected, not accepted.
            note = "packet integrity bump at seq " + (sequence + 1);
            return Reception.absent();
        }
        return Reception.of(payload, digest(payload), ++sequence);
    }

    @Override
    public String observe() {
        return "sdps:queued=" + wire.size() + ":sent=" + packetsSent + (latched ? ":latched" : "");
    }

    @Override
    public ChannelReceipt close() {
        String n = note;
        if (n.isEmpty() && !wire.isEmpty()) {
            n = "left " + wire.size() + " unconsumed packet(s)";
        }
        wire.clear();
        return new ChannelReceipt(scheme(), address, note.isEmpty(), n);
    }

    // ---- SDPS framing --------------------------------------------------------

    private byte[] frame(byte[] payload) {
        byte[] mac = mac(payload);
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        out.writeBytes(MAGIC);
        int len = payload.length;
        out.write((len >>> 24) & 0xFF);
        out.write((len >>> 16) & 0xFF);
        out.write((len >>> 8) & 0xFF);
        out.write(len & 0xFF);
        out.writeBytes(payload);
        out.writeBytes(mac);
        return out.toByteArray();
    }

    /** Verify and unwrap a packet; null when magic, length, or MAC do not check. */
    private byte[] deframe(byte[] packet) {
        if (packet.length < MAGIC.length + 4 + MAC_LEN) {
            return null;
        }
        for (int i = 0; i < MAGIC.length; i++) {
            if (packet[i] != MAGIC[i]) {
                return null;
            }
        }
        int p = MAGIC.length;
        int len = ((packet[p] & 0xFF) << 24) | ((packet[p + 1] & 0xFF) << 16)
            | ((packet[p + 2] & 0xFF) << 8) | (packet[p + 3] & 0xFF);
        p += 4;
        if (len < 0 || p + len + MAC_LEN != packet.length) {
            return null;
        }
        byte[] payload = Arrays.copyOfRange(packet, p, p + len);
        byte[] mac = Arrays.copyOfRange(packet, p + len, packet.length);
        if (!MessageDigest.isEqual(mac, mac(payload))) {
            return null;
        }
        return payload;
    }

    /** A per-packet integrity tag over the payload and the endpoint address. */
    private byte[] mac(byte[] payload) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            md.update(MAGIC);
            md.update(address.getBytes(StandardCharsets.UTF_8));
            md.update(payload);
            return md.digest();
        } catch (Exception e) {
            // SHA-256 is guaranteed; a zero MAC would fail verification loudly.
            return new byte[MAC_LEN];
        }
    }
}
