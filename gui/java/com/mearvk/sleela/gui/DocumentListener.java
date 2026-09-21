package com.mearvk.sleela.gui;

import java.io.IOException;
import java.nio.file.ClosedWatchServiceException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.StandardWatchEventKinds;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.Consumer;

/**
 * The SLeeLa GUI <b>document-change listener option</b>.
 *
 * <p>A {@code DocumentListener} watches between {@value #MIN_DOCUMENTS} and
 * {@value #MAX_DOCUMENTS} documents and reports when any of them changes. It is
 * the mechanism behind SLeeLa's "listen to document changes" option: when a
 * watched document changes on disk, the listener re-snapshots it and hands the
 * change to a callback, which the GUI runtime uses to update the running
 * window.
 *
 * <h2>Refreshes on OS call(s)</h2>
 * Change detection is driven by the operating system, not by polling. The
 * listener registers each document's parent directory with a
 * {@link java.nio.file.WatchService}, which is backed by the host OS
 * notification facility (inotify on Linux, {@code ReadDirectoryChangesW} on
 * Windows, {@code kqueue}/FSEvents on macOS). A watch thread blocks in
 * {@link WatchService#poll} — an OS call — and wakes only when the OS reports
 * activity. Each wake is an "OS call" that triggers a refresh of the affected
 * documents. A {@link #refreshNow()} method also lets a host force a
 * re-evaluation on demand (e.g. from an explicit OS/lifecycle callback).
 *
 * <h2>Bound: 1..14 documents</h2>
 * The document count is validated at construction and on {@link #watch}. Fewer
 * than one or more than fourteen documents is rejected with
 * {@link IllegalArgumentException}; this is a hard contract of the listener
 * option.
 *
 * <p>The class is thread-safe for start/stop and for {@link #refreshNow()}. The
 * change callback is invoked on the internal watch thread; callers that must
 * touch a GUI toolkit are responsible for marshaling onto the toolkit thread
 * (the {@link SleelaGuiRuntime} listen option does this for you).
 */
public final class DocumentListener implements AutoCloseable {

    /** Lower bound of the watched-document count (inclusive). */
    public static final int MIN_DOCUMENTS = 1;
    /** Upper bound of the watched-document count (inclusive). */
    public static final int MAX_DOCUMENTS = 14;

    /** A single reported document change. */
    public static final class Change {
        private final SleelaDocument previous;
        private final SleelaDocument current;

        Change(SleelaDocument previous, SleelaDocument current) {
            this.previous = previous;
            this.current = current;
        }

        /** The document as previously snapshotted (never {@code null}). */
        public SleelaDocument previous() {
            return previous;
        }

        /** The freshly re-read document (never {@code null}). */
        public SleelaDocument current() {
            return current;
        }

        /** The stable document id. */
        public String id() {
            return current.id();
        }

        @Override
        public String toString() {
            return "Change{id=" + current.id()
                + ", " + previous.revision() + " -> " + current.revision() + "}";
        }
    }

    /** id -> latest known snapshot (insertion order preserved; guarded by itself). */
    private final Map<String, SleelaDocument> documents =
        Collections.synchronizedMap(new LinkedHashMap<>());
    /** directory -> WatchKey, so we register each parent directory only once. */
    private final Map<Path, WatchKey> watchedDirs = new ConcurrentHashMap<>();
    private final Consumer<Change> onChange;
    private final long pollMillis;

    private final AtomicBoolean running = new AtomicBoolean(false);
    private volatile WatchService watchService;
    private volatile Thread watchThread;

    /**
     * Creates a listener over an ordered map of {@code id -> path}. The map must
     * hold between {@value #MIN_DOCUMENTS} and {@value #MAX_DOCUMENTS} entries.
     *
     * @param idToPath  ordered documents to watch (1..14)
     * @param onChange  invoked once per detected change (on the watch thread)
     */
    public DocumentListener(Map<String, Path> idToPath, Consumer<Change> onChange) {
        this(idToPath, onChange, 500L);
    }

    /**
     * @param pollMillis wake cadence for the blocking OS watch call; also the
     *                   effective debounce window for coalescing rapid edits.
     */
    public DocumentListener(Map<String, Path> idToPath, Consumer<Change> onChange, long pollMillis) {
        Objects.requireNonNull(idToPath, "idToPath");
        this.onChange = Objects.requireNonNull(onChange, "onChange");
        if (pollMillis <= 0) {
            throw new IllegalArgumentException("pollMillis must be positive");
        }
        this.pollMillis = pollMillis;
        requireBound(idToPath.size());
        for (Map.Entry<String, Path> e : idToPath.entrySet()) {
            String id = Objects.requireNonNull(e.getKey(), "document id");
            Path path = Objects.requireNonNull(e.getValue(), "document path");
            documents.put(id, SleelaDocument.snapshot(id, path));
        }
    }

    /** Validates the 1..14 document-count contract. */
    static void requireBound(int count) {
        if (count < MIN_DOCUMENTS || count > MAX_DOCUMENTS) {
            throw new IllegalArgumentException(
                "DocumentListener watches " + MIN_DOCUMENTS + ".." + MAX_DOCUMENTS
                    + " documents; got " + count);
        }
    }

