package com.mearvk.sleela.synchro;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.Map;

/**
 * Language integration boundary for Java callers and the SLeeLa host/VM.
 *
 * The host only needs to provide destinations and timing policy. Wire-format,
 * statistics, loss accounting, and SLA evaluation remain inside Synchro.
 */
public final class SynchroIntegration {
    private final UdpDispatcher dispatcher;

    public SynchroIntegration(InetSocketAddress[] destinations) {
        this.dispatcher = new UdpDispatcher(destinations);
    }

    public SynchroIntegration(InetSocketAddress[] destinations,
                              int payloadBytes, int timeoutMs, int window) {
        this.dispatcher = new UdpDispatcher(
            destinations, payloadBytes, timeoutMs, window);
    }

    public Map<String, SynchroStats> probe(int rounds, long intervalMs)
        throws IOException {
        return dispatcher.run(rounds, intervalMs);
    }

    public SlaReporter.Result evaluate(double thresholdMs, double percentile)
        throws IOException {
        return SlaReporter.evaluate(dispatcher.run(0, 0),
                                    thresholdMs, percentile);
    }
}
