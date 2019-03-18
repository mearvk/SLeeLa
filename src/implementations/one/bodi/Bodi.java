package implementations.one.bodi;

import java.rmi.Remote;

public class Bodi
{
    public Bodi bodi = this;

    public Bodi reference = this;

    //

    public static void run(String protocol, String bodiref, String methodname)
    {

    }

    public static void run(String protocol, String bodiref, String methodname, StackTraceElement[] stack)
    {

    }

    //

    public static Remote pull(String bodiref)
    {
        return null;
    }

    public static Remote push(String bodiref, Remote remote)
    {
        return null;
    }
}
