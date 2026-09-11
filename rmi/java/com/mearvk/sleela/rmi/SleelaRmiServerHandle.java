package com.mearvk.sleela.rmi;

import com.mearvk.sleela.gui.SleelaRuntime;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Objects;

/** Owns the complete server-side RMI lifecycle. */
public final class SleelaRmiServerHandle implements AutoCloseable {
    private final Registry registry;
    private final SleelaRmiService service;
    private final String serviceName;
    private final int registryPort;
    private boolean closed;

    private SleelaRmiServerHandle(Registry registry, SleelaRmiService service,
                                   String serviceName, int registryPort) {
        this.registry = registry;
        this.service = service;
        this.serviceName = serviceName;
        this.registryPort = registryPort;
    }

    public static SleelaRmiServerHandle start(String serviceName, int registryPort,
                                              SleelaRuntime runtime) throws RemoteException {
        Objects.requireNonNull(serviceName, "serviceName");
        Objects.requireNonNull(runtime, "runtime");
        if (registryPort < 1 || registryPort > 65535) {
            throw new IllegalArgumentException("registryPort must be 1..65535");
        }

        Registry registry = LocateRegistry.createRegistry(registryPort);
        SleelaRmiService service = new SleelaRmiService(serviceName, runtime);
        registry.rebind(serviceName, service);
        return new SleelaRmiServerHandle(registry, service, serviceName, registryPort);
    }

    public String serviceName() {
        return serviceName;
    }

    public int registryPort() {
        return registryPort;
    }

    public String health() throws RemoteException {
        return service.health();
    }

    public synchronized boolean isClosed() {
        return closed;
    }

    @Override
    public synchronized void close() {
        if (closed) {
            return;
        }
        try {
            registry.unbind(serviceName);
        } catch (Exception ignored) {
            // Shutdown is best-effort if the registry has already gone away.
        }
        try {
            UnicastRemoteObject.unexportObject(service, true);
        } catch (Exception ignored) {
            // Already unexported.
        }
        closed = true;
    }
}
