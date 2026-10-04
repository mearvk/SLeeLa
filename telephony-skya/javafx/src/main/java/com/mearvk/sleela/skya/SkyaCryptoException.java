package com.mearvk.sleela.skya;

/** Checked failure in the Skya crypto/certificate layer. */
public class SkyaCryptoException extends Exception {
    public SkyaCryptoException(String message) { super(message); }
    public SkyaCryptoException(String message, Throwable cause) { super(message, cause); }
}