    /** The set of document ids currently watched, in insertion order. */
    public List<String> watchedIds() {
        synchronized (documents) {
            return Collections.unmodifiableList(new ArrayList<>(documents.keySet()));
        }
    }

    /** How many documents are currently watched. */
    public int count() {
        return documents.size();
    }

    /**
     * Starts the OS-backed watch. Registers each document's parent directory
     * with the platform {@link WatchService} and spins the watch thread. Safe to
     * call once; a second call while running is a no-op.
     */
    public synchronized void watch() {
        if (running.get()) {
            return;
        }
        requireBound(documents.size());
        try {
            WatchService service = FileSystems.getDefault().newWatchService();
            this.watchService = service;
            List<Path> dirs = new ArrayList<>();
            synchronized (documents) {
                for (SleelaDocument doc : documents.values()) {
                    Path dir = doc.path().getParent();
                    if (dir != null) {
                        dirs.add(dir);
                    }
                }
            }
            for (Path dir : dirs) {
                watchedDirs.computeIfAbsent(dir, d -> registerDir(service, d));
            }
        } catch (IOException e) {
            throw new IllegalStateException("Unable to start OS document watch", e);
        }
        running.set(true);
        Thread t = new Thread(this::runWatchLoop, "sleela-document-listener");
        t.setDaemon(true);
        this.watchThread = t;
        t.start();
    }

    private static WatchKey registerDir(WatchService service, Path dir) {
        try {
            return dir.register(service,
                StandardWatchEventKinds.ENTRY_MODIFY,
                StandardWatchEventKinds.ENTRY_CREATE,
                StandardWatchEventKinds.ENTRY_DELETE);
        } catch (IOException e) {
            throw new IllegalStateException("Unable to watch directory: " + dir, e);
        }
    }

    /** The blocking OS-call loop: wake on OS notification, then refresh. */
    private void runWatchLoop() {
        WatchService service = this.watchService;
        while (running.get() && service != null) {
            WatchKey key;
            try {
                // Blocking OS call; returns null on timeout, a key on OS activity.
                key = service.poll(pollMillis, TimeUnit.MILLISECONDS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            } catch (ClosedWatchServiceException e) {
                break;
            }
            if (key != null) {
                // Drain the OS events; we use them only as a wake signal and
                // then re-evaluate each document's real revision below.
                key.pollEvents();
                key.reset();
            }
            // Whether woken by an OS event or by the poll timeout, re-check the
            // watched documents and emit coalesced changes.
            refreshNow();
        }
    }

    /**
     * Forces an immediate re-evaluation of every watched document and emits a
     * {@link Change} for each one whose OS revision moved since the last check.
     * This is what an explicit OS/lifecycle callback calls to refresh on demand;
     * it is also called internally on every watch-thread wake.
     *
     * @return the number of documents that changed
     */
    public int refreshNow() {
        // Detect changes under the map lock, then fire callbacks outside it so a
        // slow or reentrant callback never blocks the watched-document map.
        List<Change> changes = new ArrayList<>();
        synchronized (documents) {
            for (Map.Entry<String, SleelaDocument> e : documents.entrySet()) {
                SleelaDocument previous = e.getValue();
                SleelaDocument current = previous.reread();
                if (current.changedFrom(previous)) {
                    e.setValue(current);
                    changes.add(new Change(previous, current));
                }
            }
        }
        for (Change change : changes) {
            try {
                onChange.accept(change);
            } catch (RuntimeException callbackError) {
                // A faulty callback must not stop the listener; a watched
                // document staying observable is more important than one
                // failed refresh.
            }
        }
        return changes.size();
    }

    /** True while the OS watch loop is active. */
    public boolean isRunning() {
        return running.get();
    }

    /** Stops the watch thread and releases the OS watch handle. Idempotent. */
    @Override
    public synchronized void close() {
        if (!running.getAndSet(false)) {
            closeService();
            return;
        }
        Thread t = this.watchThread;
        if (t != null) {
            t.interrupt();
        }
        closeService();
        watchedDirs.clear();
    }

    private void closeService() {
        WatchService service = this.watchService;
        this.watchService = null;
        if (service != null) {
            try {
                service.close();
            } catch (IOException ignored) {
                // Best-effort release of the OS watch handle.
            }
        }
    }

    /**
     * Convenience builder that accepts a list of {@code id=path} pairs and
     * enforces the 1..14 bound. Order is preserved.
     */
    public static Map<String, Path> documents(List<String> ids, List<Path> paths) {
        Objects.requireNonNull(ids, "ids");
        Objects.requireNonNull(paths, "paths");
        if (ids.size() != paths.size()) {
            throw new IllegalArgumentException("ids and paths must have equal length");
        }
        requireBound(ids.size());
        Map<String, Path> map = new LinkedHashMap<>();
        for (int i = 0; i < ids.size(); i++) {
            map.put(ids.get(i), paths.get(i));
        }
        return map;
    }
}
