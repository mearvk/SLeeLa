package com.mearvk.sleela.munction;

import com.mearvk.sleela.munction.channels.CryptoChannel;

import java.nio.charset.StandardCharsets;
import java.util.Objects;

/**
 * Munction™ — the reach-composition fluent form (MUNCTION.md).
 *
 * <p>A Munction™ is one sane sentence of <em>reach</em>: it {@link #start(String)}s
 * an addressed reach, {@link #connect(String)}s over a system-method channel
 * (pipe, file, internet, private packet, or crypto — resolved from the URI
 * scheme), moves a datum through interim stages, {@link #consume()}s what
 * returns, {@link #latch()}es the reached state, and
 * {@link #closeWithReceipt()}s. The sentence is bounded to {@value #MIN_CALLS}
 * ..{@value #MAX_CALLS} calls and follows the BODI™ verb ladder; violating the
 * shape stops at the boundary rather than inventing meaning.
 *
 * <pre>{@code
 * MunctionReceipt r = Munction.start("xyx")
 *     .connect("sdps://tmcf:19866")
 *     .send(data)
 *     .thatch(interims)
 *     .consume()
 *     .consume()
 *     .latch()
 *     .closeWithReceipt();
 * }</pre>
 *
 * <p>Every Munction™ is receivable (it always yields a {@link MunctionReceipt})
 * and every {@link #send(byte[])} is coherent (accounted offered vs.
 * acknowledged bytes). The object is single-use: once closed it cannot be
 * reopened, matching a single sentence.
 */
public final class Munction {

    /** Fewest calls a sane reach sentence may have (name → connect → move → close). */
    public static final int MIN_CALLS = 4;
    /** Most calls one sane reach sentence may have; longer intent splits in two. */
    public static final int MAX_CALLS = 16;

    /** Phases a reach passes through, used to police the legal sequence. */
    private enum Phase { STARTED, CONNECTED, MOVING, CLOSED }

    private final String name;
    private final MunctionReceipt.Builder receipt = new MunctionReceipt.Builder();

    private Phase phase = Phase.STARTED;
    private MunctionChannel channel;
    private String scheme = "";
    private Interims thatched;
    private boolean latched;
    private int callCount;   // counts every chained call incl. opener + closer

    private Munction(String name) {
        this.name = Objects.requireNonNull(name, "name");
        this.callCount = 1; // start(...) is the first call
        receipt.name(name).witness(witness("register"));
    }

    // ---- opener --------------------------------------------------------------

    /** Open a named reach (BODI™ register+bind). The opener; exactly once, first. */
    public static Munction start(String name) {
        if (name == null || name.isEmpty()) {
            throw new IllegalArgumentException("Munction.start(name) requires a non-empty name");
        }
        return new Munction(name);
    }

    // ---- middles -------------------------------------------------------------

    /** Bind the reach to a channel resolved from the URI scheme (BODI™ connect). */
    public Munction connect(String uri) {
        step();
        if (phase != Phase.STARTED) {
            throw boundary("connect must immediately follow start");
        }
        this.scheme = MunctionChannels.schemeOf(uri);
        this.channel = MunctionChannels.resolve(uri);
        this.channel.open(MunctionChannels.addressOf(uri));
        receipt.scheme(scheme).address(safeAddress(uri)).witness(witness("connect"));
        phase = Phase.CONNECTED;
        return this;
    }

    /** Explicitly open the resource (implied by connect; optional, BODI™ open). */
    public Munction open() {
        step();
        requireConnectedNotMoving("open");
        receipt.witness(witness("open"));
        return this;
    }

    /** Permit an optional capability such as {@code retry} or {@code crypto:<keyref>}. */
    public Munction enable(String policy) {
        step();
        requireConnectedNotMoving("enable");
        Objects.requireNonNull(policy, "policy");
        // A crypto key reference is resolved out of band into the crypto channel;
        // the keyref (a reference, not a secret) is recorded, never the key bytes.
        if (policy.startsWith("crypto:") && channel instanceof CryptoChannel cc) {
            cc.useKey(policy.substring("crypto:".length()));
        }
        receipt.witness(witness("enable:" + policy));
        return this;
    }

    /** Coherently push a datum onto the channel (BODI™ push). */
    public Munction send(byte[] datum) {
        step();
        requireMovable("send");
        Objects.requireNonNull(datum, "datum");
        byte[] payload = thatched == null ? datum : thatched.apply(datum);
        SendCoherence coherence;
        try {
            coherence = channel.send(payload);
        } catch (RuntimeException e) {
            receipt.bump("send failed: " + e.getMessage());
            coherence = new SendCoherence(MunctionDigest.of(payload), payload.length, 0L, false);
        }
        if (!coherence.coherent()) {
            receipt.bump("incoherent send " + coherence.datumDigest());
        }
        receipt.send(coherence).witness(witness("send"));
        phase = Phase.MOVING;
        return this;
    }

    /** Convenience: coherently push a String datum (UTF-8). */
    public Munction send(String datum) {
        return send(Objects.requireNonNull(datum, "datum").getBytes(StandardCharsets.UTF_8));
    }

    /** Weave interim stages over the covered path (BODI™ attach). Precedes consume. */
    public Munction thatch(Interims interims) {
        step();
        requireMovable("thatch");
        this.thatched = Objects.requireNonNull(interims, "interims");
        channel.thatch(interims);
        receipt.interims(interims.names()).witness(witness("thatch"));
        phase = Phase.MOVING;
        return this;
    }

