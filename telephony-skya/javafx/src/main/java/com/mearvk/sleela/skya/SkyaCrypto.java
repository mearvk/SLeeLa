package com.mearvk.sleela.skya;

import javax.crypto.Cipher;
import javax.crypto.KeyAgreement;
import javax.crypto.spec.GCMParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.nio.charset.StandardCharsets;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.MessageDigest;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.spec.X509EncodedKeySpec;
import java.util.Base64;

/**
 * Optional, opportunistic crypto for Skya connections.
 *
 * <p>The model is deliberately "crypto if we can, plaintext if we must": every
 * connection — client↔server and client↔client — MAY run a Diffie–Hellman
 * exchange to derive an AES-256-GCM session key, optionally authenticated by a
 * DSA signature from a {@link SkyaCertificate}. If either side has no
 * certificate, or declines/doesn't understand the crypto handshake, the
 * connection still functions in plaintext. Nothing here is required for a
 * connection to work.
 *
 * <p>Primitives (all pure-JDK, no external libraries):
 * <ul>
 *   <li><b>Diffie–Hellman</b> (2048-bit) for the initial key agreement.</li>
 *   <li><b>DSA</b> (SHA256withDSA) to authenticate the DH public value and the
 *       certificate — the "small key exchange" signing path.</li>
 *   <li><b>AES-256-GCM</b> session cipher keyed by SHA-256 of the DH secret.</li>
 * </ul>
 *
 * <h2>Handshake (line-oriented, optional)</h2>
 * <pre>
 *   A -> B : SKYACRYPTO/1 HELLO dh=&lt;b64 DH pub&gt; [cert=&lt;b64 of the SKYACERT line&gt;] [sig=&lt;b64 DSA over dh&gt;]
 *   B -> A : SKYACRYPTO/1 HELLO dh=&lt;b64 DH pub&gt; [cert=...] [sig=...]
 * </pre>
 * Both sides then derive the same session key. A peer that replies with
 * {@code SKYACRYPTO/1 PLAIN} (or anything non-crypto) signals plaintext; the
 * caller proceeds unencrypted. Certificate/sig are optional: a missing cert
 * yields an unauthenticated (but still encrypted) session.
 */
public final class SkyaCrypto {
    public static final String WIRE_PREFIX = "SKYACRYPTO/1";
    private static final int DH_BITS = 2048;
    private static final int GCM_TAG_BITS = 128;
    private static final int IV_BYTES = 12;
    private static final SecureRandom RNG = new SecureRandom();

    private final KeyPair dhKeyPair;

    private SkyaCrypto(KeyPair dhKeyPair) { this.dhKeyPair = dhKeyPair; }

    /** Create a fresh ephemeral DH keypair for one connection. */
    public static SkyaCrypto newEphemeral() throws SkyaCryptoException {
        try {
            KeyPairGenerator kpg = KeyPairGenerator.getInstance("DH");
            kpg.initialize(DH_BITS);
            return new SkyaCrypto(kpg.generateKeyPair());
        } catch (Exception e) {
            throw new SkyaCryptoException("DH keypair generation failed", e);
        }
    }

    /** Our DH public value, Base64-encoded, for the HELLO line. */
    public String dhPublicB64() {
        return Base64.getEncoder().encodeToString(dhKeyPair.getPublic().getEncoded());
    }

    /**
     * Build our HELLO line. If {@code cert} is non-null and self-owned, the DH
     * public value is DSA-signed and the certificate is attached, so the peer
     * can authenticate us. With no certificate the HELLO is still valid — just
     * unauthenticated (encryption without identity).
     */
    public String hello(SkyaCertificate cert) {
        StringBuilder sb = new StringBuilder(WIRE_PREFIX).append(" HELLO dh=").append(dhPublicB64());
        if (cert != null && cert.selfOwned()) {
            try {
                byte[] sig = cert.sign(dhKeyPair.getPublic().getEncoded());
                // Base64 the whole certificate line as one token so it carries
                // no spaces (its own fields are space-delimited and its Base64
                // bodies may contain '+'/'/').
                String certB64 = Base64.getEncoder().encodeToString(
                        cert.toWire().getBytes(StandardCharsets.UTF_8));
                sb.append(" cert=").append(certB64);
                sb.append(" sig=").append(Base64.getEncoder().encodeToString(sig));
            } catch (SkyaCryptoException e) {
                SkyaLog.warn("crypto", "could not attach certificate to HELLO: " + e.getMessage());
            }
        }
        return sb.toString();
    }

    /** The plaintext-fallback signal a peer may send instead of a HELLO. */
    public static String plain() { return WIRE_PREFIX + " PLAIN"; }

    /** True if a received line is a Skya crypto HELLO we can act on. */
    public static boolean isHello(String line) {
        return line != null && line.trim().startsWith(WIRE_PREFIX + " HELLO");
    }

