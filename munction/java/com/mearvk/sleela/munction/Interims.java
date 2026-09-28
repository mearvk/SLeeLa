package com.mearvk.sleela.munction;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.function.UnaryOperator;

/**
 * The interim stages a Munction™ {@code thatch} weaves over the reach
 * (MUNCTION.md §3). Interims are ordered, named transforms applied to the datum
 * as it flows through the covered part of the path — a thatched roof over the
 * reach. Each stage is a pure {@code byte[] -> byte[]} step (e.g. compress,
 * frame, checksum); ordering is preserved.
 *
 * <p>Interims are declared once and handed to {@code thatch(...)}; the channel
 * runs them, in order, over the datum it is about to move or has just received.
 */
public final class Interims {
    /** name -> transform, in declaration order. */
    private final Map<String, UnaryOperator<byte[]>> stages = new LinkedHashMap<>();

    /** An empty thatch (a covered path that transforms nothing). */
    public static Interims none() {
        return new Interims();
    }

    public static Interims of(String name, UnaryOperator<byte[]> stage) {
        return new Interims().stage(name, stage);
    }

    /** Append an interim stage; later stages run after earlier ones. */
    public Interims stage(String name, UnaryOperator<byte[]> stage) {
        stages.put(Objects.requireNonNull(name, "name"), Objects.requireNonNull(stage, "stage"));
        return this;
    }

    /** The interim stage names, in order. */
    public List<String> names() {
        return Collections.unmodifiableList(new ArrayList<>(stages.keySet()));
    }

    /** Number of interim stages. */
    public int size() {
        return stages.size();
    }

    /** Run every interim stage over {@code data}, in declaration order. */
    public byte[] apply(byte[] data) {
        byte[] out = data;
        for (UnaryOperator<byte[]> stage : stages.values()) {
            out = stage.apply(out);
        }
        return out;
    }

    @Override
    public String toString() {
        return "interims" + names();
    }
}
