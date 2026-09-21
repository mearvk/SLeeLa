package com.mearvk.sleela.munction;

import java.util.Objects;

/**
 * One unit received by a {@code consume} on a Munction™ channel (receivability,
 * MUNCTION.md §2.2 / §5).
 *
 * <p>A {@code Reception} is either <b>present</b> (it carries a {@link #datum()}
 * that actually arrived, with its {@link #digest()} and {@link #sequence()}) or
 * <b>absent</b> ({@link #present()} is false) at end-of-reach. A Munction™ never
 * fabricates a reception: an absent reception means the channel had nothing more
 * to hand back within its bound, and the receipt records exactly that.
 */
public final class Reception {
    private static final Reception ABSENT = new Reception(false, null, "", -1);

    private final boolean present;
    private final byte[] datum;
    private final String digest;
    private final long sequence;

    private Reception(boolean present, byte[] datum, String digest, long sequence) {
        this.present = present;
        this.datum = datum;
        this.digest = digest;
        this.sequence = sequence;
    }

    /** A present reception carrying the bytes that arrived. */
    public static Reception of(byte[] datum, String digest, long sequence) {
        Objects.requireNonNull(datum, "datum");
        Objects.requireNonNull(digest, "digest");
        return new Reception(true, datum.clone(), digest, sequence);
    }

    /** The end-of-reach marker: nothing more was received. */
    public static Reception absent() {
        return ABSENT;
    }

    /** True when this reception carries a datum that actually arrived. */
    public boolean present() {
        return present;
    }

    /** The received bytes (defensive copy); empty when absent. */
    public byte[] datum() {
        return present ? datum.clone() : new byte[0];
    }

    /** Content digest of the received datum; empty when absent. */
    public String digest() {
        return digest;
    }

    /** Monotonic sequence position of this reception; -1 when absent. */
    public long sequence() {
        return sequence;
    }

    @Override
    public String toString() {
        return present
            ? "reception{seq=" + sequence + ", digest=" + digest + ", bytes=" + datum.length + "}"
            : "reception{absent}";
    }
}
