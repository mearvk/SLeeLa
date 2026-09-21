package com.mearvk.sleela.munction;

import java.time.Instant;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;

/**
 * The immutable, witnessed record a Munction™ hands back from
 * {@code closeWithReceipt()} (MUNCTION.md §6) — the durable proof of
 * receivability. A receipt never claims a send that was not acknowledged nor a
 * reception that did not arrive: it is the protected structure of the Reach.
 */
public final class MunctionReceipt {

    /** The outcome of a Munction™ sentence. */
    public enum Outcome {
        /** Completed and latched, or closed cleanly. */
        REACHED,
        /** A bump stopped propagation at the boundary; what was reached is real. */
        CONTAINED,
        /** Retired early; the receipt records the residual (no fictional rollback). */
        ABORTED
    }

    private final String name;
    private final String scheme;
    private final String address;
    private final List<SendCoherence> sent;
    private final List<Reception> received;
    private final List<String> interims;
    private final boolean latched;
    private final Outcome outcome;
    private final List<String> bumps;
    private final String witness;
    private final Instant startedAt;
    private final Instant closedAt;

    MunctionReceipt(Builder b) {
        this.name = b.name;
        this.scheme = b.scheme;
        this.address = b.address;
        this.sent = List.copyOf(b.sent);
        this.received = List.copyOf(b.received);
        this.interims = List.copyOf(b.interims);
        this.latched = b.latched;
        this.outcome = b.outcome;
        this.bumps = List.copyOf(b.bumps);
        this.witness = b.witness;
        this.startedAt = b.startedAt;
        this.closedAt = b.closedAt;
    }

    public String name() { return name; }
    public String scheme() { return scheme; }
    public String address() { return address; }
    public List<SendCoherence> sent() { return sent; }
    public List<Reception> received() { return received; }
    public List<String> interims() { return interims; }
    public boolean latched() { return latched; }
    public Outcome outcome() { return outcome; }
    public List<String> bumps() { return bumps; }
    public String witness() { return witness; }
    public Instant startedAt() { return startedAt; }
    public Instant closedAt() { return closedAt; }

    /** True when every recorded send was coherent and no bump was noted. */
    public boolean fullyCoherent() {
        return bumps.isEmpty() && sent.stream().allMatch(SendCoherence::coherent);
    }

    @Override
    public String toString() {
        return "MunctionReceipt{name=" + name + ", scheme=" + scheme
            + ", address=" + address + ", outcome=" + outcome
            + ", sent=" + sent.size() + ", received=" + received.size()
            + ", interims=" + interims + ", latched=" + latched
            + ", bumps=" + bumps + ", witness=" + witness + "}";
    }

    /** Mutable accumulator the {@link Munction} engine fills as the reach runs. */
    static final class Builder {
        private String name = "";
        private String scheme = "";
        private String address = "";
        private final List<SendCoherence> sent = new ArrayList<>();
        private final List<Reception> received = new ArrayList<>();
        private final List<String> interims = new ArrayList<>();
        private boolean latched;
        private Outcome outcome = Outcome.REACHED;
        private final List<String> bumps = new ArrayList<>();
        private String witness = "";
        private final Instant startedAt = Instant.now();
        private Instant closedAt;

        Builder name(String n) { this.name = n; return this; }
        Builder scheme(String s) { this.scheme = s; return this; }
        Builder address(String a) { this.address = a; return this; }
        Builder send(SendCoherence c) { sent.add(c); return this; }
        Builder receive(Reception r) { received.add(r); return this; }
        Builder interims(List<String> names) { interims.addAll(names); return this; }
        Builder latched(boolean v) { this.latched = v; return this; }
        Builder outcome(Outcome o) { this.outcome = o; return this; }
        Builder bump(String b) { if (b != null && !b.isEmpty()) bumps.add(b); return this; }
        Builder witness(String w) { this.witness = w; return this; }

        List<String> bumpsView() { return Collections.unmodifiableList(bumps); }

        MunctionReceipt build() {
            Objects.requireNonNull(outcome, "outcome");
            this.closedAt = Instant.now();
            return new MunctionReceipt(this);
        }
    }
}
