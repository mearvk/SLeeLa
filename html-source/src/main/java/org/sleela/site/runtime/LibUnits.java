package org.sleela.site.runtime;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Index of library units (lib/**&#47;Name.sleela) so the safety check can follow the units a program pulls in.
 * Several units can share a name (the GUI ships five parallel version lines); the check treats all of them as referenced.
 */
public final class LibUnits {
    private static final long MAX_UNIT_BYTES = 2_000_000L;
    private final Path lib;
    private volatile Map<String, List<Path>> index;

    public LibUnits(Path home) { this.lib = home == null ? null : home.resolve("lib"); }

    private Map<String, List<Path>> index() {
        Map<String, List<Path>> ix = index;
        if (ix != null) return ix;
        synchronized (this) {
            if (index != null) return index;
            Map<String, List<Path>> m = new HashMap<>();
            if (lib != null && Files.isDirectory(lib)) {
                try {
                    Files.walkFileTree(lib, new SimpleFileVisitor<Path>() {
                        @Override public FileVisitResult visitFile(Path f, BasicFileAttributes a) {
                            String n = f.getFileName().toString();
                            if (n.endsWith(".sleela") && a.isRegularFile()) {
                                m.computeIfAbsent(n.substring(0, n.length() - 7), k -> new ArrayList<>()).add(f);
                            }
                            return FileVisitResult.CONTINUE;
                        }
                    });
                } catch (IOException ignored) { }
            }
            index = m;
            return m;
        }
    }

    /** Combined text of every unit with this name, or null if there is none. */
    public String sourceOf(String unit) {
        List<Path> paths = index().get(unit);
        if (paths == null) return null;
        StringBuilder sb = new StringBuilder();
        for (Path p : paths) {
            try {
                if (Files.size(p) <= MAX_UNIT_BYTES) sb.append(new String(Files.readAllBytes(p), StandardCharsets.UTF_8)).append('\n');
                else sb.append("osRun\n");   // an unreadable-size unit is treated as unsafe
            } catch (IOException e) {
                sb.append("osRun\n");
            }
        }
        return sb.toString();
    }

    public int size() { return index().size(); }
}
