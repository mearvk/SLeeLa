package implementations._001_.bodi;

import java.rmi.Remote;

/** Static compatibility facade. New code should use Bodi.system(...). */
public class BodiFunctionalExtender
{
    public static Remote pull(String bodiref)
    {
        return Bodi.reference == null ? null : Bodi.reference.pull(bodiref);
    }

    public static void push(String bodiref, Remote remote)
    {
        if (Bodi.reference != null)
            Bodi.reference.push(bodiref, remote);
    }

    public static BodiObjectReference system(String name)
    {
        return Bodi.system(name);
    }
}
