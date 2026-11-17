package org.sleela.site;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.stream.Stream;

import org.sleela.site.Model.ExampleFile;
import org.sleela.site.Model.ExampleGroup;
import org.sleela.site.Model.Hit;
import org.sleela.site.Model.Module;
import org.sleela.site.Model.Tutorial;

/** Read-only view of the SLeeLa source tree (tutorials, examples, modules, API docs). */
public final class ContentIndex {
    public static final long MAX_FILE_BYTES = 1_500_000L;
    private static final int MAX_EXAMPLE_FILES = 6000;
    private static final int MAX_MODULE_FILES = 3000;

    private static final Set<String> TEXT_EXT = Set.of(
            "md", "sleela", "java", "c", "h", "cpp", "hpp", "cc", "py", "sh", "xml", "sst", "json", "txt",
            "conf", "yml", "yaml", "model", "syntax", "sheet", "mk", "cmake", "html", "css", "js", "jsp", "properties");
    private static final Set<String> TEXT_NAMES = Set.of("Makefile", "LICENSE", "README", "Dockerfile", "CMakeLists.txt");
    private static final Set<String> SKIP_DIRS = Set.of("build", "node_modules", "target", "dist", "out", ".git");
    private static final Set<String> NOT_SYS_MODULES = Set.of("lib", "examples", "html-source");

    private final Path root;
    private final Path realRoot;
    private final Map<String, Object> cache = new ConcurrentHashMap<>();

    public ContentIndex(Path root) throws IOException {
        this.root = root.toAbsolutePath().normalize();
        this.realRoot = this.root.toRealPath();
    }

    public Path getRoot() { return root; }

    // ---------------------------------------------------------------- safe file access

    public static String extOf(String name) {
        int d = name.lastIndexOf('.');
        return d < 0 ? "" : name.substring(d + 1).toLowerCase(Locale.ROOT);
    }

    public static boolean allowedName(String name) {
        return TEXT_EXT.contains(extOf(name)) || TEXT_NAMES.contains(name);
    }

    public static String langOf(String name) {
        String e = extOf(name);
        switch (e) {
            case "sleela": case "sst": return "sleela";
            case "h": case "c": case "cpp": case "hpp": case "cc": return "c";
            case "sh": return "bash";
            case "md": return "markdown";
            case "": return name.equals("Makefile") ? "makefile" : "text";
            default: return e;
        }
    }

    /** Resolve a repo-relative path to a readable regular text file, or null. No traversal, no symlink escape, no hidden files. */
    public Path safeFile(String rel) {
        if (rel == null || rel.isEmpty() || rel.indexOf('\0') >= 0 || rel.indexOf('\\') >= 0 || rel.startsWith("/")) return null;
        for (String seg : rel.split("/")) {
            if (seg.isEmpty() || seg.startsWith(".")) return null;
        }
        if (!allowedName(rel.substring(rel.lastIndexOf('/') + 1))) return null;
        try {
            Path p = root.resolve(rel).normalize();
            if (!p.startsWith(root) || !Files.isRegularFile(p)) return null;
            Path real = p.toRealPath();
            if (!real.startsWith(realRoot)) return null;
            if (Files.size(real) > MAX_FILE_BYTES) return null;
            return real;
        } catch (IOException | RuntimeException e) {
            return null;
        }
    }

    public String readText(String rel) {
        Path p = safeFile(rel);
        if (p == null) return null;
        try {
            return new String(Files.readAllBytes(p), StandardCharsets.UTF_8);
        } catch (IOException e) {
            return null;
        }
    }

    private boolean isDir(String rel) { return Files.isDirectory(root.resolve(rel)); }

    // ---------------------------------------------------------------- tutorials

