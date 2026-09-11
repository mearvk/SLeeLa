package com.mearvk.sleela.rmi;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Objects;

/** Client facade for a SLeeLa service exported through standard Java RMI. */
public final class SleelaRmiClient implements AutoCloseable {
    private final SleelaRemote remote;

    public SleelaRmiClient(String host, int port, String serviceName) throws Exception {
        Objects.requireNonNull(host, "host");
        Objects.requireNonNull(serviceName, "serviceName");
        Registry registry = LocateRegistry.getRegistry(host, port);
        this.remote = (SleelaRemote) registry.lookup(serviceName);
    }

    public String serviceName() throws Exception {
        return remote.serviceName();
    }

    public String invoke(String operation, String arguments) throws Exception {
        return remote.invoke(operation, arguments);
    }

    public String health() throws Exception {
        return remote.health();
    }

    @Override
    public void close() {
        // Registry lookup returns a proxy; no local export is owned here.
    }
}
