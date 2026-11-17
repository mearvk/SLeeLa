package org.sleela.site;

import java.util.List;

/** Plain bean types (getter style) so they work in EL on every servlet container. */
public final class Model {
    private Model() {}

    public static final class Tutorial {
        private final String slug, title, relPath;
        public Tutorial(String slug, String title, String relPath) { this.slug = slug; this.title = title; this.relPath = relPath; }
        public String getSlug() { return slug; }
        public String getTitle() { return title; }
        public String getRelPath() { return relPath; }
        public String getUrl() { return "/tutorials/" + Routes.seg(slug); }
    }

    public static final class ExampleFile {
        private final String groupSlug, rel, lang;
        private final long size;
        public ExampleFile(String groupSlug, String rel, String lang, long size) { this.groupSlug = groupSlug; this.rel = rel; this.lang = lang; this.size = size; }
        public String getGroupSlug() { return groupSlug; }
        public String getRel() { return rel; }
        public String getName() { return rel.substring(rel.lastIndexOf('/') + 1); }
        public String getLang() { return lang; }
        public long getSize() { return size; }
        public String getUrl() { return "/examples/" + Routes.seg(groupSlug) + "/" + Routes.path(rel); }
    }

    public static final class ExampleGroup {
        private final String slug, label, baseRel;
        private final List<ExampleFile> files;
        public ExampleGroup(String slug, String label, String baseRel, List<ExampleFile> files) { this.slug = slug; this.label = label; this.baseRel = baseRel; this.files = files; }
        public String getSlug() { return slug; }
        public String getLabel() { return label; }
        public String getBaseRel() { return baseRel; }
        public List<ExampleFile> getFiles() { return files; }
        public int getCount() { return files.size(); }
        public String getUrl() { return "/examples/" + Routes.seg(slug); }
    }

    public static final class Module {
        private final String kind, name, relDir;
        private final boolean readme, api;
        public Module(String kind, String name, String relDir, boolean readme, boolean api) { this.kind = kind; this.name = name; this.relDir = relDir; this.readme = readme; this.api = api; }
        public String getKind() { return kind; }
        public String getName() { return name; }
        public String getRelDir() { return relDir; }
        public boolean isReadme() { return readme; }
        public boolean isApi() { return api; }
        public String getUrl() { return "/modules/" + kind + "/" + Routes.seg(name); }
        public String getApiUrl() { return "/api/package/" + Routes.seg(name); }
    }

    public static final class Hit {
        private final String kind, title, url, snippet;
        public Hit(String kind, String title, String url, String snippet) { this.kind = kind; this.title = title; this.url = url; this.snippet = snippet; }
        public String getKind() { return kind; }
        public String getTitle() { return title; }
        public String getUrl() { return url; }
        public String getSnippet() { return snippet; }
    }

    public static final class Crumb {
        private final String label, url;
        public Crumb(String label, String url) { this.label = label; this.url = url; }
        public String getLabel() { return label; }
        public String getUrl() { return url; }
    }
}
