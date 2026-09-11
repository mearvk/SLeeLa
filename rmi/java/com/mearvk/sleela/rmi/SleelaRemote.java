package com.mearvk.sleela.rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;

/**
 * Java RMI contract for a SLeeLa service.
 *
 * SLeeLa is a peer to Java here: Java supplies the standard JRMP/RMI transport
 * and registry machinery while the remote operation remains SLeeLa business logic.
 */
public interface SleelaRemote extends Remote {
    String serviceName() throws RemoteException;

    String invoke(String operation, String arguments) throws RemoteException;

    String health() throws RemoteException;
}
