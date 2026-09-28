package com.mearvk.sleela.munction;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Small, dependency-free content-digest helper used for coherent-send
 * accounting and reception identity. A digest names a datum without carrying
 * its meaning, so it is safe to place in a receipt.
 */
final class MunctionDigest {
    private MunctionDigest() {
    }

    /** A short, stable content fingerprint: {@code sha256:<first 16 hex>}. */
    static String of(byte[] data) {
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
            // SHA-256 is guaranteed by the platform; fall back defensively.
            return "len:" + data.length;
        }
    }
}
