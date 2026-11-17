package org.sleela.site.runtime;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.function.Function;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Decides what may run on the website. This is the second layer behind the OS sandbox.
 *
 * SLeeLa has no capability system of its own: osRun is system() via /bin/sh, openFile/unlinkFile take any path,
 * connect/listen take any host, Munction file:/tcp: reach handles touch disk and the network, and spawn makes real
 * threads. So programs that name any host-reaching builtin are refused before they are handed to the compiler.
 *
 * Because a capitalized identifier auto-loads a matching unit from lib/ and those units call the same builtins, the
 * check also follows referenced library units transitively and applies the same deny-list to them. Comments and string
 * literals are stripped first so a banned word inside them never trips the check.
 *
 * The identifier list is taken from the compiler builtin table (impl/frontend/semantic.cpp builtin(), the os, osls, 
 * synchro, bestOf, Munction and native families) and sleela_os.c. It is deliberately broad.
 */
public final class SourcePolicy {

    /** Builtin identifiers (and keywords) that reach the host. Matched as whole words after comments/strings are removed. */
    static final Set<String> DENY = Set.of(
            // process execution
            "osRun", "osSpawn", "osWait", "osKill", "osProcessClose",
            // filesystem write/delete and directory mutation
            "openFile", "unlinkFile", "fifoCreate", "npfsCreate", "pipe", "pipePeer",
            "osRemove", "osRename", "osMakeDir", "osChangeDir", "osSetEnv",
            // environment and host introspection that can leak secrets
            "osGetEnv", "osCapability", "osHostName", "osUserName", "osTempDir", "osCurrentDir", "osProcessId",
            "oslsEnumerateEnvironment", "oslsGetEnvironmentValue", "oslsListDirectory", "oslsGetFileInfo",
            "oslsEnumerateProcesses", "oslsProcessInfo", "oslsNetworkResolve", "oslsGetNetworkInterfaces",
            // raw sockets and network timing
            "sockread", "sockwrite", "sockclose", "timeNtp",
            // reach handles (file:, tcp:, net: URIs) and native / FFI / dynamic loading
            "Munction", "nativeOpen", "nativeCall", "nativeClose");

    /**
     * Builtins whose names are also ordinary method names in the Java-mirror library units (Consumer.accept, Reader.lock,
     * Thread.join ...). In the visitor's own program they are always refused. In a library unit only a bare call, not a
     * declaration or a method call on an object, counts.
     */
    static final Set<String> SOFT = Set.of("listen", "accept", "connect", "spawn", "join", "lock", "unlock", "recv", "send");

    /** Types the compiler treats as built in; their Java-mirror units are not followed. */
    static final Set<String> BUILTIN_TYPES = Set.of("String", "Main");

    /** Keywords that must not appear as a declaration (native methods, dynamite/permissible connectors). */
    static final Pattern NATIVE_DECL = Pattern.compile("(^|[^A-Za-z0-9_])native\\s+[A-Za-z_]");
    static final Pattern DYNAMITE = Pattern.compile("(^|[^A-Za-z0-9_])(dynamite|permissible)\\s+connector\\b");
    /** Prefix families denied wholesale. */
    static final String[] DENY_PREFIX = {"Munction", "synchro", "bestOf", "osls"};
    /** Identifier reference. */
    private static final Pattern WORD = Pattern.compile("[A-Za-z_][A-Za-z0-9_]*");

    public static final class Verdict {
        public final boolean allowed;
        public final List<String> reasons;
        Verdict(boolean allowed, List<String> reasons) { this.allowed = allowed; this.reasons = reasons; }
    }

    /** resolver maps a capitalized unit name to its source text, or null if it is not a known library unit. */
    public static Verdict check(String source, int maxBytes, Function<String, String> libResolver) {
        List<String> reasons = new ArrayList<>();
        if (source == null || source.isBlank()) return new Verdict(false, List.of("The program is empty."));
        if (source.getBytes(java.nio.charset.StandardCharsets.UTF_8).length > maxBytes) {
            return new Verdict(false, List.of("The program is larger than the " + (maxBytes / 1024) + " KB limit."));
        }
        if (source.indexOf('\0') >= 0) return new Verdict(false, List.of("The program contains a null byte."));
        if (!isPlainText(source)) return new Verdict(false, List.of("The program must be plain UTF-8 text."));

        Set<String> seen = new LinkedHashSet<>();
        Deque<String> queue = new ArrayDeque<>();
        queue.add("__main__");
        boolean first = true;
        int units = 0;
        while (!queue.isEmpty()) {
            if (units >= MAX_UNITS) {
                reasons.add("The program pulls in more than " + MAX_UNITS + " library units, which is more than the website will check.");
                break;
            }
            String unit = queue.poll();
            boolean isMain = first;
            String text = isMain ? source : libResolver.apply(unit);
            first = false;
            units++;
            if (text == null) continue;
            String where = isMain ? "your program" : "library unit " + unit;
            String code = stripCommentsAndStrings(text);
            scanOne(code, where, isMain, reasons);
            if (isMain) {
                // names the program declares itself resolve to its own classes; the loader does not fetch a library unit for them
                Matcher d = DECLARED.matcher(code);
                while (d.find()) seen.add(d.group(1));
            }
            for (String ref : capitalizedRefs(code)) if (!BUILTIN_TYPES.contains(ref) && seen.add(ref)) queue.add(ref);
        }
        return new Verdict(reasons.isEmpty(), reasons.isEmpty() ? List.of("Allowed.") : dedupe(reasons));
    }

    static final int MAX_UNITS = 600;

