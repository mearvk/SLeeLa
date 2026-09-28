package com.mearvk.sleela.gui;

import java.nio.file.Path;
import java.util.Map;
import java.util.Objects;

/**
 * Connects a Java GUI backend to SLeeLa business logic.
 * Java owns presentation; SLeeLa owns operation meaning and state transitions.
 */
public final class SleelaGuiHost implements AutoCloseable {
    private final SleelaGui gui;
    private final SleelaRuntime runtime;

    /** Active document-change listener, when the listen option is enabled. */
    private DocumentListener documentListener;

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

    /**
     * Enables the <b>document-change listener option</b> on the Java host path:
     * watch 1..14 documents and, when any changes on an OS call, invoke a SLeeLa
     * operation and refresh the running GUI with its result.
     *
     * <p>SLeeLa stays authoritative for what a document change <em>means</em>:
     * on each change the host calls {@code changeOperation} with the changed
     * document's id and new revision as arguments and publishes the returned
     * value to the window. If the operation returns {@code null}, the GUI's
     * default {@link SleelaGui#refresh(String, String)} is used instead so the
     * live UI still reflects the change.
     *
     * <p>The map must hold between {@link DocumentListener#MIN_DOCUMENTS} and
     * {@link DocumentListener#MAX_DOCUMENTS} documents. Calling this again
     * replaces any previously installed listener.
     *
     * @param idToPath        ordered {@code id -> path} documents to watch (1..14)
     * @param changeOperation SLeeLa operation invoked on each change
     * @return the started {@link DocumentListener}
     */
    public DocumentListener listen(Map<String, Path> idToPath, String changeOperation) {
        Objects.requireNonNull(idToPath, "idToPath");
        Objects.requireNonNull(changeOperation, "changeOperation");
        stopListening();
        DocumentListener listener = new DocumentListener(idToPath, change -> {
            Object result = runtime.call(changeOperation, change.id(), change.current().revision());
            if (result != null) {
                gui.setText(String.valueOf(result));
            } else {
                gui.refresh(change.id(), change.current().revision());
            }
        });
        listener.watch();
        this.documentListener = listener;
        return listener;
    }

    /**
     * Forces an immediate refresh of the watched documents (explicit OS/lifecycle
     * refresh). No-op when the listen option is off.
     *
     * @return number of documents that changed, or {@code 0} if not listening
     */
    public int refreshNow() {
        DocumentListener listener = this.documentListener;
        return listener == null ? 0 : listener.refreshNow();
    }

    /** True while the document-change listener option is active. */
    public boolean isListening() {
        DocumentListener listener = this.documentListener;
        return listener != null && listener.isRunning();
    }

    /** Stops the document-change listener if one is installed. */
    public void stopListening() {
        DocumentListener listener = this.documentListener;
        this.documentListener = null;
        if (listener != null) {
            listener.close();
        }
    }

    @Override
    public void close() {
        try {
            stopListening();
        } finally {
            try {
                gui.close();
            } finally {
                runtime.close();
            }
        }
    }
}
