package com.mearvk.sleela.audio.gui;

import java.nio.file.Path;
import java.util.List;

public interface SleelaAi {
    enum InputKind { DATA, FILE, AUDIO, VIDEO }
    enum Operation { INSPECT, CLASSIFY, EXTRACT, SUMMARIZE, TRANSFORM }

    record Input(InputKind kind, String name, Path path, byte[] data,
                 long timestampNs, long durationNs) {}

    record Request(Operation operation, String modelId, String instruction,
                   int maxObservations) {}

    record Observation(String label, double confidence,
                       long offsetNs, long durationNs) {}

    record Result(boolean accepted, boolean completed, long requestId,
                  List<Observation> observations, String summary) {}

    boolean validate(Input input, Request request);

    Result invoke(Input input, Request request);

    /**
     * Loads a declarative SLeeLa AI model/data contract.
     * XML is a model description; execution remains under SLeeLa.
     */
    void loadModelXml(Path xml);

    /**
     * Connects the presentation layer to the Standard SLeeLa VM.
     */
    void connectToVm();
}
