package org.sleela.site.runtime;

import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/** Bundled starter programs (resources/org/sleela/site/runtime/samples). Each one is checked in a real sandbox before shipping. */
public final class Samples {
    public static final class Sample {
        private final String id, title, source;
        Sample(String id, String title, String source) { this.id = id; this.title = title; this.source = source; }
        public String getId() { return id; }
        public String getTitle() { return title; }
        public String getSource() { return source; }
    }

    private static final String BASE = "/org/sleela/site/runtime/samples/";
    private static final Pattern ENTRY = Pattern.compile("\"file\"\\s*:\\s*\"([^\"]+)\"\\s*,\\s*\"title\"\\s*:\\s*\"([^\"]+)\"");
    private static volatile Map<String, Sample> cache;

    public static Map<String, Sample> all() {
        Map<String, Sample> c = cache;
        if (c != null) return c;
        Map<String, Sample> m = new LinkedHashMap<>();
        String idx = read("index.json");
        if (idx != null) {
            Matcher e = ENTRY.matcher(idx);
            while (e.find()) {
                String src = read(e.group(1));
                if (src != null) m.put(e.group(1).replace(".sleela", ""), new Sample(e.group(1).replace(".sleela", ""), e.group(2), src));
            }
        }
        cache = m;
        return m;
    }

    public static List<Sample> list() { return new ArrayList<>(all().values()); }

    private static String read(String name) {
        try (InputStream in = Samples.class.getResourceAsStream(BASE + name)) {
            return in == null ? null : new String(in.readAllBytes(), StandardCharsets.UTF_8);
        } catch (IOException e) {
            return null;
        }
    }

    private Samples() {}
}
