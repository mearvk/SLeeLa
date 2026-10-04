package com.mearvk.sleela.skya;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * Loads, issues, and renews the local Skya certificate.
 *
 * <p>Policy (per the feature request):
 * <ul>
 *   <li>The local certificate is stored under {@code ~/.sleela/skya/skya-cert.txt}
 *       (override with {@code SKYA_CERT_FILE}).</li>
 *   <li>On load, if there is no certificate, or it is expired/invalid, a fresh
 *       ~30-day certificate is issued and saved. Connections still work with no
 *       certificate, so issuance failures are non-fatal (we just run plaintext).</li>
 *   <li>{@link #needsRenewalPrompt()} is true when the certificate is within the
 *       renewal window; the GUI uses this to prompt the user to update.</li>
 *   <li>{@link #renew()} issues and persists a new certificate — used when the
 *       user accepts the prompt or a newer connection requires it.</li>
 * </ul>
 *
 * <p>Crypto is optional: {@link #certificate()} may be {@code null} if issuance
 * is disabled ({@code SKYA_CRYPTO=off}) or failed, and every caller treats a
 * null certificate as "run plaintext".
 */
public final class SkyaCertStore {
    private final String subject;
    private final Path path;
    private final boolean cryptoEnabled;
    private SkyaCertificate certificate;

    public SkyaCertStore(String subject) {
        this.subject = subject;
        this.cryptoEnabled = !"off".equalsIgnoreCase(SkyaConfig.envOr("SKYA_CRYPTO", "on"));
        this.path = resolvePath();
        this.certificate = cryptoEnabled ? loadOrIssue() : null;
        if (!cryptoEnabled) SkyaLog.info("crypto", "crypto disabled (SKYA_CRYPTO=off); connections run plaintext");
    }

    /** The current local certificate, or null if crypto is off/unavailable. */
    public synchronized SkyaCertificate certificate() { return certificate; }

    public boolean cryptoEnabled() { return cryptoEnabled; }

    /** True when the certificate exists and is within the renewal window. */
    public synchronized boolean needsRenewalPrompt() {
        return certificate != null && certificate.needsRenewal();
    }

    /** A short human-readable status for the GUI (e.g. "valid, 29 days"). */
    public synchronized String status() {
        if (!cryptoEnabled) return "crypto off (plaintext)";
        if (certificate == null) return "no certificate (plaintext fallback)";
        if (certificate.isExpired()) return "certificate EXPIRED — renew";
        if (certificate.needsRenewal()) return "certificate expiring in " + certificate.daysUntilExpiry() + " days — renew soon";
        return "certificate valid, " + certificate.daysUntilExpiry() + " days remaining";
    }

    /** Issue and persist a fresh certificate; returns it (or null on failure). */
    public synchronized SkyaCertificate renew() {
        try {
            SkyaCertificate fresh = SkyaCertificate.issue(subject);
            trySave(fresh);
            certificate = fresh;
            SkyaLog.info("crypto", "certificate renewed; valid " + fresh.daysUntilExpiry() + " days");
            return fresh;
        } catch (SkyaCryptoException e) {
            SkyaLog.warn("crypto", "certificate renewal failed: " + e.getMessage());
            return certificate;
        }
    }

    private SkyaCertificate loadOrIssue() {
        try {
            if (Files.isRegularFile(path)) {
                SkyaCertificate c = SkyaCertificate.load(path);
                if (c.verifySelfSignature() && !c.isExpired()) {
                    SkyaLog.info("crypto", "loaded certificate subject=" + c.subject()
                            + " (" + c.daysUntilExpiry() + " days remaining)");
                    return c;
                }
                SkyaLog.info("crypto", "stored certificate expired/invalid; issuing a fresh one");
            }
        } catch (SkyaCryptoException e) {
            SkyaLog.warn("crypto", "could not load certificate (" + e.getMessage() + "); issuing a fresh one");
        }
        try {
            SkyaCertificate fresh = SkyaCertificate.issue(subject);
            trySave(fresh);
            return fresh;
        } catch (SkyaCryptoException e) {
            SkyaLog.warn("crypto", "certificate issuance failed (" + e.getMessage() + "); running plaintext");
            return null;
        }
    }

    private void trySave(SkyaCertificate c) {
        try { c.save(path); }
        catch (SkyaCryptoException e) { SkyaLog.warn("crypto", "could not persist certificate: " + e.getMessage()); }
    }

    private static Path resolvePath() {
        String override = System.getenv("SKYA_CERT_FILE");
        if (override != null && !override.isBlank()) return Paths.get(override.trim());
        String home = System.getProperty("user.home", ".");
        return Paths.get(home, ".sleela", "skya", "skya-cert.txt");
    }
}
