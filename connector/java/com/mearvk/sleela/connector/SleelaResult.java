package com.mearvk.sleela.connector;

import java.util.Objects;

/** Transport-neutral result returned to a Java host. */
public record SleelaResult(boolean success, String value, String error) {
    public SleelaResult {
        value = value == null ? "" : value;
        error = error == null ? "" : error;
        if (success && !error.isEmpty()) {
            throw new IllegalArgumentException("successful results cannot contain an error");
        }
        if (!success && error.isEmpty()) {
            throw new IllegalArgumentException("failed results must contain an error");
        }
    }

    public static SleelaResult success(String value) {
        return new SleelaResult(true, Objects.requireNonNullElse(value, ""), "");
    }

    public static SleelaResult failure(String error) {
        return new SleelaResult(false, "", Objects.requireNonNull(error, "error"));
    }
}
