package implementations._001.drivers;

import implementations._001.exceptions.NordshriftExceptions;

public class Startup
{
    Startup startup;

    public static void main(String...args)
    {
        try
        {
            Class.forName("implementations._001.nordshrift.Nordshrift").newInstance();
        }
        catch(Exception exception)
        {
            NordshriftExceptions.relist(exception, "no://earth");
        }
    }
}