    @SuppressWarnings("unchecked")
    public List<Tutorial> tutorials() {
        return (List<Tutorial>) cache.computeIfAbsent("tutorials", k -> {
            List<Tutorial> out = new ArrayList<>();
            Path dir = root.resolve("tutorial");
            if (Files.isDirectory(dir)) {
                try (Stream<Path> s = Files.list(dir)) {
                    s.filter(p -> p.getFileName().toString().matches("\\d{2}-.*\\.md")).sorted().forEach(p -> {
                        String file = p.getFileName().toString();
                        String slug = file.substring(0, file.length() - 3);
                        String md = readText("tutorial/" + file);
                        String title = md == null ? null : Markdown.firstHeading(md);
                        out.add(new Tutorial(slug, title == null ? slug : title, "tutorial/" + file));
                    });
                } catch (IOException ignored) { }
            }
            return Collections.unmodifiableList(out);
        });
    }

    public Tutorial tutorial(String slug) {
        for (Tutorial t : tutorials()) if (t.getSlug().equals(slug)) return t;
        return null;
    }

    // ---------------------------------------------------------------- examples

    @SuppressWarnings("unchecked")
    public Map<String, ExampleGroup> exampleGroups() {
        return (Map<String, ExampleGroup>) cache.computeIfAbsent("examples", k -> {
            Map<String, List<ExampleFile>> groups = new TreeMap<>();
            Map<String, String> bases = new TreeMap<>();
            int[] budget = {MAX_EXAMPLE_FILES};
            for (String rootName : new String[] {"examples", "lib/examples"}) {
                Path base = root.resolve(rootName);
                if (!Files.isDirectory(base)) continue;
                try {
                    Files.walkFileTree(base, Set.of(), 6, new SimpleFileVisitor<Path>() {
                        @Override public FileVisitResult preVisitDirectory(Path d, BasicFileAttributes a) {
                            String n = d.getFileName().toString();
                            return !d.equals(base) && (n.startsWith(".") || SKIP_DIRS.contains(n)) ? FileVisitResult.SKIP_SUBTREE : FileVisitResult.CONTINUE;
                        }
                        @Override public FileVisitResult visitFile(Path f, BasicFileAttributes a) {
                            String name = f.getFileName().toString();
                            if (budget[0] <= 0) return FileVisitResult.TERMINATE;
                            if (name.startsWith(".") || !a.isRegularFile() || !allowedName(name) || a.size() > MAX_FILE_BYTES) return FileVisitResult.CONTINUE;
                            String rel = base.relativize(f).toString().replace('\\', '/');
                            int i = rel.indexOf('/');
                            String key = i < 0 ? rootName : rootName + "/" + rel.substring(0, i);
                            String within = i < 0 ? rel : rel.substring(i + 1);
                            String slug = Routes.exampleGroupSlug(key);
                            groups.computeIfAbsent(key, x -> new ArrayList<>()).add(new ExampleFile(slug, within, langOf(name), a.size()));
                            bases.put(key, i < 0 ? rootName : rootName + "/" + rel.substring(0, i));
                            budget[0]--;
                            return FileVisitResult.CONTINUE;
                        }
                    });
                } catch (IOException ignored) { }
            }
            Map<String, ExampleGroup> out = new LinkedHashMap<>();
            for (Map.Entry<String, List<ExampleFile>> e : groups.entrySet()) {
                List<ExampleFile> files = e.getValue();
                files.sort(Comparator.comparing(ExampleFile::getRel, String.CASE_INSENSITIVE_ORDER));
                String slug = Routes.exampleGroupSlug(e.getKey());
                out.put(slug, new ExampleGroup(slug, e.getKey(), bases.get(e.getKey()), Collections.unmodifiableList(files)));
            }
            return Collections.unmodifiableMap(out);
        });
    }

    public ExampleGroup exampleGroup(String slug) { return exampleGroups().get(slug); }

    /** Repo-relative path of an example file, validated against the group listing. */
    public String exampleRepoPath(ExampleGroup g, String rel) {
        for (ExampleFile f : g.getFiles()) {
            if (f.getRel().equals(rel)) {
                return g.getBaseRel() + "/" + rel;
            }
        }
        return null;
    }

