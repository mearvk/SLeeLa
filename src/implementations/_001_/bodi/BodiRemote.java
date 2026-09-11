package implementations._001_.bodi;

import java.rmi.Remote;
import java.rmi.RemoteException;

/** Minimal Java RMI contract retained beneath the preferred Bodi API. */
public interface BodiRemote extends Remote
{
    Remote pull(String bodiref) throws RemoteException;
    void push(String bodiref, Remote remote) throws RemoteException;
}
