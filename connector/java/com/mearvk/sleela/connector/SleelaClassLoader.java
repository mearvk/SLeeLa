package com.mearvk.sleela.connector;

import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.jar.JarFile;

/**
 * Allow-listed loader for raw compiled Java classes used by SLeeLa.
 *
 * Resolution is non-initializing by default. Explicit roots may be directories
 * or readable JAR files. Adding a root invalidates cached resolutions so a
 * class loaded before the root was added cannot shadow the new search space.
 */
public final class SleelaClassLoader implements AutoCloseable {
    public static final class ClassResolutionException extends Exception {
        private static final long serialVersionUID = 1L;
        public ClassResolutionException(String message) { super(message); }
        public ClassResolutionException(String message, Throwable cause) { super(message, cause); }
    }

    private static final String BINARY_NAME =
            "[A-Za-z_$][A-Za-z0-9_$]*(\\.[A-Za-z_$][A-Za-z0-9_$]*)*";

    private final Set<String> allowedNames = ConcurrentHashMap.newKeySet();
    private final Set<String> allowedPackages = ConcurrentHashMap.newKeySet();
    private final Map<String, Class<?>> cache = new ConcurrentHashMap<>();
    private final ClassLoader parent;
    private final List<URL> roots = new ArrayList<>();
    private volatile URLClassLoader rootLoader;
    private boolean resolutionStarted;

    public SleelaClassLoader() {
        this(SleelaClassLoader.class.getClassLoader());
    }

    public SleelaClassLoader(ClassLoader parent) {
        this.parent = Objects.requireNonNull(parent, "parent");
    }

    public SleelaClassLoader allowClass(String fqcn) {
        String name = requireBinaryName(fqcn, "class name");
        allowedNames.add(name);
        cache.remove(name);
        return this;
    }

    public SleelaClassLoader allowPackage(String packagePrefix) {
        String name = requireBinaryName(packagePrefix, "package prefix");
        allowedPackages.add(name);
        cache.clear();
        return this;
    }

    /** Add a directory of class files or a readable .jar file. */
    public SleelaClassLoader addRoot(Path dirOrJar) throws IOException {
        Objects.requireNonNull(dirOrJar, "dirOrJar");
        Path root = dirOrJar.toAbsolutePath().normalize();
        if (Files.isDirectory(root)) {
            // A directory root is a package-preserving class-file tree.
        } else if (Files.isRegularFile(root)
                && root.getFileName().toString().toLowerCase(Locale.ROOT).endsWith(".jar")) {
            // Opening the archive here makes malformed/unreadable roots fail
            // at configuration time, not during an unrelated class lookup.
            try (JarFile ignored = new JarFile(root.toFile(), false)) {
                // Validation only; URLClassLoader owns subsequent reads.
            }
        } else {
            throw new IOException("load root must be a directory or readable .jar: " + root);
        }

        URL url = root.toUri().toURL();
        synchronized (roots) {
            if (resolutionStarted) {
                throw new IOException("configure all Java class roots before the first class resolution");
            }
            if (roots.contains(url)) {
                return this;
            }
            List<URL> nextRoots = new ArrayList<>(roots);
            nextRoots.add(url);
            URLClassLoader replacement = new URLClassLoader(nextRoots.toArray(new URL[0]), parent);
            URLClassLoader previous = rootLoader;
            roots.add(url);
            rootLoader = replacement;
            cache.clear();
            if (previous != null) {
                try {
                    previous.close();
                } catch (IOException ignored) {
                    // The new loader is active; a previous loader close failure
                    // must not silently restore stale class-resolution results.
                }
            }
        }
        return this;
    }

    /** Resolve a class without running static initializers. */
    public Class<?> resolve(String fqcn) throws ClassResolutionException {
        return resolve(fqcn, false);
    }

    /**
     * Resolve a class under the configured allow-list.
     *
     * @param initialize explicitly opt in to static initialization
     */
    public Class<?> resolve(String fqcn, boolean initialize) throws ClassResolutionException {
        String name = requireBinaryName(fqcn, "class name");
        if (!isAllowed(name)) {
            throw new ClassResolutionException("class not allow-listed: " + name);
        }
        synchronized (roots) {
            resolutionStarted = true;
            Class<?> cached = cache.get(name);
            if (cached != null) {
                if (initialize) {
                    forceInit(cached);
                }
                return cached;
            }
            ClassLoader loader = rootLoader != null ? rootLoader : parent;
            try {
                Class<?> type = Class.forName(name, initialize, loader);
                cache.put(name, type);
                return type;
            } catch (ClassNotFoundException e) {
                throw new ClassResolutionException("class not found on configured roots: " + name, e);
            } catch (LinkageError e) {
                throw new ClassResolutionException(
                        "class failed to link: " + name + " (" + e.getMessage() + ")", e);
            }
        }
    }

    /** Check policy only; this method does not load or initialize the class. */
    public boolean isAllowed(String fqcn) {
        if (fqcn == null) {
            return false;
        }
        final String name;
        try {
            name = requireBinaryName(fqcn, "class name");
        } catch (IllegalArgumentException invalid) {
            return false;
        }
        if (allowedNames.contains(name)) {
            return true;
        }
        for (String pkg : allowedPackages) {
            if (name.startsWith(pkg + ".")) {
                return true;
            }
        }
        return false;
    }

    public Set<String> allowedClassNames() {
        return new LinkedHashSet<>(allowedNames);
    }

    @Override
    public void close() {
        synchronized (roots) {
            URLClassLoader current = rootLoader;
            rootLoader = null;
            roots.clear();
            cache.clear();
            resolutionStarted = false;
            if (current != null) {
                try {
                    current.close();
                } catch (IOException ignored) {
                    // Closing is best effort; all future lookups use the parent.
                }
            }
        }
    }

    private static void forceInit(Class<?> type) throws ClassResolutionException {
        try {
            Class.forName(type.getName(), true, type.getClassLoader());
        } catch (ClassNotFoundException e) {
            throw new ClassResolutionException("class disappeared during initialization: "
                    + type.getName(), e);
        } catch (LinkageError e) {
            throw new ClassResolutionException("class failed during initialization: "
                    + type.getName(), e);
        }
    }

    private static String requireBinaryName(String value, String label) {
        Objects.requireNonNull(value, label);
        String name = value.trim();
        if (name.isEmpty() || !name.matches(BINARY_NAME)) {
            throw new IllegalArgumentException("invalid " + label + ": " + value);
        }
        return name;
    }
}
