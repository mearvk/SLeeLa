package com.mearvk.sleela.gui;

import java.util.Objects;

/**
 * Path 3 Java facade for the native GUI/runtime bridge.
 * The native implementation can bind this facade to slcore_exchange().
 */
public final class JniSleelaRuntime implements SleelaRuntime {
    static {
        // The deployment may provide the library as sleela_gui_bridge.
        // Loading is intentionally opt-in through the constructor.
    }

    private final long nativeHandle;

    public JniSleelaRuntime(long nativeHandle) {
        if (nativeHandle == 0) throw new IllegalArgumentException("nativeHandle");
        this.nativeHandle = nativeHandle;
    }

    @Override
    public Object call(String operation, Object... arguments) {
        Objects.requireNonNull(operation, "operation");
        return nativeCall(nativeHandle, operation, arguments == null ? new Object[0] : arguments);
    }

    private static native Object nativeCall(long handle, String operation, Object[] arguments);

    @Override
    public void close() {
        // Native ownership/lifecycle is controlled by the host that created the handle.
    }
}
