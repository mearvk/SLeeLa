package com.mearvk.sleela.java28;

import java.nio.charset.StandardCharsets;

/**
 * Typed value on the Sleela &lt;-&gt; Java 28 SecureJDK link (J28-MEM-0001 §2, §4).
 *
 * <p>Mirrors Sleela's SLValue tagged union plus the opaque object handle:
 * {@code n}=null, {@code i}=int64, {@code d}=double, {@code b}=bool,
 * {@code s}=string, {@code h}=object handle. Handles are opaque to Sleela; the
 * memory host resolves them against its handle table.
 */
public final class SleelaValue {
    public enum Kind { NULL, INT, DOUBLE, BOOL, STR, HANDLE }

    public final Kind kind;
    public final long i;      // INT and HANDLE
    public final double d;    // DOUBLE
    public final boolean b;   // BOOL
    public final String s;    // STR

    private SleelaValue(Kind kind, long i, double d, boolean b, String s) {
        this.kind = kind; this.i = i; this.d = d; this.b = b; this.s = s;
    }

    public static final SleelaValue NULL = new SleelaValue(Kind.NULL, 0, 0, false, null);
    public static SleelaValue ofInt(long v)     { return new SleelaValue(Kind.INT, v, 0, false, null); }
    public static SleelaValue ofDouble(double v){ return new SleelaValue(Kind.DOUBLE, 0, v, false, null); }
    public static SleelaValue ofBool(boolean v) { return new SleelaValue(Kind.BOOL, 0, 0, v, null); }
    public static SleelaValue ofStr(String v)   { return new SleelaValue(Kind.STR, 0, 0, false, v == null ? "" : v); }
    public static SleelaValue ofHandle(long h)  { return new SleelaValue(Kind.HANDLE, h, 0, false, null); }

    public boolean isHandle() { return kind == Kind.HANDLE; }

    /** Encode this value into the wire token form (J28-MEM-0001 §4). */
    public String encode() {
        switch (kind) {
            case NULL:   return "n";
            case INT:    return "i:" + i;
            case DOUBLE: return "d:" + d;
            case BOOL:   return "b:" + (b ? "1" : "0");
            case HANDLE: return "h:" + i;
            case STR: {
                byte[] bytes = s.getBytes(StandardCharsets.UTF_8);
                return "s:" + bytes.length + ":" + s;
            }
            default: throw new IllegalStateException("unknown kind " + kind);
        }
    }

    /**
     * Convert to an actual Java argument for reflective ctor/method invocation.
     * Handles resolve to live objects via the supplied memory host.
     */
    public Object toJava(SleelaMemoryServer host) {
        switch (kind) {
            case NULL:   return null;
            case INT:    return i;
            case DOUBLE: return d;
            case BOOL:   return b;
            case STR:    return s;
            case HANDLE: return host.resolve(i);
            default: throw new IllegalStateException("unknown kind " + kind);
        }
    }

    /** Map a Java result back into a link value; unknown object types become handles. */
    public static SleelaValue fromJava(Object o, SleelaMemoryServer host) {
        if (o == null) return NULL;
        if (o instanceof Boolean v)   return ofBool(v);
        if (o instanceof Long v)      return ofInt(v);
        if (o instanceof Integer v)   return ofInt(v.longValue());
        if (o instanceof Short v)     return ofInt(v.longValue());
        if (o instanceof Byte v)      return ofInt(v.longValue());
        if (o instanceof Double v)    return ofDouble(v);
        if (o instanceof Float v)     return ofDouble(v.doubleValue());
        if (o instanceof String v)    return ofStr(v);
        // Any other object lives on the Java 28 heap and is exposed as a handle.
        return ofHandle(host.retain(o));
    }

    /**
     * Parse one wire token into a value. A {@code parseHandleContext} is needed
     * only for validation errors; parsing itself is context-free.
     */
    public static SleelaValue decode(String token) {
        if (token == null || token.isEmpty()) {
            throw new IllegalArgumentException("empty value token");
        }
        if (token.equals("n")) return NULL;
        int colon = token.indexOf(':');
        if (colon < 0) {
            throw new IllegalArgumentException("not a typed value token: '" + token + "'");
        }
        char tag = token.charAt(0);
        String rest = token.substring(colon + 1);
        switch (tag) {
            case 'i': return ofInt(Long.parseLong(rest));
            case 'd': return ofDouble(Double.parseDouble(rest));
            case 'b': return ofBool(rest.equals("1"));
            case 'h': return ofHandle(Long.parseLong(rest));
            case 's': {
                // token form is s:<len>:<utf8>; rest is "<len>:<utf8>"
                int c2 = rest.indexOf(':');
                if (c2 < 0) throw new IllegalArgumentException("malformed string token");
                return ofStr(rest.substring(c2 + 1));
            }
            default:  throw new IllegalArgumentException("unknown value tag '" + tag + "'");
        }
    }

    @Override public String toString() { return encode(); }
}
