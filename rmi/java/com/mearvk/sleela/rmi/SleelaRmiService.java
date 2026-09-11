package com.mearvk.sleela.rmi;

import com.mearvk.sleela.gui.SleelaRuntime;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.Objects;

/** Exports SLeeLa business operations through standard Java RMI. */
public final class SleelaRmiService extends UnicastRemoteObject implements SleelaRemote {
    private final String name;
    private final SleelaRuntime runtime;

    public SleelaRmiService(String name, SleelaRuntime runtime) throws RemoteException {
        super(0);
        this.name = Objects.requireNonNull(name, "name");
        this.runtime = Objects.requireNonNull(runtime, "runtime");
    }

    @Override
    public String serviceName() {
        return name;
    }

    @Override
    public String invoke(String operation, String arguments) throws RemoteException {
        Objects.requireNonNull(operation, "operation");
        Object result = runtime.call(operation, arguments == null ? "" : arguments);
        return result == null ? "" : String.valueOf(result);
    }

    @Override
    public String health() {
        return "SLeeLa RMI service " + name + " = healthy";
    }
}
