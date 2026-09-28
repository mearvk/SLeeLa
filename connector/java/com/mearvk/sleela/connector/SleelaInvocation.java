package com.mearvk.sleela.connector;

import java.util.Objects;

/** Immutable request sent from a Java host into SLeeLa. */
public record SleelaInvocation(String operation, String arguments) {
    public SleelaInvocation {
        Objects.requireNonNull(operation, "operation");
        if (operation.isBlank()) {
            throw new IllegalArgumentException("operation must not be blank");
        }
        arguments = arguments == null ? "" : arguments;
    }

    /** An invocation of {@code operation} with no arguments. */
    public static SleelaInvocation of(String operation) {
        return new SleelaInvocation(operation, "");
    }

    /** An invocation of {@code operation} with a single argument string. */
    public static SleelaInvocation of(String operation, String arguments) {
        return new SleelaInvocation(operation, arguments);
    }
}