    // ---------------------------------------------------------------- modules

    @SuppressWarnings("unchecked")
    public List<Module> libModules() {
        return (List<Module>) cache.computeIfAbsent("libModules", k -> listModules("lib", "lib", false));
    }

    @SuppressWarnings("unchecked")
    public List<Module> sysModules() {
        return (List<Module>) cache.computeIfAbsent("sysModules", k -> listModules("sys", "", true));
    }

    private List<Module> listModules(String kind, String parent, boolean requireReadmeOrHttp) {
        List<Module> out = new ArrayList<>();
        Path dir = parent.isEmpty() ? root : root.resolve(parent);
        if (!Files.isDirectory(dir)) return out;
        try (Stream<Path> s = Files.list(dir)) {
            s.filter(Files::isDirectory).sorted().forEach(p -> {
                String name = p.getFileName().toString();
                if (name.startsWith(".")) return;
                if (kind.equals("sys") && NOT_SYS_MODULES.contains(name)) return;
                if (kind.equals("lib") && name.equals("examples")) return;
                boolean readme = Files.isRegularFile(p.resolve("README.md"));
                if (requireReadmeOrHttp && !readme && !name.startsWith("http")) return;
                boolean api = Files.isRegularFile(root.resolve("docs/api/package/" + name + ".html")) && kind.equals("lib");
                String rel = parent.isEmpty() ? name : parent + "/" + name;
                out.add(new Module(kind, name, rel, readme, api));
            });
        } catch (IOException ignored) { }
        return Collections.unmodifiableList(out);
    }

    public Module module(String kind, String name) {
        List<Module> l = kind.equals("lib") ? libModules() : kind.equals("sys") ? sysModules() : List.of();
        for (Module m : l) if (m.getName().equals(name)) return m;
        return null;
    }

    /** HTTP-related modules (servers, protocol generations, web server technology). */
    public List<Module> httpModules() {
        List<Module> out = new ArrayList<>();
        for (Module m : sysModules()) if (isHttpish(m.getName())) out.add(m);
        for (Module m : libModules()) if (isHttpish(m.getName())) out.add(m);
        return out;
    }

    private static boolean isHttpish(String n) {
        return n.startsWith("http") || n.contains("web-server") || n.equals("server-edition") || n.equals("website")
                || n.equals("website-generator") || n.equals("connector") || n.equals("route") || n.equals("preferred-routers");
    }

    /** Repo-relative file paths inside a module (display listing; capped). */
    @SuppressWarnings("unchecked")
    public List<String> moduleFiles(Module m) {
        return (List<String>) cache.computeIfAbsent("files:" + m.getKind() + "/" + m.getName(), k -> {
            List<String> out = new ArrayList<>();
            Path base = root.resolve(m.getRelDir());
            try {
                Files.walkFileTree(base, Set.of(), 5, new SimpleFileVisitor<Path>() {
                    @Override public FileVisitResult preVisitDirectory(Path d, BasicFileAttributes a) {
                        String n = d.getFileName().toString();
                        return !d.equals(base) && (n.startsWith(".") || SKIP_DIRS.contains(n)) ? FileVisitResult.SKIP_SUBTREE : FileVisitResult.CONTINUE;
                    }
                    @Override public FileVisitResult visitFile(Path f, BasicFileAttributes a) {
                        String name = f.getFileName().toString();
                        if (out.size() >= MAX_MODULE_FILES) return FileVisitResult.TERMINATE;
                        if (!name.startsWith(".") && a.isRegularFile() && allowedName(name) && a.size() <= MAX_FILE_BYTES) {
                            out.add(base.relativize(f).toString().replace('\\', '/'));
                        }
                        return FileVisitResult.CONTINUE;
                    }
                });
            } catch (IOException ignored) { }
            out.sort(String.CASE_INSENSITIVE_ORDER);
            return Collections.unmodifiableList(out);
        });
    }

