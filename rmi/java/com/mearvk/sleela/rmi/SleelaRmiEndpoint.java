package com.mearvk.sleela.rmi;

import java.util.Objects;

/** Immutable address of a SLeeLa RMI service. */
public record SleelaRmiEndpoint(String host, int registryPort, String serviceName) {
    public SleelaRmiEndpoint {
        Objects.requireNonNull(host, "host");
        Objects.requireNonNull(serviceName, "serviceName");
        if (host.isBlank()) {
            throw new IllegalArgumentException("host must not be blank");
        }
        if (serviceName.isBlank()) {
            throw new IllegalArgumentException("serviceName must not be blank");
        }
        if (registryPort < 1 || registryPort > 65535) {
            throw new IllegalArgumentException("registryPort must be 1..65535");
        }
    }

    public static SleelaRmiEndpoint local(String serviceName, int registryPort) {
        return new SleelaRmiEndpoint("127.0.0.1", registryPort, serviceName);
    }
}
