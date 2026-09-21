package com.mearvk.sleela.gui;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.Objects;

/**
 * One document watched by the SLeeLa GUI document listener.
 *
 * <p>A {@code SleelaDocument} is an immutable snapshot: a stable {@code id} (the
 * name SLeeLa addresses it by), the resolved {@code path} on the host, and a
 * {@code revision} fingerprint captured from the operating system's file
 * metadata (last-modified time and size). The listener compares a fresh
 * snapshot against the previous one to decide whether a watched document
 * actually changed — an OS watch event that does not move the fingerprint is
 * coalesced away rather than pushed to the running GUI.
 *
 * <p>The revision is deliberately derived from OS-reported metadata, not the
 * file contents: it is cheap to take on every OS call and is sufficient to
 * detect the "document changed" condition the GUI refreshes on.
 */
public final class SleelaDocument {

    /** SLeeLa-facing identity of the document (stable across revisions). */
    private final String id;
    /** Resolved, normalized path on the host filesystem. */
    private final Path path;
    /** OS-derived change fingerprint: {@code <lastModifiedMillis>:<size>}. */
    private final String revision;

    public SleelaDocument(String id, Path path, String revision) {
        this.id = Objects.requireNonNull(id, "id");
        this.path = Objects.requireNonNull(path, "path").toAbsolutePath().normalize();
        this.revision = Objects.requireNonNull(revision, "revision");
    }

    /**
     * Captures a document snapshot from the current OS file metadata. If the
     * file is absent or unreadable the revision is recorded as {@code "absent"},
     * so appearance/disappearance is itself a detectable change.
     */
    public static SleelaDocument snapshot(String id, Path path) {
        Objects.requireNonNull(id, "id");
        Objects.requireNonNull(path, "path");
        return new SleelaDocument(id, path, revisionOf(path));
    }

    /** Returns a snapshot of this document re-read from the OS right now. */
    public SleelaDocument reread() {
        return new SleelaDocument(id, path, revisionOf(path));
    }

    /** Reads the OS change fingerprint for {@code path}; {@code "absent"} if unavailable. */
    static String revisionOf(Path path) {
        try {
            BasicFileAttributes attrs = Files.readAttributes(path, BasicFileAttributes.class);
            return attrs.lastModifiedTime().toMillis() + ":" + attrs.size();
        } catch (IOException | RuntimeException unavailable) {
            return "absent";
        }
    }

    public String id() {
        return id;
    }

    public Path path() {
        return path;
    }

    public String revision() {
        return revision;
    }

    /** True when {@code other} names the same document at a different revision. */
    public boolean changedFrom(SleelaDocument other) {
        return other != null
            && id.equals(other.id)
            && path.equals(other.path)
            && !revision.equals(other.revision);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof SleelaDocument other)) return false;
        return id.equals(other.id) && path.equals(other.path) && revision.equals(other.revision);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, path, revision);
    }

    @Override
    public String toString() {
        return "SleelaDocument{id=" + id + ", path=" + path + ", revision=" + revision + "}";
    }
}