    /** Examples that belong with a module: examples/<name> and lib/examples/<name>. */
    public List<ExampleGroup> examplesFor(String name) {
        List<ExampleGroup> out = new ArrayList<>();
        for (String key : new String[] {"examples/" + name, "lib/examples/" + name}) {
            ExampleGroup g = exampleGroup(Routes.exampleGroupSlug(key));
            if (g != null) out.add(g);
        }
        return out;
    }

    // ---------------------------------------------------------------- API docs

    @SuppressWarnings("unchecked")
    public List<String> apiPackages() {
        return (List<String>) cache.computeIfAbsent("apiPackages", k -> {
            List<String> out = new ArrayList<>();
            Path dir = root.resolve("docs/api/package");
            if (Files.isDirectory(dir)) {
                try (Stream<Path> s = Files.list(dir)) {
                    s.map(p -> p.getFileName().toString()).filter(n -> n.endsWith(".html"))
                     .map(n -> n.substring(0, n.length() - 5)).sorted().forEach(out::add);
                } catch (IOException ignored) { }
            }
            return Collections.unmodifiableList(out);
        });
    }

    /** Raw API page source, or null. {@code page} is "index", "packages", "glossary", "authorship" or "package/<name>". */
    public String apiPage(String page) {
        Path p;
        if (page.startsWith("package/")) {
            String name = page.substring(8);
            if (!apiPackages().contains(name)) return null;
            p = root.resolve("docs/api/package/" + name + ".html");
        } else if (Set.of("index", "packages", "glossary", "authorship").contains(page)) {
            p = root.resolve("docs/api/" + page + ".html");
        } else return null;
        try {
            return Files.isRegularFile(p) ? new String(Files.readAllBytes(p), StandardCharsets.UTF_8) : null;
        } catch (IOException e) {
            return null;
        }
    }

    // ---------------------------------------------------------------- search

    public List<Hit> search(String q) {
        List<Hit> hits = new ArrayList<>();
        String needle = q.toLowerCase(Locale.ROOT).trim();
        if (needle.length() < 2) return hits;
        for (Tutorial t : tutorials()) {
            String md = readText(t.getRelPath());
            if (md == null) continue;
            String low = md.toLowerCase(Locale.ROOT);
            int at = low.indexOf(needle);
            if (at >= 0 || t.getTitle().toLowerCase(Locale.ROOT).contains(needle)) {
                hits.add(new Hit("Tutorial", t.getTitle(), t.getUrl(), snippet(md, Math.max(at, 0), needle.length())));
            }
        }
        for (ExampleGroup g : exampleGroups().values()) {
            for (ExampleFile f : g.getFiles()) {
                if (f.getRel().toLowerCase(Locale.ROOT).contains(needle) && hits.size() < 300) {
                    hits.add(new Hit("Example", f.getName(), f.getUrl(), g.getLabel() + "/" + f.getRel()));
                }
            }
        }
        for (Module m : sysModules()) if (m.getName().toLowerCase(Locale.ROOT).contains(needle)) hits.add(new Hit("Module", m.getName(), m.getUrl(), m.getRelDir()));
        for (Module m : libModules()) if (m.getName().toLowerCase(Locale.ROOT).contains(needle)) hits.add(new Hit("Library module", m.getName(), m.getUrl(), m.getRelDir()));
        for (String p : apiPackages()) if (p.toLowerCase(Locale.ROOT).contains(needle)) hits.add(new Hit("API package", p, "/api/package/" + Routes.seg(p), "docs/api/package/" + p + ".html"));
        return hits;
    }

    private static String snippet(String text, int at, int len) {
        int from = Math.max(0, at - 70);
        int to = Math.min(text.length(), at + len + 110);
        return (from > 0 ? "\u2026" : "") + text.substring(from, to).replaceAll("\\s+", " ") + (to < text.length() ? "\u2026" : "");
    }
}
