package com.mearvk.sleela.gui;

import java.util.Objects;

/** Toolkit-neutral GUI contract used by SLeeLa hosts. */
public interface SleelaGui extends AutoCloseable {
    void show(String title, int width, int height);

    void setText(String text);

    void onAction(Runnable action);

    /**
     * Refreshes the running GUI in response to a watched document change.
     *
     * <p>This is the sink the {@link DocumentListener} option drives when a
     * document changes on an OS call. The default implementation publishes a
     * short status line through {@link #setText(String)}; a backend or host may
     * override it to re-render richer document state. Implementations must be
     * safe to call from a non-toolkit thread — they are expected to marshal onto
     * the toolkit thread themselves (the {@link SwingGui}/{@link FxGui} backends
     * already do, and {@link SleelaGuiRuntime#listen} routes through this).
     *
     * @param documentId the id of the document that changed
     * @param revision   the new OS-derived revision fingerprint
     */
    default void refresh(String documentId, String revision) {
        setText("Document '" + documentId + "' changed (" + revision + ")");
    }

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
