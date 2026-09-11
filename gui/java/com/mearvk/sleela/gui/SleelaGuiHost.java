package com.mearvk.sleela.gui;

import java.util.Objects;

/**
 * Connects a Java GUI backend to SLeeLa business logic.
 * Java owns presentation; SLeeLa owns operation meaning and state transitions.
 */
public final class SleelaGuiHost implements AutoCloseable {
    private final SleelaGui gui;
    private final SleelaRuntime runtime;

    public SleelaGuiHost(SleelaGui gui, SleelaRuntime runtime) {
        this.gui = Objects.requireNonNull(gui, "gui");
        this.runtime = Objects.requireNonNull(runtime, "runtime");
    }

    public void show(String title, int width, int height, String actionOperation) {
        gui.show(title, width, height);
        gui.onAction(() -> {
            Object result = runtime.call(actionOperation);
            if (result != null) gui.setText(String.valueOf(result));
        });
    }

    @Override
    public void close() {
        try {
            gui.close();
        } finally {
            runtime.close();
        }
    }
}
