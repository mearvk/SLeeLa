package com.mearvk.sleela.munction;

/**
 * The one contract every Munction™ system-method channel implements
 * (MUNCTION.md §5), so a single Munction™ sentence reaches pipes, files, the
 * internet, private packets, and crypto uniformly.
 *
 * <p>A channel is <b>receivable</b> (its {@link #consume()} yields a truthful
 * {@link Reception} and its {@link #close()} yields a {@link ChannelReceipt})
 * and its {@link #send(byte[])} is <b>coherent</b> (it returns a
 * {@link SendCoherence} accounting offered vs. acknowledged bytes). A channel
 * that cannot satisfy this contract is not a Munction™ channel and must not be
 * registered as a system method.
 */
public interface MunctionChannel {

    /** The URI scheme this channel serves (e.g. {@code "sdps"}, {@code "file"}). */
    String scheme();

    /** Bind the underlying OS resource for {@code address} (host:port, path, …). */
    void open(String address);

    /** Coherently push a datum; returns the send's coherence accounting. */
    SendCoherence send(byte[] datum);

    /** Register the interim stages to weave over this channel's data flow. */
    void thatch(Interims interims);

    /** Pull one available unit; an absent {@link Reception} marks end-of-reach. */
    Reception consume();

    /** Record channel state without consuming; returns a short state summary. */
    String observe();

    /** Hold/commit the reached state so it survives the close. */
    void latch();

    /** Teardown; returns the channel's contribution to the receipt. */
    ChannelReceipt close();
}
