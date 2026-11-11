import java.io.*;
import java.lang.reflect.*;
import java.util.*;
import java.util.concurrent.*;

/**
 * Behavioral probe of JDK accessor contracts, for
 * lib/java/tools/jdk_api_accessors.py.
 *
 * Reads type names (one per line) on stdin. For each concrete public class
 * it picks a public constructor, instantiates it twice with two different
 * argument sets, and records -- by calling the public JDK API only, never
 * reading implementation source -- which observations hold on BOTH runs:
 *
 *   param      getter g() returns exactly constructor argument i
 *              (== for references, equals for primitives / String)
 *   constant   getter g() returns the same primitive / String value on both
 *              runs and on a third instance built 5 ms later: the initial
 *              state of that property, independent of the arguments
 *   setter     after s(v) the paired getter returns v, for two values
 *              (not probed for beans that fire property-change events)
 *
 * Only get* / is* / has* getters, and accessors named like a constructor
 * parameter, are probed. Output: one JSON object per class that yielded
 * at least one observation. Each class probe runs on its own thread with a
 * time limit; classes in packages or with names that touch the host (files,
 * sockets, threads, processes, windows, audio, printing) are never touched.
 *
 * usage: javac -d /tmp/probe ApiAccessorProbe.java
 *        java -Djava.awt.headless=true -cp /tmp/probe ApiAccessorProbe < types.txt > accessors.jsonl
 */
public class ApiAccessorProbe {
    static final String UNSAFE = ".*(File|Socket|Server|Thread|Process|Runtime|Executor|Channel|Robot|Desktop|Print|Audio|Clip|Mixer|Sound|Midi|Sequencer|Synthesizer|Window|Frame|Dialog|Applet|Timer|Console|Selector|Pipe|Lock|Semaphore|Latch|Barrier|Phaser|Exchanger|ClassLoader|Logger|LogManager|Preferences|Toolkit|Graphics|Image|Font|Cursor|Clipboard|Datagram|URLConnection|HttpClient|Registry|Naming|InitialContext|ManagementFactory|MBeanServer|ScriptEngine|Compiler|Service|Provider|KeyStore|SecureRandom|Random|Instant|Clock|Date|Calendar|Time|System|Shutdown|Stack|Scanner|Inflater|Deflater|Zip|Jar|Cleaner|Reference).*";

    static String q(String s) {
        StringBuilder b = new StringBuilder("\"");
        for (char c : s.toCharArray()) {
            if (c == '"' || c == '\\') b.append('\\').append(c);
            else if (c < 32 || c > 126) b.append(String.format("\\u%04x", (int) c));
            else b.append(c);
        }
        return b.append('"').toString();
    }

    /** Argument for parameter i of type t in run r (0 or 1), scheme k. */
    static Object arg(Class<?> t, int i, int r, int k) throws Exception {
        int base = (k == 0) ? (r == 0 ? 3 : 5) : (r == 0 ? 1 : 2);
        int v = base + i * 2 + (k == 0 ? 0 : 0);
        if (k == 0) v = (r == 0 ? 11 : 23) + i * 37;
        if (t == int.class) return v;
        if (t == long.class) return (long) v * 1000003L;
        if (t == short.class) return (short) v;
        if (t == byte.class) return (byte) (v % 100);
        if (t == char.class) return (char) ('a' + (v % 26));
        if (t == double.class) return v + 0.5;
        if (t == float.class) return (float) (v + 0.25);
        if (t == boolean.class) return ((i + r) % 2) == 0;
        if (t == String.class) return "s" + r + "_" + i + "_" + k;
        if (t == Object.class) return new Object();
        if (t == Integer.class) return Integer.valueOf(v);
        if (t == Long.class) return Long.valueOf(v);
        if (t == Double.class) return Double.valueOf(v + 0.5);
        if (t == Boolean.class) return Boolean.valueOf(((i + r) % 2) == 0);
        if (t.isArray() || t.isInterface() || Modifier.isAbstract(t.getModifiers()) || t.isEnum()) return null;
        if (t.getName().matches(UNSAFE)) return null;
        try {
            Constructor<?> c = t.getConstructor();
            return c.newInstance();
        } catch (Throwable e) { return null; }
    }

