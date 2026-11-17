package org.sleela.site.runtime;

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.SecureRandom;
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;

/** Stateless anti-CSRF token for the runtime form: timestamp plus HMAC under a per-process secret. */
public final class FormToken {
    private static final long TTL_MS = 2L * 60 * 60 * 1000;
    private final byte[] secret = new byte[32];

    public FormToken() { new SecureRandom().nextBytes(secret); }

    public String issue() {
        String ts = Long.toString(System.currentTimeMillis(), 36);
        return ts + "." + mac(ts);
    }

    public boolean valid(String token) {
        if (token == null) return false;
        int dot = token.indexOf('.');
        if (dot < 1) return false;
        String ts = token.substring(0, dot);
        try {
            long t = Long.parseLong(ts, 36);
            long age = System.currentTimeMillis() - t;
            if (age < -60_000L || age > TTL_MS) return false;
        } catch (NumberFormatException e) {
            return false;
        }
        return MessageDigest.isEqual(mac(ts).getBytes(StandardCharsets.US_ASCII), token.substring(dot + 1).getBytes(StandardCharsets.US_ASCII));
    }

    private String mac(String data) {
        try {
            Mac m = Mac.getInstance("HmacSHA256");
            m.init(new SecretKeySpec(secret, "HmacSHA256"));
            byte[] d = m.doFinal(data.getBytes(StandardCharsets.UTF_8));
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < 16; i++) sb.append(String.format("%02x", d[i]));
            return sb.toString();
        } catch (java.security.GeneralSecurityException e) {
            throw new IllegalStateException(e);
        }
    }
}
