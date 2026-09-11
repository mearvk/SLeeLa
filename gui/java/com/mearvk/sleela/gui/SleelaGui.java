package com.mearvk.sleela.gui;

import java.util.Objects;

/** Toolkit-neutral GUI contract used by SLeeLa hosts. */
public interface SleelaGui extends AutoCloseable {
    void show(String title, int width, int height);

    void setText(String text);

    void onAction(Runnable action);

    @Override
    void close();

    default void closeQuietly() {
        try {
            close();
        } catch (RuntimeException ignored) {
            // Host shutdown should remain deterministic.
        }
    }

    /** Creates a small example window using the requested backend. */
    static SleelaGui create(String backend) {
        Objects.requireNonNull(backend, "backend");
        return switch (backend.toLowerCase()) {
            case "swing" -> new SwingGui();
            case "javafx", "fx" -> new FxGui();
            default -> throw new IllegalArgumentException("Unknown GUI backend: " + backend);
        };
    }
}
