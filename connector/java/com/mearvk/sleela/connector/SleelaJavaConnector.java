package com.mearvk.sleela.connector;

/**
 * Common Java-side integration contract for SLeeLa.
 *
 * Java applications use this interface without depending on the selected
 * transport. Implementations may use a local process, Java RMI, or HTTP.
 */
public interface SleelaJavaConnector extends AutoCloseable {
    SleelaResult invoke(SleelaInvocation invocation) throws Exception;

    default SleelaResult invoke(String operation, String arguments) throws Exception {
        return invoke(new SleelaInvocation(operation, arguments));
    }

    /** Lightweight transport-level liveness check. */
    String health() throws Exception;

    default boolean isHealthy() {
        try {
            health();
            return true;
        } catch (Exception failure) {
            return false;
        }
    }

    @Override
    default void close() throws Exception {
        // Stateless connectors have nothing to release.
    }
}
