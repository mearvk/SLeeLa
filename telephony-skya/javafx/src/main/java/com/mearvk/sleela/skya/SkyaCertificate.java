package com.mearvk.sleela.skya;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.Base64;

/**
 * A self-managed Skya certificate: a DSA-signed record binding a subject to a
 * public key for a bounded validity window (default 30 days). It is deliberately
 * NOT an X.509/CA certificate — Skya connections are peer-to-peer and
 * opportunistic, so this is a lightweight, portable, pure-JDK credential that
 * supports exactly what the feature needs: a time-boxed identity, renewal, and
 * DSA-signed authentication of the Diffie–Hellman exchange.
 *
 * <p>Design points (per the feature request):
 * <ul>
 *   <li>Each certificate is valid for a variable amount of time (~30 days).</li>
 *   <li>Certificates can be renewed; a newer connection presenting a fresh
 *       certificate supersedes an older one, and the GUI is prompted to update
 *       when a presented certificate is near or past expiry.</li>
 *   <li>Having no certificate is valid: connections fall back to plaintext and
 *       still function (see {@link SkyaCrypto}).</li>
 *   <li>DSA is used for the signature (small keys, cheap verify) over the DH
 *       public exchange and the certificate body itself.</li>
 * </ul>
 *
 * <p>Wire form (one line, pipe-delimited, Base64 fields):
 * {@code SKYACERT/1 subject=<b64> notBefore=<ms> notAfter=<ms> dsaPub=<b64> sig=<b64>}
 * where {@code sig} is SHA256withDSA over {@code subject|notBefore|notAfter|dsaPub}.
 */
public final class SkyaCertificate {
    public static final String WIRE_PREFIX = "SKYACERT/1";
    /** Default validity window: 30 days. */
    public static final long DEFAULT_VALIDITY_MS = 30L * 24 * 60 * 60 * 1000;
    /** Renew/prompt when within this window of expiry (5 days). */
    public static final long RENEW_WINDOW_MS = 5L * 24 * 60 * 60 * 1000;

    private final String subject;
    private final long notBefore;
    private final long notAfter;
    private final PublicKey dsaPublic;
    private final byte[] signature;
    // Present only for a certificate we own (self-issued); null for a peer's.
    private final PrivateKey dsaPrivate;

    private SkyaCertificate(String subject, long notBefore, long notAfter,
                            PublicKey dsaPublic, byte[] signature, PrivateKey dsaPrivate) {
        this.subject = subject;
        this.notBefore = notBefore;
        this.notAfter = notAfter;
        this.dsaPublic = dsaPublic;
        this.signature = signature;
        this.dsaPrivate = dsaPrivate;
    }

    /** Issue a fresh self-signed Skya certificate valid for the default window. */
    public static SkyaCertificate issue(String subject) throws SkyaCryptoException {
        return issue(subject, DEFAULT_VALIDITY_MS);
    }

    public static SkyaCertificate issue(String subject, long validityMs) throws SkyaCryptoException {
        try {
            KeyPairGenerator kpg = KeyPairGenerator.getInstance("DSA");
            kpg.initialize(2048);
            KeyPair kp = kpg.generateKeyPair();
            long now = System.currentTimeMillis();
            long nb = now, na = now + validityMs;
            byte[] sig = sign(kp.getPrivate(), body(subject, nb, na, kp.getPublic()));
            SkyaLog.info("crypto", "issued certificate subject=" + subject
                    + " validityDays=" + (validityMs / (24 * 60 * 60 * 1000)));
            return new SkyaCertificate(subject, nb, na, kp.getPublic(), sig, kp.getPrivate());
        } catch (Exception e) {
            throw new SkyaCryptoException("certificate issuance failed", e);
        }
    }

    /** True once we are within the renewal window of (or past) expiry. */
    public boolean needsRenewal() {
        return System.currentTimeMillis() >= (notAfter - RENEW_WINDOW_MS);
    }

    public boolean isExpired() { return System.currentTimeMillis() > notAfter; }
    public boolean isCurrentlyValid() {
        long now = System.currentTimeMillis();
        return now >= notBefore && now <= notAfter;
    }

    public long millisUntilExpiry() { return notAfter - System.currentTimeMillis(); }
    public long daysUntilExpiry() { return millisUntilExpiry() / (24L * 60 * 60 * 1000); }
    public String subject() { return subject; }
    public long notAfter() { return notAfter; }
    public boolean selfOwned() { return dsaPrivate != null; }

    /** Verify this certificate's own self-signature and validity window. */
    public boolean verifySelfSignature() {
        try {
            return verify(dsaPublic, body(subject, notBefore, notAfter, dsaPublic), signature);
        } catch (Exception e) {
            return false;
        }
    }

    /** Sign arbitrary bytes with this certificate's DSA key (self-owned only). */
    public byte[] sign(byte[] data) throws SkyaCryptoException {
        if (dsaPrivate == null) throw new SkyaCryptoException("cannot sign: certificate is not self-owned");
        try { return sign(dsaPrivate, data); }
        catch (Exception e) { throw new SkyaCryptoException("signing failed", e); }
    }

