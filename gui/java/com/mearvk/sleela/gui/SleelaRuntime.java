package com.mearvk.sleela.gui;

import java.util.Objects;

/**
 * Small Java-side runtime boundary for invoking SLeeLa application logic.
 *
 * Path 1 uses a Java host adapter: Java owns the process and asks a SLeeLa
 * program to perform business operations. The actual VM binding can later be
 * supplied by the native runtime without changing the GUI API.
 */
public interface SleelaRuntime extends AutoCloseable {
    Object call(String operation, Object... arguments);

    @Override
    void close();

    static SleelaRuntime unsupported() {
        return new SleelaRuntime() {
            @Override
            public Object call(String operation, Object... arguments) {
                Objects.requireNonNull(operation, "operation");
                throw new UnsupportedOperationException(
                    "No SLeeLa runtime binding installed for: " + operation);
            }

            @Override
            public void close() {
            }
        };
    }
}
