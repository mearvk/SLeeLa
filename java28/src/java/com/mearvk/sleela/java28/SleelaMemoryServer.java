package com.mearvk.sleela.java28;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicLong;

/**
 * The Java 28 SecureJDK memory model (J28-MEM-0001, Model A).
 *
 * <p>Sleela drives; this server <b>owns the objects</b>. It assigns opaque
 * handles, resolves them for reflective {@code new}/{@code call}/{@code get}/
 * {@code set}, and enforces the SecureJDK posture: an explicit class allow-list,
 * handle isolation, a bounded live-handle count, and a values-and-names-only
 * wire (no bytecode, no serialized objects, no DTD).
 *
 * <p>{@link #handleLine(String)} is the single dispatcher shared by BOTH
 * channels (port socket and JNI local feedback), so their behavior is identical
 * by construction.
 */
public final class SleelaMemoryServer {

    public static final String IDENTITY = "Java 28 SecureJDK memory host";
    public static final int PROTOCOL_VERSION = 1;

    private final Map<String, Class<?>> allowed = new HashMap<>();
    private final Map<Long, Object> heap = new HashMap<>();
    private final AtomicLong nextHandle = new AtomicLong(1);
    private final int maxHandles;
    private long highWater = 0;

    public SleelaMemoryServer() { this(4096); }

    public SleelaMemoryServer(int maxHandles) { this.maxHandles = maxHandles; }

    /** SecureJDK allow-list: only registered classes may be instantiated/called. */
    public synchronized void registerClass(Class<?> type) {
        allowed.put(type.getSimpleName(), type);
    }

    /** Retain a Java object on the heap and return its opaque handle. */
    public synchronized long retain(Object o) {
        if (o == null) return 0;
        if (heap.size() >= maxHandles) {
            throw new SecurityException("live-handle limit reached (" + maxHandles + ")");
        }
        long h = nextHandle.getAndIncrement();
        heap.put(h, o);
        if (heap.size() > highWater) highWater = heap.size();
        return h;
    }

    /** Resolve a handle to its live object, enforcing isolation. */
    public synchronized Object resolve(long handle) {
        if (handle == 0) return null;
        Object o = heap.get(handle);
        if (o == null) throw new IllegalArgumentException("invalid or released handle h:" + handle);
        return o;
    }

    // ---- Shared dispatcher (used by port + JNI) ----------------------------

    /**
     * Handle one request line and return one response line, per J28-MEM-0001
     * §3/§4. Never throws: failures come back as {@code ERR}.
     */
    public String handleLine(String line) {
        try {
            List<String> toks = tokenize(line);
            if (toks.isEmpty()) return err("empty request");
            String op = toks.get(0);
            switch (op) {
                case "hello":   return ok(SleelaValue.ofStr(IDENTITY + " v" + PROTOCOL_VERSION));
                case "new":     return doNew(toks);
                case "call":    return doCall(toks);
                case "get":     return doGet(toks);
                case "set":     return doSet(toks);
                case "free":    return doFree(toks);
                case "describe":return doDescribe(toks);
                case "stats":   return doStats();
                case "bye":     return ok(SleelaValue.NULL);
                default:        return err("unknown op '" + op + "'");
            }
        } catch (SecurityException se) {
            return err("security: " + se.getMessage());
        } catch (Exception e) {
            String m = e.getMessage();
            return err(m == null ? e.getClass().getSimpleName() : m);
        }
    }

    private synchronized String doNew(List<String> t) throws Exception {
        if (t.size() < 2) return err("new: missing class name");
        Class<?> type = allowed.get(t.get(1));
        if (type == null) return err("class not allow-listed: " + t.get(1));
        Object[] args = decodeArgs(t, 2);
        Constructor<?> ctor = matchConstructor(type, args);
        if (ctor == null) return err("no matching constructor for " + t.get(1) + "/" + args.length);
        Object obj = ctor.newInstance(args);
        return ok(SleelaValue.ofHandle(retain(obj)));
    }

    private synchronized String doCall(List<String> t) throws Exception {
        if (t.size() < 3) return err("call: need handle and method");
        Object target = resolve(handleArg(t.get(1)));
        String method = t.get(2);
        Object[] args = decodeArgs(t, 3);
        Method m = matchMethod(target.getClass(), method, args);
        if (m == null) return err("no matching method " + method + "/" + args.length);
        Object result = m.invoke(target, args);
        return ok(SleelaValue.fromJava(result, this));
    }

    private synchronized String doGet(List<String> t) throws Exception {
        if (t.size() < 3) return err("get: need handle and field");
        Object target = resolve(handleArg(t.get(1)));
        Field f = target.getClass().getField(t.get(2));
        return ok(SleelaValue.fromJava(f.get(target), this));
    }

    private synchronized String doSet(List<String> t) throws Exception {
        if (t.size() < 4) return err("set: need handle, field, value");
        Object target = resolve(handleArg(t.get(1)));
        Field f = target.getClass().getField(t.get(2));
        Object v = SleelaValue.decode(t.get(3)).toJava(this);
        f.set(target, coerce(v, f.getType()));
        return ok(SleelaValue.NULL);
    }