    private static void scanOne(String code, String where, boolean isMain, List<String> reasons) {
        Matcher m = WORD.matcher(code);
        Set<String> hits = new LinkedHashSet<>();
        while (m.find()) {
            String w = m.group();
            if (DENY.contains(w)) hits.add(w);
            else if (SOFT.contains(w)) {
                if (isMain || isBareCall(code, m.start(), m.end())) hits.add(w);
            } else for (String pre : DENY_PREFIX) {
                if (w.length() > pre.length() && w.startsWith(pre) && Character.isUpperCase(w.charAt(pre.length()))) { hits.add(w); break; }
            }
        }
        for (String h : hits) reasons.add("Host access is not allowed on the website: " + where + " uses " + h + ".");
        // native members in library units only declare a JDK API; they cannot load code without nativeOpen, which is refused above
        if (isMain && NATIVE_DECL.matcher(code).find()) reasons.add("Native methods are not allowed: " + where + " declares a native member.");
        if (DYNAMITE.matcher(code).find()) reasons.add("Dynamite/permissible connectors are not allowed: " + where + ".");
    }

    /** True for name( written as a plain function call: not after a dot, and not a declaration such as "void accept(". */
    private static boolean isBareCall(String code, int start, int end) {
        int k = end;
        while (k < code.length() && Character.isWhitespace(code.charAt(k))) k++;
        if (k >= code.length() || code.charAt(k) != '(') return false;
        int p = start - 1;
        while (p >= 0 && Character.isWhitespace(code.charAt(p))) p--;
        if (p < 0) return true;
        char c = code.charAt(p);
        if (c == '.') return false;
        if (Character.isLetterOrDigit(c) || c == '_' || c == '>' || c == ']') {
            // previous token is a word: a declaration unless it is a statement keyword
            int q = p;
            while (q >= 0 && (Character.isLetterOrDigit(code.charAt(q)) || code.charAt(q) == '_')) q--;
            String prev = code.substring(q + 1, p + 1);
            return prev.equals("return") || prev.equals("else");
        }
        return true;
    }

    /** Capitalized identifiers that are not immediately a method call (SomeClass.foo()) are candidate unit names. */
    static Set<String> capitalizedRefs(String code) {
        Set<String> out = new LinkedHashSet<>();
        Matcher m = WORD.matcher(code);
        while (m.find()) {
            String w = m.group();
            if (!w.isEmpty() && Character.isUpperCase(w.charAt(0)) && w.length() > 1 && !w.equals(w.toUpperCase(java.util.Locale.ROOT))) {
                out.add(w);
            }
        }
        return out;
    }

    /**
     * Blank out comments and string/char literals exactly as the SLeeLa lexer does (impl/frontend/lexer.cpp skipTrivia,
     * makeString, makeChar): '#' and '//' run to end of line; '<<' ... '>>' and a one-line '< ... >' are comments only
     * when they start at a line prefix; "..." and '.' are literals. Anything that does not match those shapes is left as
     * code, so a mismatch can only make the check stricter, never hide a call. Where the real lexer would raise an error
     * the program cannot run at all.
     */
    static String stripCommentsAndStrings(String s) {
        StringBuilder out = new StringBuilder(s.length());
        int n = s.length();
        int i = 0;
        while (i < n) {
            char c = s.charAt(i);
            if (c == '#' || (c == '/' && i + 1 < n && s.charAt(i + 1) == '/')) {
                while (i < n && s.charAt(i) != '\n') i++;
                continue;
            }
            if (c == '<' && atLinePrefix(s, i)) {
                if (i + 1 < n && s.charAt(i + 1) == '<') {
                    int end = s.indexOf(">>", i + 2);
                    if (end >= 0) { i = end + 2; out.append(' '); continue; }
                } else {
                    int e = i + 1;
                    while (e < n && s.charAt(e) != '\n' && s.charAt(e) != '>') e++;
                    if (e < n && s.charAt(e) == '>') {
                        while (i < n && s.charAt(i) != '\n') i++;
                        continue;
                    }
                }
            }
            if (c == '"') {
                int j = i + 1;
                boolean closed = false;
                while (j < n) {
                    char d = s.charAt(j);
                    if (d == '\\') { j += 2; continue; }
                    if (d == '"') { closed = true; break; }
                    j++;
                }
                if (closed) { i = j + 1; out.append(' '); continue; }
            }
            if (c == '\'') {
                int j = i + 1;
                if (j < n && s.charAt(j) == '\\') {
                    j += 2;
                    while (j < n && j < i + 12 && s.charAt(j) != '\'' && s.charAt(j) != '\n') j++;
                } else if (j < n && s.charAt(j) != '\'' && s.charAt(j) != '\n') {
                    j++;
                    while (j < n && (s.charAt(j) & 0xC0) == 0x80) j++;
                }
                if (j < n && s.charAt(j) == '\'') { i = j + 1; out.append(' '); continue; }
            }
            out.append(c);
            i++;
        }
        return out.toString();
    }

    private static boolean atLinePrefix(String s, int pos) {
        for (int p = pos; p > 0 && s.charAt(p - 1) != '\n'; p--) {
            char b = s.charAt(p - 1);
            if (b != ' ' && b != '\t' && b != '\r') return false;
        }
        return true;
    }

    private static final Pattern DECLARED = Pattern.compile("\\b(?:class|struct|interface|enum)\\s+([A-Za-z_][A-Za-z0-9_]*)");

    private static boolean isPlainText(String s) {
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c < 0x20 && c != '\n' && c != '\r' && c != '\t') return false;
        }
        return true;
    }

    private static List<String> dedupe(List<String> in) {
        return new ArrayList<>(new LinkedHashSet<>(in));
    }

    private SourcePolicy() {}
}
