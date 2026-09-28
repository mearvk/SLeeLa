package com.mearvk.sleela.connector;

import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Intelligent loader of raw compiled classes for the SLeeLa &rarr; Java bridge.
 *
 * <p>This is a {@code Class.forName()}-style facility, but <b>hardened</b>. A raw
 * {@code Class.forName(userString)} on a JVM is a code-execution / deserialization
 * hazard: it can resolve and static-initialize <i>any</i> class on the classpath.
 * SLeeLa's bridge instead loads only what an operator has explicitly permitted,
 * validates the request, caches the result, and <b>fails closed</b> on anything
 * unknown — matching the allow-list posture already used by the java28 memory
 * server.</p>
 *
 * <h2>What "intelligent" means here</h2>
 * <ul>
 *   <li><b>Allow-listed.</b> A class resolves only if its fully-qualified name is
 *       explicitly allowed, or it lives under an allowed package prefix.</li>
 *   <li><b>Explicit roots.</b> Raw compiled {@code .class} directories and
 *       {@code .jar} files can be added as load roots; classes are read from
 *       those via a {@link URLClassLoader} layered on the bridge's own loader.</li>
 *   <li><b>Non-initializing by default.</b> Resolution does <i>not</i> run the
 *       class's static initializers unless the caller opts in, so merely looking
 *       a class up cannot trigger side effects.</li>
 *   <li><b>Cached.</b> Successful resolutions are memoized.</li>
 *   <li><b>Fail closed.</b> A disallowed or missing class yields a clear
 *       {@link ClassResolutionException}, never a silent or arbitrary load.</li>
 * </ul>
 *
 * <p>Thread-safe. Not {@code AutoCloseable}-required, but {@link #close()} releases
 * any {@link URLClassLoader} opened for jar/dir roots.</p>
 */
public final class SleelaClassLoader implements AutoCloseable {

    /** Thrown when a class cannot be resolved under the loader's policy. */
    public static final class ClassResolutionException extends Exception {
        private static final long serialVersionUID = 1L;
        public ClassResolutionException(String message) { super(message); }
        public ClassResolutionException(String message, Throwable cause) { super(message, cause); }
    }

    private final Set<String> allowedNames = ConcurrentHashMap.newKeySet();
    private final Set<String> allowedPackages = ConcurrentHashMap.newKeySet();
    private final Map<String, Class<?>> cache = new ConcurrentHashMap<>();
    private final ClassLoader parent;

    // Lazily-built loader over the operator-supplied roots (dirs/jars).
    private final List<URL> roots = new ArrayList<>();
    private volatile URLClassLoader rootLoader;

    public SleelaClassLoader() {
        this(SleelaClassLoader.class.getClassLoader());
    }

    public SleelaClassLoader(ClassLoader parent) {
        this.parent = Objects.requireNonNull(parent, "parent");
    }

    // ---- policy configuration -------------------------------------------

    /** Permit one fully-qualified class name (e.g. {@code "com.acme.Widget"}). */
    public SleelaClassLoader allowClass(String fqcn) {
        allowedNames.add(requireFqcn(fqcn));
        cache.remove(fqcn); // re-evaluate under the new policy if it was denied
        return this;
    }

    /** Permit every class under a package prefix (e.g. {@code "com.acme"}). */
    public SleelaClassLoader allowPackage(String packagePrefix) {
        Objects.requireNonNull(packagePrefix, "packagePrefix");
        if (packagePrefix.isBlank()) {
            throw new IllegalArgumentException("packagePrefix must not be blank");
        }
        allowedPackages.add(packagePrefix);
        return this;
    }

    /** Add a directory of raw compiled {@code .class} files or a {@code .jar}. */
    public SleelaClassLoader addRoot(Path dirOrJar) throws IOException {
        Objects.requireNonNull(dirOrJar, "dirOrJar");
        if (!Files.exists(dirOrJar)) {
            throw new IOException("load root does not exist: " + dirOrJar);
        }
        synchronized (roots) {
            roots.add(dirOrJar.toUri().toURL());
            // Rebuild the loader so the new root is visible; close the old one.
            URLClassLoader previous = rootLoader;
            rootLoader = new URLClassLoader(roots.toArray(new URL[0]), parent);
            if (previous != null) {
                try { previous.close(); } catch (IOException ignored) { /* best effort */ }
            }
        }
        return this;
    }

    // ---- resolution ------------------------------------------------------

    /** Resolve a class by name under the policy, without running its static
     *  initializers. Equivalent intent to {@code Class.forName(name, false, cl)}. */
    public Class<?> resolve(String fqcn) throws ClassResolutionException {
        return resolve(fqcn, false);
    }

    /**
     * Resolve a class by name under the policy. The {@code Class.forName()}
     * analogue, but allow-listed, cached, and fail-closed.
     *
     * @param fqcn       fully-qualified class name
     * @param initialize if true, run the class's static initializers
     */
    public Class<?> resolve(String fqcn, boolean initialize) throws ClassResolutionException {
        requireFqcn(fqcn);
        if (!isAllowed(fqcn)) {
            throw new ClassResolutionException("class not allow-listed: " + fqcn);
        }
        Class<?> cached = cache.get(fqcn);
        if (cached != null) {
            if (initialize) {
                forceInit(cached);
            }
            return cached;
        }
        ClassLoader loader = rootLoader != null ? rootLoader : parent;
        try {
            Class<?> type = Class.forName(fqcn, initialize, loader);
            cache.put(fqcn, type);
            return type;
        } catch (ClassNotFoundException e) {
            throw new ClassResolutionException("class not found on any load root: " + fqcn, e);
        } catch (LinkageError e) {
            // e.g. bad/incompatible bytecode in a raw .class root.
            throw new ClassResolutionException("class failed to link: " + fqcn + " (" + e.getMessage() + ")", e);
        }
    }

    /** True if {@code fqcn} would resolve under the current policy (no load). */
    public boolean isAllowed(String fqcn) {
        if (fqcn == null || fqcn.isBlank()) {
            return false;
        }
        if (allowedNames.contains(fqcn)) {
            return true;
        }
        for (String pkg : allowedPackages) {
            if (fqcn.equals(pkg) || fqcn.startsWith(pkg + ".")) {
                return true;
            }
        }
        return false;
    }

    /** Snapshot of the explicitly-allowed class names (diagnostics). */
    public Set<String> allowedClassNames() {
        return new LinkedHashSet<>(allowedNames);
    }

    @Override
    public void close() {
        synchronized (roots) {
            if (rootLoader != null) {
                try { rootLoader.close(); } catch (IOException ignored) { /* best effort */ }
                rootLoader = null;
            }
        }
    }

    // ---- helpers ---------------------------------------------------------

    private static void forceInit(Class<?> type) throws ClassResolutionException {
        try {
            Class.forName(type.getName(), true, type.getClassLoader());
        } catch (ClassNotFoundException e) {
            throw new ClassResolutionException("class disappeared during init: " + type.getName(), e);
        }
    }

    private static String requireFqcn(String fqcn) {
        Objects.requireNonNull(fqcn, "fqcn");
        String s = fqcn.trim();
        if (s.isEmpty()) {
            throw new IllegalArgumentException("class name must not be blank");
        }
        // Reject obvious junk / injection: only a dotted Java identifier chain
        // (optionally with '$' for nested classes) is a valid binary name.
        if (!s.matches("[A-Za-z_$][A-Za-z0-9_$]*(\\.[A-Za-z_$][A-Za-z0-9_$]*)*")) {
            throw new IllegalArgumentException("not a valid class name: " + fqcn);
        }
        return s;
    }
}
