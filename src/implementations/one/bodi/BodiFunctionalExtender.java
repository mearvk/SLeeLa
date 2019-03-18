package implementations.one.bodi;

import java.rmi.Naming;
import java.rmi.Remote;

public class BodiFunctionalExtender
{
    public static Remote pull(String bodiref)
    {
        return null;
    }

    public static void push(String bodiref, Remote remote)
    {
        try
        {
            //Naming.rebind(bodiref, remote);
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }
    }
}
