package com.mearvk.sleela.munction.channels;

import com.mearvk.sleela.munction.ChannelReceipt;
import com.mearvk.sleela.munction.Interims;
import com.mearvk.sleela.munction.MunctionChannel;
import com.mearvk.sleela.munction.MunctionChannels;
import com.mearvk.sleela.munction.Reception;
import com.mearvk.sleela.munction.SendCoherence;

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.Arrays;

/**
 * Crypto envelope channel ({@code crypto:} scheme, MUNCTION.md §4.2). It does
 * not name a transport; it <b>wraps</b> one. {@code connect("crypto:tcp://…")}
 * binds the inner channel (here {@code tcp}) inside this envelope, which
 * <b>seals</b> each {@code send} before it reaches the inner channel and
 * <b>opens</b> each {@code consume} after the inner channel returns.
 *
 * <p>The seal is authenticated: a per-datum tag is appended and verified on open,
 * so a tampered or truncated payload fails to open (a bump recorded in the
 * receipt) rather than being silently accepted as plaintext. Key material is
 * never in the URI; a key reference is supplied out of band via
 * {@code enable("crypto:<keyref>")} and resolved by the host. When no key
 * reference has been resolved, the envelope uses a deterministic, clearly-marked
 * development seal so the reach still demonstrably seals/opens end to end.
 */
public final class CryptoChannel implements MunctionChannel {

    private MunctionChannel inner;
    private String address = "";
    private byte[] key = deriveKey("crypto:dev-seal"); // replaced by a resolved keyref
    private String note = "";
    private long sequence = 0L;
    private boolean latched = false;

    @Override
    public String scheme() {
        return "crypto";
    }

    @Override
    public void open(String address) {
        // addressOf("crypto:tcp://host:port") -> "tcp://host:port" (inner URI).
        this.address = address;
        String innerUri = address;
        this.inner = MunctionChannels.resolve(innerUri);
        this.inner.open(MunctionChannels.addressOf(innerUri));
    }

    /** Supply a resolved key reference (from {@code enable("crypto:<keyref>")}). */
    public void useKey(String keyRef) {
        if (keyRef != null && !keyRef.isEmpty()) {
            this.key = deriveKey(keyRef);
        }
    }

    @Override
    public SendCoherence send(byte[] datum) {
        byte[] sealed = seal(datum);
        SendCoherence innerCoherence = inner.send(sealed);
        // Coherence is reported over the ORIGINAL datum; the seal tag is part of
        // the accounting, so a bump on the inner channel surfaces here too.
        boolean coherent = innerCoherence.coherent();
        if (!coherent) {
            note = "inner send incoherent";
        }
        return new SendCoherence(digest(datum), datum.length,
            coherent ? datum.length : 0L, coherent);
    }

    @Override
    public void thatch(Interims interims) {
        inner.thatch(interims);
    }

    @Override
    public Reception consume() {
        Reception r = inner.consume();
        if (!r.present()) {
            return Reception.absent();
        }
        byte[] opened = open(r.datum());
        if (opened == null) {
            note = "seal verification bump at seq " + (sequence + 1);
            return Reception.absent();
        }
        return Reception.of(opened, digest(opened), ++sequence);
    }

    @Override
    public String observe() {
        return "crypto[" + (inner == null ? "?" : inner.observe()) + "]" + (latched ? ":latched" : "");
    }

    @Override
    public void latch() {
        this.latched = true;
        if (inner != null) {
            inner.latch();
        }
    }

    @Override
    public ChannelReceipt close() {
        ChannelReceipt innerReceipt = inner == null ? null : inner.close();
        boolean clean = note.isEmpty() && (innerReceipt == null || innerReceipt.clean());
        String combined = note;
        if (innerReceipt != null && !innerReceipt.note().isEmpty()) {
            combined = combined.isEmpty() ? "inner: " + innerReceipt.note()
                : combined + "; inner: " + innerReceipt.note();
        }
        return new ChannelReceipt(scheme(), address, clean, combined);
    }

    // ---- authenticated seal --------------------------------------------------

    /**
     * Seal = keystream-masked payload || tag(key, masked). Authenticated: any
     * change to the sealed bytes changes the tag check on open.
     */
    private byte[] seal(byte[] plaintext) {
        byte[] masked = mask(plaintext);
        byte[] tag = tag(masked);
        byte[] out = new byte[masked.length + tag.length];
        System.arraycopy(masked, 0, out, 0, masked.length);
        System.arraycopy(tag, 0, out, masked.length, tag.length);
        return out;
    }

    /** Open = verify tag, then unmask. Null when the tag does not verify. */
    private byte[] open(byte[] sealed) {
        if (sealed.length < 32) {
            return null;
        }
        int split = sealed.length - 32;
        byte[] masked = Arrays.copyOfRange(sealed, 0, split);
        byte[] tag = Arrays.copyOfRange(sealed, split, sealed.length);
        if (!MessageDigest.isEqual(tag, tag(masked))) {
            return null;
        }
        return mask(masked); // masking is its own inverse (keystream XOR)
    }

    /** Keystream XOR over the payload (symmetric; its own inverse). */
    private byte[] mask(byte[] data) {
        byte[] ks = keystream(data.length);
        byte[] out = new byte[data.length];
        for (int i = 0; i < data.length; i++) {
            out[i] = (byte) (data[i] ^ ks[i]);
        }
        return out;
    }

    private byte[] keystream(int len) {
        byte[] ks = new byte[len];
        int produced = 0;
        long counter = 0;
        try {
            while (produced < len) {
                MessageDigest md = MessageDigest.getInstance("SHA-256");
                md.update(key);
                md.update((byte) (counter >>> 24));
                md.update((byte) (counter >>> 16));
                md.update((byte) (counter >>> 8));
                md.update((byte) counter);
                byte[] block = md.digest();
                int take = Math.min(block.length, len - produced);
                System.arraycopy(block, 0, ks, produced, take);
                produced += take;
                counter++;
            }
        } catch (Exception e) {
            // SHA-256 is guaranteed; a zero keystream would still round-trip.
        }
        return ks;
    }

    private byte[] tag(byte[] masked) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            md.update(key);
            md.update("tag".getBytes(StandardCharsets.UTF_8));
            md.update(masked);
            return md.digest();
        } catch (Exception e) {
            return new byte[32];
        }
    }

    private static byte[] deriveKey(String keyRef) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            md.update("munction-key".getBytes(StandardCharsets.UTF_8));
            md.update(keyRef.getBytes(StandardCharsets.UTF_8));
            return md.digest();
        } catch (Exception e) {
            return new byte[32];
        }
    }

    private static String digest(byte[] data) {
        if (data == null) {
            return "sha256:null";
        }
        try {
            byte[] h = MessageDigest.getInstance("SHA-256").digest(data);
            StringBuilder sb = new StringBuilder("sha256:");
            for (int i = 0; i < 8 && i < h.length; i++) {
                sb.append(Character.forDigit((h[i] >> 4) & 0xF, 16));
                sb.append(Character.forDigit(h[i] & 0xF, 16));
            }
            return sb.toString();
        } catch (Exception e) {
            return "len:" + data.length;
        }
    }
}
