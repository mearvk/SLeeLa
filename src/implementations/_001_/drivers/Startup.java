package implementations._001_.drivers;

import implementations._001_.exceptions.NordshriftExceptions;

public class Startup
{
    Startup startup;

    public static void main(String...args)
    {
        try
        {
            Class.forName("implementations._001_.nordshrift.Nordshrift").newInstance();
        }
        catch(Exception exception)
        {
            NordshriftExceptions.relist(exception, "no://earth");
        }
    }
}
