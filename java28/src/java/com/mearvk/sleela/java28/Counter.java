package com.mearvk.sleela.java28;

/**
 * Allow-listed demo object living on the Java 28 SecureJDK heap. Sleela never
 * holds this object — only a handle to it — and drives it via the link.
 */
public final class Counter {
    public long value;

    public Counter() { this.value = 0; }
    public Counter(long start) { this.value = start; }

    public long add(long delta) { value += delta; return value; }
    public long get() { return value; }
    public void reset() { value = 0; }
    public boolean isPositive() { return value > 0; }
    public String label(String prefix) { return prefix + "=" + value; }
}
