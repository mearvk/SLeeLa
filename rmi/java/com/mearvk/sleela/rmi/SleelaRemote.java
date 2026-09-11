package com.mearvk.sleela.rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;

/** Stable Java RMI contract presented by a SLeeLa service. */
public interface SleelaRemote extends Remote {
    String serviceName() throws RemoteException;

    /** Invoke one bounded SLeeLa operation using textual arguments. */
    String invoke(String operation, String arguments) throws RemoteException;

    /** Lightweight liveness/identity check. */
    String health() throws RemoteException;
}