    /** Pull one available unit of what the channel returns (BODI™ pull). Repeatable. */
    public Munction consume() {
        step();
        requireMovable("consume");
        Reception reception;
        try {
            reception = channel.consume();
        } catch (RuntimeException e) {
            receipt.bump("consume failed: " + e.getMessage());
            reception = Reception.absent();
        }
        receipt.receive(reception).witness(witness("consume"));
        phase = Phase.MOVING;
        return this;
    }

    /** Record channel state into the witness without consuming (BODI™ observe). */
    public Munction observe() {
        step();
        requireMovable("observe");
        String state = channel.observe();
        receipt.witness(witness("observe:" + state));
        return this;
    }

    /** Carry the reach onward to a further endpoint (BODI™ propagate; degree ≤ 4). */
    public Munction propagate(String uri) {
        step();
        requireMovable("propagate");
        receipt.witness(witness("propagate:" + MunctionChannels.schemeOf(uri)));
        // Federated onward reach is recorded on the witness; the actual mirror
        // endpoint is bounded by the Constitution (degree ≤ 4) and handled by
        // the host's propagation policy, not fabricated here.
        return this;
    }

    /** Hold/commit the reached state so it survives the close (BODI™ commit). Once. */
    public Munction latch() {
        step();
        requireMovable("latch");
        if (latched) {
            throw boundary("latch may appear at most once");
        }
        channel.latch();
        latched = true;
        receipt.latched(true).witness(witness("latch"));
        return this;
    }

    // ---- closers -------------------------------------------------------------

    /** Preferred closer: orderly teardown that returns the receipt. */
    public MunctionReceipt closeWithReceipt() {
        step();
        requireInReach("closeWithReceipt");
        finish(latched ? MunctionReceipt.Outcome.REACHED : MunctionReceipt.Outcome.REACHED);
        return receipt.build();
    }

    /** Plain closer: orderly teardown, receipt discarded by the caller. */
    public MunctionReceipt close() {
        return closeWithReceipt();
    }

    /** Negative closer: retire the reach now; the receipt records the residual. */
    public MunctionReceipt abort() {
        step();
        requireInReach("abort");
        receipt.bump("aborted by caller").witness(witness("abort"));
        finish(MunctionReceipt.Outcome.ABORTED);
        return receipt.build();
    }

    // ---- internals -----------------------------------------------------------

    private void finish(MunctionReceipt.Outcome outcome) {
        // Enforce the sanity bound at the close of the sentence.
        if (callCount < MIN_CALLS || callCount > MAX_CALLS) {
            // Teardown still happens; the receipt records the boundary fact.
            receipt.bump("call count " + callCount + " outside " + MIN_CALLS + ".." + MAX_CALLS);
            outcome = MunctionReceipt.Outcome.CONTAINED;
        }
        MunctionReceipt.Outcome resolved = outcome;
        if (channel != null) {
            try {
                ChannelReceipt cr = channel.close();
                if (!cr.clean()) {
                    receipt.bump("channel close: " + cr.note());
                    if (resolved == MunctionReceipt.Outcome.REACHED) {
                        resolved = MunctionReceipt.Outcome.CONTAINED;
                    }
                }
            } catch (RuntimeException e) {
                receipt.bump("channel close failed: " + e.getMessage());
                resolved = MunctionReceipt.Outcome.CONTAINED;
            }
        }
        receipt.outcome(resolved).witness(witness("close"));
        phase = Phase.CLOSED;
    }

    /** Count a chained call and reject a call count already past the sane maximum. */
    private void step() {
        if (phase == Phase.CLOSED) {
            throw boundary("the sentence is already closed");
        }
        callCount++;
        if (callCount > MAX_CALLS) {
            // One over the maximum: the sentence is no longer sane. Stop here.
            throw boundary("more than " + MAX_CALLS + " calls in one Munction sentence");
        }
    }

    private void requireInReach(String verb) {
        if (phase == Phase.CLOSED) {
            throw boundary(verb + " after close");
        }
    }

    private void requireConnectedNotMoving(String verb) {
        if (phase != Phase.CONNECTED) {
            throw boundary(verb + " is only legal between connect and the first data verb");
        }
    }

    private void requireMovable(String verb) {
        if (phase == Phase.STARTED) {
            throw boundary(verb + " requires a connect first");
        }
        if (phase == Phase.CLOSED) {
            throw boundary(verb + " after close");
        }
    }

    private RuntimeException boundary(String why) {
        // A shape violation is a BODI™ boundary stop, recorded, not invented past.
        receipt.bump("boundary: " + why);
        return new IllegalStateException("Munction boundary: " + why);
    }

    private String witness(String verb) {
        // system → sequence → verb, man=operator; the address stays out of the
        // witness verb string (secrets never enter the witness).
        return "munction:" + name + "#" + callCount + ":" + verb;
    }

    private static String safeAddress(String uri) {
        // Keep host:port / path, never any query/user-info that could carry a secret.
        String addr = MunctionChannels.addressOf(uri);
        int q = addr.indexOf('?');
        if (q >= 0) {
            addr = addr.substring(0, q);
        }
        int at = addr.indexOf('@');
        if (at >= 0) {
            addr = addr.substring(at + 1);
        }
        return addr;
    }
}
