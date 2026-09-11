package com.mearvk.sleela.rmi;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Objects;

/** Client-side facade over a SLeeLa Java RMI remote proxy. */
public final class SleelaRmiClient implements AutoCloseable {
    private final SleelaRmiEndpoint endpoint;
    private final SleelaRemote remote;

    public SleelaRmiClient(SleelaRmiEndpoint endpoint) throws Exception {
        this.endpoint = Objects.requireNonNull(endpoint, "endpoint");
        Registry registry = LocateRegistry.getRegistry(endpoint.host(), endpoint.registryPort());
        this.remote = (SleelaRemote) registry.lookup(endpoint.serviceName());
    }

    public SleelaRmiClient(String host, int port, String serviceName) throws Exception {
        this(new SleelaRmiEndpoint(host, port, serviceName));
    }

    public SleelaRmiEndpoint endpoint() {
        return endpoint;
    }

    public String serviceName() throws Exception {
        return remote.serviceName();
    }

    public String invoke(String operation, String arguments) throws Exception {
        Objects.requireNonNull(operation, "operation");
        return remote.invoke(operation, arguments == null ? "" : arguments);
    }

    public String health() throws Exception {
        return remote.health();
    }

    /** Returns true only when the remote endpoint answers its health call. */
    public boolean isHealthy() {
        try {
            remote.health();
            return true;
        } catch (Exception failure) {
            return false;
        }
    }

    @Override
    public void close() {
        // A client lookup does not own the remote export or the registry.
    }
}
