package implementations._001.bodi;

import java.rmi.Remote;

public class BodiRMIExtender
{
    public Remote pull(String bodiref) throws Exception
    {
        return Bodi.registry001.lookup(bodiref);
    }

    public void push(String bodiref, Remote remote) throws Exception
    {
        Bodi.registry001.bind(bodiref, remote);
    }
}