    static boolean same(Object a, Object b, Class<?> t) {
        if (a == null || b == null) return false;
        if (t.isPrimitive() || t == String.class || t == Integer.class || t == Long.class || t == Double.class || t == Boolean.class) return a.equals(b);
        return a == b;
    }

    static String literal(Object v, Class<?> t) {
        if (v == null) return null;
        if (t == String.class) { String s = (String) v; for (char c : s.toCharArray()) if (c < 32 || c > 126) return null; return q(s); }
        if (t == boolean.class) return v.toString();
        if (t == int.class || t == long.class || t == short.class || t == byte.class) return v.toString();
        if (t == char.class) return Integer.toString((Character) v);
        if (t == double.class || t == float.class) {
            double d = ((Number) v).doubleValue();
            if (Double.isNaN(d) || Double.isInfinite(d)) return null;
            if (d != 0 && Math.abs(d) < 2.2250738585072014E-308) return null;
            String s = t == float.class ? Double.toString((double) (Float) v) : Double.toString(d);
            return q(s);
        }
        return null;
    }

    static boolean getterName(String n) {
        for (String p : new String[] {"get", "is", "has"})
            if (n.startsWith(p) && n.length() > p.length() && Character.isUpperCase(n.charAt(p.length()))) return true;
        return false;
    }

    /** Beans that notify listeners on change: a plain stored setter would miss the event. */
    static boolean firesPropertyChanges(Class<?> c) {
        for (Method m : c.getMethods()) if (m.getName().equals("addPropertyChangeListener")) return true;
        return false;
    }

