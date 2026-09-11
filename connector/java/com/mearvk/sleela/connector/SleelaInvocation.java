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
}
