package com.mearvk.sleela.munction.channels;

import com.mearvk.sleela.munction.ChannelReceipt;
import com.mearvk.sleela.munction.Reception;
import com.mearvk.sleela.munction.SendCoherence;

import java.util.ArrayDeque;
import java.util.Deque;

/**
 * OS pipe / FIFO channel ({@code pipe:} scheme, MUNCTION.md §4). Models a
 * block-framed pipe: each {@code send} enqueues one frame and each
 * {@code consume} dequeues one, so sends and receipts stay coherent and framed.
 * The in-process frame queue stands in for an anonymous pipe pair or a named
 * FIFO endpoint; the framing and receivability contract are identical.
 */
public final class PipeChannel extends AbstractChannel {

    private final Deque<byte[]> frames = new ArrayDeque<>();
    private long enqueued = 0L;

    @Override
    public String scheme() {
        return "pipe";
    }

    @Override
    public void open(String address) {
        this.address = address; // e.g. a FIFO path or a named pipe endpoint
    }

    @Override
    public SendCoherence send(byte[] datum) {
        byte[] payload = applyInterims(datum);
        frames.addLast(payload.clone());
        enqueued++;
        // A pipe frame is accepted whole; offered == acknowledged.
        return new SendCoherence(digest(payload), payload.length, payload.length, true);
    }

    @Override
    public Reception consume() {
        byte[] frame = frames.pollFirst();
        if (frame == null) {
            return Reception.absent();
        }
        return Reception.of(frame, digest(frame), ++sequence);
    }

    @Override
    public String observe() {
        return "pipe:frames=" + frames.size() + ":sent=" + enqueued + (latched ? ":latched" : "");
    }

    @Override
    public ChannelReceipt close() {
        String note = frames.isEmpty() ? "" : "drained " + frames.size() + " unconsumed frame(s)";
        frames.clear();
        return new ChannelReceipt(scheme(), address, true, note);
    }
}
