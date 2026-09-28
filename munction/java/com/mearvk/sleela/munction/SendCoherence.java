package com.mearvk.sleela.munction;

import java.util.Objects;

/**
 * The coherence accounting for one {@code send} on a Munction™ channel.
 *
 * <p>Coherent send (MUNCTION.md §2.3) means the bytes offered to a channel are
 * the bytes the channel accounts for, or the discrepancy is recorded. A
 * {@code SendCoherence} carries the datum's identity ({@link #datumDigest()}),
 * how many bytes were {@link #offered()}, and how many the channel
 * {@link #acknowledged()}. {@link #coherent()} is true only when the two match
 * (or the channel's framing otherwise proves full acceptance).
 *
 * <p>This record is immutable and is folded, per send, into the
 * {@link MunctionReceipt}.
 */
public final class SendCoherence {
    private final String datumDigest;
    private final long offered;
    private final long acknowledged;
    private final boolean coherent;

    public SendCoherence(String datumDigest, long offered, long acknowledged) {
        this(datumDigest, offered, acknowledged, offered == acknowledged);
    }

    public SendCoherence(String datumDigest, long offered, long acknowledged, boolean coherent) {
        this.datumDigest = Objects.requireNonNull(datumDigest, "datumDigest");
        if (offered < 0 || acknowledged < 0) {
            throw new IllegalArgumentException("byte counts must be non-negative");
        }
        this.offered = offered;
        this.acknowledged = acknowledged;
        this.coherent = coherent;
    }

    /** A content digest identifying the datum offered (e.g. a hash prefix). */
    public String datumDigest() {
        return datumDigest;
    }

    /** Number of bytes offered to the channel. */
    public long offered() {
        return offered;
    }

    /** Number of bytes the channel acknowledged accepting. */
    public long acknowledged() {
        return acknowledged;
    }

    /** True iff the send was fully accounted for. */
    public boolean coherent() {
        return coherent;
    }

    @Override
    public String toString() {
        return "send{digest=" + datumDigest + ", offered=" + offered
            + ", ack=" + acknowledged + ", coherent=" + coherent + "}";
    }
}
