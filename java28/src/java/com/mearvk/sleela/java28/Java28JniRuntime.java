package com.mearvk.sleela.java28;

/**
 * JNI (local feedback) channel for the Java 28 SecureJDK memory link
 * (J28-MEM-0001 §5). Same object memory model and grammar as the port channel,
 * but in-process: no socket, single address space.
 *
 * <p>Direction of control (Model A): a Sleela-side native driver runs the loop
 * and issues request lines; the native bridge calls back into this runtime's
 * {@link #dispatch(String)}, which funnels to the shared
 * {@link SleelaMemoryServer#handleLine(String)}. Because both channels end at
 * {@code handleLine}, their behavior is identical by construction.
 *
 * <p>{@link #run(long)} enters the native driver (which drives the demo
 * sequence and returns), passing an opaque handle to this runtime so the native
 * side can call {@link #dispatch(String)} back.
 */
public final class Java28JniRuntime {

    static {
        // Library name resolves to libsleela_java28.so / .dylib on the path
        // supplied via -Djava.library.path.
        System.loadLibrary("sleela_java28");
    }

    private final SleelaMemoryServer host;

    public Java28JniRuntime() {
        this.host = new SleelaMemoryServer();
        this.host.registerClass(Counter.class);
        this.host.registerClass(Ledger.class);
    }

    public Java28JniRuntime(SleelaMemoryServer host) {
        this.host = host;
    }

    /**
     * The local-feedback entry point. Called by the native bridge with one
     * request line; returns one response line. This is the JNI analogue of the
     * port server's per-line handler.
     */
    public String dispatch(String requestLine) {
        return host.handleLine(requestLine);
    }

    /** Enter the native driver, which will call {@link #dispatch} back in-process. */
    public native int runNativeDemo();

    public static void main(String[] args) {
        Java28JniRuntime rt = new Java28JniRuntime();
        System.out.println("== Sleela driving the Java 28 SecureJDK memory model (JNI local feedback) ==");
        int rc = rt.runNativeDemo();
        System.out.println("Java 28 memory demo (JNI): " + (rc == 0 ? "PASS" : "FAIL rc=" + rc));
        System.exit(rc);
    }
}
