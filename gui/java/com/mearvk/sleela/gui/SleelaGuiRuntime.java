package com.mearvk.sleela.gui;

import java.nio.file.Path;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

/**
 * Path 2: SLeeLa owns GUI intent and Java provides the desktop implementation.
 * A SLeeLa-side dispatcher can bind named operations to this host.
 */
public final class SleelaGuiRuntime {
    private final SleelaGui gui;
    private final Map<String, Runnable> actions = new LinkedHashMap<>();

    /** Active document-change listener, when the listen option is enabled. */
    private DocumentListener documentListener;

    public SleelaGuiRuntime(SleelaGui gui) {
        this.gui = Objects.requireNonNull(gui, "gui");
    }

    public void window(String title, int width, int height) {
        gui.show(title, width, height);
    }

    public void text(String text) {
        gui.setText(text);
    }

    public void action(String name, Runnable operation) {
        actions.put(Objects.requireNonNull(name, "name"), Objects.requireNonNull(operation, "operation"));
        gui.onAction(() -> {
            Runnable selected = actions.get(name);
            if (selected != null) selected.run();
        });
    }

    /**
     * Enables the <b>document-change listener option</b>: watch 1..14 documents
     * and refresh the running GUI whenever any of them changes on an OS call.
     *
     * <p>Each entry of {@code idToPath} is a document to observe. The map must
     * hold between {@link DocumentListener#MIN_DOCUMENTS} and
     * {@link DocumentListener#MAX_DOCUMENTS} entries; otherwise this throws
     * {@link IllegalArgumentException}. When a watched document changes, the
     * listener drives {@link SleelaGui#refresh(String, String)} on the running
     * window — the Swing/JavaFX backends marshal that onto their toolkit thread,
     * so the update is applied safely to the live UI.
     *
     * <p>Calling {@code listen} again replaces any previously installed listener.
     *
     * @param idToPath ordered {@code id -> path} documents to watch (1..14)
     * @return the started {@link DocumentListener} (also retained for cleanup)
     */
    public DocumentListener listen(Map<String, Path> idToPath) {
        Objects.requireNonNull(idToPath, "idToPath");
        stopListening();
        DocumentListener listener = new DocumentListener(
            idToPath,
            change -> gui.refresh(change.id(), change.current().revision()));
        listener.watch();
        this.documentListener = listener;
        return listener;
    }

    /**
     * Convenience form of {@link #listen(Map)} taking parallel id/path lists.
     * The lists must be equal length and hold 1..14 entries.
     */
    public DocumentListener listen(List<String> ids, List<Path> paths) {
        return listen(DocumentListener.documents(ids, paths));
    }

    /**
     * Forces an immediate refresh of the watched documents (an explicit
     * OS/lifecycle "refresh on call"). No-op when the listen option is off.
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

    public void close() {
        stopListening();
        gui.close();
    }
}
