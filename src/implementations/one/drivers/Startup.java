package implementations.one.drivers;

import implementations.one.exceptions.NordshriftExceptions;

public class Startup
{
    Startup startup;

    public static void main(String...args)
    {
        try
        {
            Class.forName("implementations.one.nordshrift.Nordshrift").newInstance();
        }
        catch(Exception exception)
        {
            NordshriftExceptions.relist(exception, "no://earth");
        }
    }
}