    static String probe(Class<?> c) throws Exception {
        if (c.isInterface() || Modifier.isAbstract(c.getModifiers()) || c.isEnum() || c.isRecord() || Throwable.class.isAssignableFrom(c)) return null;
        if (!Modifier.isPublic(c.getModifiers()) || c.getName().matches(UNSAFE)) return null;
        for (Class<?> k = c; k != null; k = k.getSuperclass()) if (k.getName().matches(UNSAFE)) return null;
        List<Constructor<?>> ctors = new ArrayList<>();
        for (Constructor<?> k : c.getConstructors()) if (!k.isVarArgs()) ctors.add(k);
        ctors.sort((a, b) -> b.getParameterCount() - a.getParameterCount());
        List<Method> getters = new ArrayList<>();
        Map<String, Method> getterByName = new HashMap<>();
        for (Method m : c.getDeclaredMethods()) {
            int mo = m.getModifiers();
            if (!Modifier.isPublic(mo) || Modifier.isStatic(mo) || m.isSynthetic() || m.isBridge() || m.getParameterCount() != 0 || m.getReturnType() == void.class) continue;
            getters.add(m);
            getterByName.put(m.getName(), m);
        }
        if (getters.isEmpty()) return null;
        for (Constructor<?> ctor : ctors) {
            Class<?>[] pt = ctor.getParameterTypes();
            for (int k = 0; k < 2; k++) {
                Object[] a0 = new Object[pt.length], a1 = new Object[pt.length];
                for (int i = 0; i < pt.length; i++) { a0[i] = arg(pt[i], i, 0, k); a1[i] = arg(pt[i], i, 1, k); }
                Object o0, o1, o2;
                try {
                    o0 = ctor.newInstance(a0); o1 = ctor.newInstance(a1);
                    Thread.sleep(5);
                    o2 = ctor.newInstance(a0);
                } catch (Throwable e) { continue; }
                List<String> obs = new ArrayList<>();
                Set<String> done = new HashSet<>();
                // An initial-state value is only trusted when every argument differs
                // between the runs (a default-constructed or null argument is the
                // same in both runs, so a value derived from it would look constant).
                boolean varied = true;
                for (Class<?> t : pt)
                    if (!(t.isPrimitive() || t == String.class || t == Object.class || t == Integer.class || t == Long.class || t == Double.class || t == Boolean.class)) varied = false;
                for (Method g : getters) {
                    Object r0, r1, r2;
                    try { r0 = g.invoke(o0); r1 = g.invoke(o1); r2 = g.invoke(o2); } catch (Throwable e) { continue; }
                    int hit = -1, hits = 0;
                    for (int i = 0; i < pt.length; i++) {
                        if (pt[i] != g.getReturnType()) continue;
                        if (same(r0, a0[i], pt[i]) && same(r1, a1[i], pt[i])) { hit = i; hits++; }
                    }
                    if (hits == 1) { obs.add("[\"param\"," + q(g.getName()) + "," + hit + "]"); done.add(g.getName()); continue; }
                    if (!getterName(g.getName()) || !varied) continue;
                    String l0 = literal(r0, g.getReturnType()), l1 = literal(r1, g.getReturnType()), l2 = literal(r2, g.getReturnType());
                    if (l0 != null && l0.equals(l1) && l0.equals(l2)) { obs.add("[\"constant\"," + q(g.getName()) + "," + l0 + "]"); done.add(g.getName()); }
                }
                // setter pairs, probed on a fresh instance built from run-0 arguments
                for (Method s : (firesPropertyChanges(c) ? new Method[0] : c.getDeclaredMethods())) {
                    int mo = s.getModifiers();
                    if (!Modifier.isPublic(mo) || Modifier.isStatic(mo) || s.isSynthetic() || s.isBridge() || s.getParameterCount() != 1 || !s.getName().startsWith("set") || s.getName().length() < 4) continue;
                    String prop = s.getName().substring(3);
                    Method g = getterByName.get("get" + prop);
                    if (g == null) g = getterByName.get("is" + prop);
                    if (g == null || !done.contains(g.getName()) || g.getReturnType() != s.getParameterTypes()[0]) continue;
                    Class<?> t = s.getParameterTypes()[0];
                    try {
                        Object o = ctor.newInstance(a0);
                        Object v1 = arg(t, 7, 0, 0), v2 = arg(t, 8, 1, 0);
                        if (v1 == null || v2 == null) continue;
                        s.invoke(o, v1); Object r1 = g.invoke(o);
                        s.invoke(o, v2); Object r2 = g.invoke(o);
                        if (same(r1, v1, t) && same(r2, v2, t)) obs.add("[\"setter\"," + q(s.getName()) + "," + q(g.getName()) + "]");
                    } catch (Throwable e) { }
                }
                if (obs.isEmpty()) return null;
                StringBuilder p = new StringBuilder();
                for (int i = 0; i < pt.length; i++) { if (i > 0) p.append(","); p.append(q(pt[i].getTypeName())); }
                return "{\"name\":" + q(c.getName().replace('$', '.')) + ",\"ctor\":[" + p + "],\"scheme\":" + k + ",\"obs\":[" + String.join(",", obs) + "]}";
            }
        }
        return null;
    }

    public static void main(String[] args) throws Exception {
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        List<String> names = new ArrayList<>();
        for (String l; (l = in.readLine()) != null; ) if (!l.isBlank()) names.add(l.trim());
        ExecutorService ex = Executors.newCachedThreadPool(r -> { Thread t = new Thread(r); t.setDaemon(true); return t; });
        PrintStream out = new PrintStream(new FileOutputStream(FileDescriptor.out), true, "UTF-8");
        PrintStream quiet = new PrintStream(OutputStream.nullOutputStream());
        System.setOut(quiet);
        System.setErr(quiet);
        int found = 0;
        for (String n : names) {
            Future<String> f = ex.submit(() -> {
                Class<?> c = Class.forName(n, false, ClassLoader.getSystemClassLoader());
                return probe(c);
            });
            try {
                String r = f.get(2, TimeUnit.SECONDS);
                if (r != null) { out.println(r); found++; }
            } catch (Throwable e) { f.cancel(true); }
        }
        out.flush();
        System.exit(found >= 0 ? 0 : 1);
    }
}