    private synchronized String doFree(List<String> t) {
        if (t.size() < 2) return err("free: need handle");
        long h = handleArg(t.get(1));
        heap.remove(h);
        return ok(SleelaValue.NULL);
    }

    private synchronized String doDescribe(List<String> t) {
        if (t.size() < 2) return err("describe: need handle");
        Object o = resolve(handleArg(t.get(1)));
        Class<?> c = o.getClass();
        StringBuilder sb = new StringBuilder(c.getSimpleName()).append(" fields=[");
        boolean first = true;
        for (Field f : c.getFields()) { if (!first) sb.append(','); sb.append(f.getName()); first = false; }
        sb.append("] methods=[");
        first = true;
        for (Method m : c.getMethods()) {
            if (m.getDeclaringClass() == Object.class) continue;
            if (!first) sb.append(','); sb.append(m.getName()); first = false;
        }
        sb.append(']');
        return ok(SleelaValue.ofStr(sb.toString()));
    }

    private synchronized String doStats() {
        return ok(SleelaValue.ofStr("live=" + heap.size() + " highWater=" + highWater
                + " max=" + maxHandles));
    }

    // ---- helpers -----------------------------------------------------------

    private long handleArg(String tok) {
        SleelaValue v = SleelaValue.decode(tok);
        if (!v.isHandle()) throw new IllegalArgumentException("expected handle, got " + tok);
        return v.i;
    }

    private Object[] decodeArgs(List<String> t, int from) {
        List<Object> out = new ArrayList<>();
        for (int i = from; i < t.size(); i++) out.add(SleelaValue.decode(t.get(i)).toJava(this));
        return out.toArray();
    }

    private Constructor<?> matchConstructor(Class<?> type, Object[] args) {
        for (Constructor<?> c : type.getConstructors()) {
            if (c.getParameterCount() == args.length && paramsMatch(c.getParameterTypes(), args)) return c;
        }
        return null;
    }

    private Method matchMethod(Class<?> type, String name, Object[] args) {
        for (Method m : type.getMethods()) {
            if (m.getName().equals(name) && m.getParameterCount() == args.length
                    && paramsMatch(m.getParameterTypes(), args)) return m;
        }
        return null;
    }

    private boolean paramsMatch(Class<?>[] params, Object[] args) {
        for (int i = 0; i < params.length; i++) {
            Object a = args[i];
            if (a == null) { if (params[i].isPrimitive()) return false; continue; }
            if (!assignable(params[i], a.getClass())) return false;
        }
        return true;
    }

    private boolean assignable(Class<?> param, Class<?> argType) {
        if (param.isAssignableFrom(argType)) return true;
        // numeric widening across the value contract (Long/Double from the wire)
        if ((param == long.class || param == Long.class) && argType == Long.class) return true;
        if ((param == int.class || param == Integer.class) && argType == Long.class) return true;
        if ((param == double.class || param == Double.class) && argType == Double.class) return true;
        if ((param == boolean.class || param == Boolean.class) && argType == Boolean.class) return true;
        return false;
    }

    /** Coerce a wire value (Long/Double/Boolean/String) to a specific field type. */
    private Object coerce(Object v, Class<?> target) {
        if (v == null) return null;
        if (v instanceof Long l) {
            if (target == int.class || target == Integer.class) return l.intValue();
            if (target == long.class || target == Long.class) return l;
        }
        if (v instanceof Double d) {
            if (target == double.class || target == Double.class) return d;
        }
        return v;
    }

    /** Args after {@code call target method}: reflective args coerced to param types. */
    private String ok(SleelaValue v) { return "OK " + v.encode(); }
    private String err(String msg) { return "ERR " + SleelaValue.ofStr(msg).encode(); }

    /**
     * Tokenize a request line honoring length-prefixed strings
     * ({@code s:<len>:<utf8>}), which may contain spaces (J28-MEM-0001 §4).
     */
    static List<String> tokenize(String line) {
        List<String> out = new ArrayList<>();
        byte[] raw = line.getBytes(StandardCharsets.UTF_8);
        int i = 0, n = raw.length;
        while (i < n) {
            while (i < n && raw[i] == ' ') i++;
            if (i >= n) break;
            // Detect a length-prefixed string token: s:<len>:<bytes>
            if (i + 1 < n && raw[i] == 's' && raw[i + 1] == ':') {
                int p = i + 2, len = 0;
                boolean haveDigits = false;
                while (p < n && raw[p] >= '0' && raw[p] <= '9') { len = len * 10 + (raw[p] - '0'); p++; haveDigits = true; }
                if (haveDigits && p < n && raw[p] == ':') {
                    int start = p + 1;
                    int end = start + len;
                    if (end > n) throw new IllegalArgumentException("string length overruns line");
                    String s = new String(raw, start, len, StandardCharsets.UTF_8);
                    out.add("s:" + len + ":" + s);
                    i = end;
                    continue;
                }
            }
            int start = i;
            while (i < n && raw[i] != ' ') i++;
            out.add(new String(raw, start, i - start, StandardCharsets.UTF_8));
        }
        return out;
    }
}
