package implementations.one.bodi;

import java.rmi.Naming;
import java.rmi.Remote;

public class Bodi
{
    public Bodi bodi;

    public Bodi reference;

    public BodiRMIExtender extender002 = new BodiRMIExtender();

    public BodiFunctionalExtender extender001 = new BodiFunctionalExtender();

    //

    static
    {
        try
        {
            Naming.rebind("::",null);
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }
    }

    public static void run(String protocol, String bodiref, String methodname)
    {

    }

    public static void run(String protocol, String bodiref, String methodname, StackTraceElement[] stack)
    {

    }

    //

    public static Remote pull(String bodiref)
    {
        return BodiFunctionalExtender.pull(bodiref);
    }

    public static void push(String bodiref, Remote remote)
    {
        BodiFunctionalExtender.push(bodiref, remote);
    }
}
