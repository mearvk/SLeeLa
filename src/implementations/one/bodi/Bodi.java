package implementations.one.bodi;

import java.rmi.Remote;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class Bodi implements Remote
{
    public static Bodi reference = new Bodi();

    public Bodi bodi = reference;

    //

    public BodiRMIExtender extender001 = new BodiRMIExtender();

    //

    public static Registry registry001;

    public static Registry registry002;

    //

    static
    {
        System.out.println("  [ns::bodi]       Bodi Registry™ initialized");

        try
        {
            Bodi.registry001 = LocateRegistry.createRegistry(8888);

            Bodi.registry001.bind("@bodi", Bodi.reference);
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

    public Remote pull(String bodiref)
    {
        try
        {
            return this.extender001.pull(bodiref);
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }

        return null;
    }

    public void push(String bodiref, Remote remote)
    {
        try
        {
            this.extender001.push(bodiref, remote);
        }
        catch (Exception exception)
        {
            java.lang.System.out.println(exception);
        }
    }
}
