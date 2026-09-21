package com.mearvk.sleela.munction.channels;

import com.mearvk.sleela.munction.Interims;
import com.mearvk.sleela.munction.MunctionChannel;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Shared base for the built-in Munction™ channels. It holds the thatched
 * {@link Interims}, a sequence counter for receptions, and a digest helper, and
 * defaults {@code observe}/{@code latch} to receivable no-ops that subclasses
 * refine. Subclasses implement the system-method specifics of {@code open},
 * {@code send}, {@code consume}, and {@code close}.
 */
abstract class AbstractChannel implements MunctionChannel {

    protected String address = "";
    protected Interims interims = Interims.none();
    protected long sequence = 0L;
    protected boolean latched = false;

    @Override
    public void thatch(Interims interims) {
        this.interims = interims == null ? Interims.none() : interims;
    }

    @Override
    public String observe() {
        return scheme() + ":seq=" + sequence + (latched ? ":latched" : "");
    }

    @Override
    public void latch() {
        this.latched = true;
    }

    /** Apply the thatched interim stages to a datum about to be sent/just received. */
    protected byte[] applyInterims(byte[] data) {
        return interims.apply(data);
    }

    /** A short content fingerprint used for coherence and reception identity. */
    protected static String digest(byte[] data) {
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
        } catch (NoSuchAlgorithmException e) {
            return "len:" + data.length;
        }
    }
}