    /**
     * Complete the handshake against the peer's HELLO line and return the
     * negotiated session (encrypt/decrypt + the peer's authenticated identity,
     * if any). Returns {@code null} if the peer did not send a usable HELLO
     * (caller then continues in plaintext).
     */
    public SkyaSession accept(String peerHello) throws SkyaCryptoException {
        if (!isHello(peerHello)) return null;
        String peerDhB64 = null, certWire = null, sigB64 = null;
        for (String tok : peerHello.trim().substring((WIRE_PREFIX + " HELLO").length()).trim().split("\\s+")) {
            int eq = tok.indexOf('=');
            if (eq < 0) continue;
            String k = tok.substring(0, eq), v = tok.substring(eq + 1);
            switch (k) {
                case "dh" -> peerDhB64 = v;
                case "cert" -> certWire = new String(Base64.getDecoder().decode(v), StandardCharsets.UTF_8);
                case "sig" -> sigB64 = v;
                default -> { }
            }
        }
        if (peerDhB64 == null) return null;
        try {
            PublicKey peerDh = KeyFactory.getInstance("DH")
                    .generatePublic(new X509EncodedKeySpec(Base64.getDecoder().decode(peerDhB64)));
            KeyAgreement ka = KeyAgreement.getInstance("DH");
            ka.init(dhKeyPair.getPrivate());
            ka.doPhase(peerDh, true);
            byte[] secret = ka.generateSecret();
            byte[] sessionKey = MessageDigest.getInstance("SHA-256").digest(secret); // 32 bytes = AES-256

            // Optional peer authentication: verify the DSA sig over the peer's
            // DH public value using the attached certificate.
            SkyaCertificate peerCert = null;
            boolean authenticated = false;
            if (certWire != null) {
                peerCert = SkyaCertificate.fromWire(certWire);
                boolean certOk = peerCert.verifySelfSignature() && peerCert.isCurrentlyValid();
                if (certOk && sigB64 != null) {
                    authenticated = peerCert.verify(peerDh.getEncoded(), Base64.getDecoder().decode(sigB64));
                }
                if (!certOk) {
                    SkyaLog.warn("crypto", "peer certificate invalid or expired (subject="
                            + peerCert.subject() + "); session encrypted but unauthenticated");
                }
            }
            SkyaLog.info("crypto", "session established: encrypted=true authenticated=" + authenticated
                    + (peerCert != null ? " peer=" + peerCert.subject() : ""));
            return new SkyaSession(sessionKey, authenticated, peerCert);
        } catch (SkyaCryptoException e) {
            throw e;
        } catch (Exception e) {
            throw new SkyaCryptoException("DH handshake failed", e);
        }
    }

    /** A negotiated encrypted session: AES-256-GCM both ways + peer identity. */
    public static final class SkyaSession {
        private final SecretKeySpec key;
        private final boolean authenticated;
        private final SkyaCertificate peerCertificate;

        SkyaSession(byte[] key32, boolean authenticated, SkyaCertificate peerCertificate) {
            this.key = new SecretKeySpec(key32, "AES");
            this.authenticated = authenticated;
            this.peerCertificate = peerCertificate;
        }

        public boolean isAuthenticated() { return authenticated; }
        public SkyaCertificate peerCertificate() { return peerCertificate; }

        /** Encrypt a UTF-8 string -> Base64(iv || ciphertext+tag). */
        public String encrypt(String plaintext) throws SkyaCryptoException {
            try {
                byte[] iv = new byte[IV_BYTES];
                RNG.nextBytes(iv);
                Cipher c = Cipher.getInstance("AES/GCM/NoPadding");
                c.init(Cipher.ENCRYPT_MODE, key, new GCMParameterSpec(GCM_TAG_BITS, iv));
                byte[] ct = c.doFinal(plaintext.getBytes(StandardCharsets.UTF_8));
                byte[] out = new byte[iv.length + ct.length];
                System.arraycopy(iv, 0, out, 0, iv.length);
                System.arraycopy(ct, 0, out, iv.length, ct.length);
                return Base64.getEncoder().encodeToString(out);
            } catch (Exception e) {
                throw new SkyaCryptoException("encrypt failed", e);
            }
        }

        /** Decrypt Base64(iv || ciphertext+tag) -> UTF-8 string. */
        public String decrypt(String b64) throws SkyaCryptoException {
            try {
                byte[] in = Base64.getDecoder().decode(b64);
                byte[] iv = java.util.Arrays.copyOfRange(in, 0, IV_BYTES);
                byte[] ct = java.util.Arrays.copyOfRange(in, IV_BYTES, in.length);
                Cipher c = Cipher.getInstance("AES/GCM/NoPadding");
                c.init(Cipher.DECRYPT_MODE, key, new GCMParameterSpec(GCM_TAG_BITS, iv));
                return new String(c.doFinal(ct), StandardCharsets.UTF_8);
            } catch (Exception e) {
                throw new SkyaCryptoException("decrypt failed", e);
            }
        }
    }
}
