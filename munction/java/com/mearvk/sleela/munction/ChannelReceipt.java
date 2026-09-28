package com.mearvk.sleela.munction;

import java.util.Objects;

/**
 * A channel's contribution to the Munction™ receipt, produced by
 * {@link MunctionChannel#close()} (MUNCTION.md §5). It records the channel's own
 * view of the reach: its scheme, resolved address (never secrets), and whether
 * it closed cleanly.
 */
public final class ChannelReceipt {
    private final String scheme;
    private final String address;
    private final boolean clean;
    private final String note;

    public ChannelReceipt(String scheme, String address, boolean clean, String note) {
        this.scheme = Objects.requireNonNull(scheme, "scheme");
        this.address = Objects.requireNonNull(address, "address");
        this.clean = clean;
        this.note = note == null ? "" : note;
    }

    public String scheme() {
        return scheme;
    }

    public String address() {
        return address;
    }

    /** True when the channel released its OS resource without a residual fault. */
    public boolean clean() {
        return clean;
    }

    /** A short human note (a bump or residual fact); empty when none. */
    public String note() {
        return note;
    }

    @Override
    public String toString() {
        return "channel{" + scheme + "://" + address + ", clean=" + clean
            + (note.isEmpty() ? "" : ", note=" + note) + "}";
    }
}