    /** Verify a DSA signature against this certificate's public key. */
    public boolean verify(byte[] data, byte[] sig) {
        try { return verify(dsaPublic, data, sig); }
        catch (Exception e) { return false; }
    }

    /** Serialize to the one-line wire form (never includes the private key). */
    public String toWire() {
        return WIRE_PREFIX
                + " subject=" + b64(subject.getBytes(StandardCharsets.UTF_8))
                + " notBefore=" + notBefore
                + " notAfter=" + notAfter
                + " dsaPub=" + b64(dsaPublic.getEncoded())
                + " sig=" + b64(signature);
    }

    /** Parse a peer certificate from its wire form (no private key). */
    public static SkyaCertificate fromWire(String line) throws SkyaCryptoException {
        try {
            String s = line.trim();
            if (!s.startsWith(WIRE_PREFIX)) throw new SkyaCryptoException("not a SKYACERT line");
            String subject = null, dsaPubB64 = null, sigB64 = null;
            long nb = 0, na = 0;
            for (String tok : s.substring(WIRE_PREFIX.length()).trim().split("\\s+")) {
                int eq = tok.indexOf('=');
                if (eq < 0) continue;
                String k = tok.substring(0, eq), v = tok.substring(eq + 1);
                switch (k) {
                    case "subject" -> subject = new String(unb64(v), StandardCharsets.UTF_8);
                    case "notBefore" -> nb = Long.parseLong(v);
                    case "notAfter" -> na = Long.parseLong(v);
                    case "dsaPub" -> dsaPubB64 = v;
                    case "sig" -> sigB64 = v;
                    default -> { /* forward-compatible: ignore unknown fields */ }
                }
            }
            if (subject == null || dsaPubB64 == null || sigB64 == null)
                throw new SkyaCryptoException("certificate missing required fields");
            PublicKey pub = KeyFactory.getInstance("DSA")
                    .generatePublic(new X509EncodedKeySpec(unb64(dsaPubB64)));
            return new SkyaCertificate(subject, nb, na, pub, unb64(sigB64), null);
        } catch (SkyaCryptoException e) {
            throw e;
        } catch (Exception e) {
            throw new SkyaCryptoException("certificate parse failed", e);
        }
    }

    /** Persist a self-owned certificate (public body + private key) to a file. */
    public void save(Path path) throws SkyaCryptoException {
        if (dsaPrivate == null) throw new SkyaCryptoException("cannot save: not self-owned");
        try {
            if (path.getParent() != null) Files.createDirectories(path.getParent());
            String out = toWire() + "\n"
                    + "dsaPriv=" + b64(new PKCS8EncodedKeySpec(dsaPrivate.getEncoded()).getEncoded()) + "\n";
            Files.writeString(path, out, StandardCharsets.UTF_8);
        } catch (IOException e) {
            throw new SkyaCryptoException("certificate save failed", e);
        }
    }

    /** Load a self-owned certificate previously saved with {@link #save}. */
    public static SkyaCertificate load(Path path) throws SkyaCryptoException {
        try {
            String[] lines = Files.readString(path, StandardCharsets.UTF_8).split("\n");
            SkyaCertificate pubOnly = fromWire(lines[0]);
            PrivateKey priv = null;
            for (String l : lines) {
                if (l.startsWith("dsaPriv=")) {
                    priv = KeyFactory.getInstance("DSA")
                            .generatePrivate(new PKCS8EncodedKeySpec(unb64(l.substring("dsaPriv=".length()))));
                }
            }
            return new SkyaCertificate(pubOnly.subject, pubOnly.notBefore, pubOnly.notAfter,
                    pubOnly.dsaPublic, pubOnly.signature, priv);
        } catch (SkyaCryptoException e) {
            throw e;
        } catch (Exception e) {
            throw new SkyaCryptoException("certificate load failed", e);
        }
    }

    public PublicKey dsaPublic() { return dsaPublic; }

    private static byte[] body(String subject, long nb, long na, PublicKey pub) {
        return (subject + "|" + nb + "|" + na + "|" + b64(pub.getEncoded()))
                .getBytes(StandardCharsets.UTF_8);
    }

    private static byte[] sign(PrivateKey key, byte[] data) throws Exception {
        Signature s = Signature.getInstance("SHA256withDSA");
        s.initSign(key); s.update(data); return s.sign();
    }

    private static boolean verify(PublicKey key, byte[] data, byte[] sig) throws Exception {
        Signature s = Signature.getInstance("SHA256withDSA");
        s.initVerify(key); s.update(data); return s.verify(sig);
    }

    private static String b64(byte[] b) { return Base64.getEncoder().encodeToString(b); }
    private static byte[] unb64(String s) { return Base64.getDecoder().decode(s); }
}
