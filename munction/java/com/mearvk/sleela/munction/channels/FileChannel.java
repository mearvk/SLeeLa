package com.mearvk.sleela.munction.channels;

import com.mearvk.sleela.munction.ChannelReceipt;
import com.mearvk.sleela.munction.Reception;
import com.mearvk.sleela.munction.SendCoherence;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

/**
 * File I/O channel ({@code file:} scheme, MUNCTION.md §4). {@code send} appends
 * the datum to the file and reports coherent byte accounting; {@code consume}
 * reads the file's current contents back as one reception. Backed by ordinary
 * file I/O so the reach is real, not simulated.
 */
public final class FileChannel extends AbstractChannel {

    private Path path;
    private long bytesWritten = 0L;
    private boolean consumed = false;
    private String note = "";

    @Override
    public String scheme() {
        return "file";
    }

    @Override
    public void open(String address) {
        this.address = address;
        this.path = Path.of(address);
        try {
            if (path.getParent() != null) {
                Files.createDirectories(path.getParent());
            }
            if (!Files.exists(path)) {
                Files.createFile(path);
            }
        } catch (IOException e) {
            throw new IllegalStateException("file open failed: " + e.getMessage(), e);
        }
    }

    @Override
    public SendCoherence send(byte[] datum) {
        byte[] payload = applyInterims(datum);
        String d = digest(payload);
        try {
            Files.write(path, payload, StandardOpenOption.WRITE, StandardOpenOption.APPEND);
            bytesWritten += payload.length;
            return new SendCoherence(d, payload.length, payload.length, true);
        } catch (IOException e) {
            note = "write failed: " + e.getMessage();
            return new SendCoherence(d, payload.length, 0L, false);
        }
    }

    @Override
    public Reception consume() {
        if (consumed) {
            return Reception.absent();
        }
        consumed = true;
        try {
            byte[] all = Files.readAllBytes(path);
            byte[] out = applyInterims(all);
            return Reception.of(out, digest(out), ++sequence);
        } catch (IOException e) {
            note = "read failed: " + e.getMessage();
            return Reception.absent();
        }
    }

    @Override
    public ChannelReceipt close() {
        // Files are flushed by each write; nothing to release beyond the path.
        return new ChannelReceipt(scheme(), address, note.isEmpty(), note);
    }
}
