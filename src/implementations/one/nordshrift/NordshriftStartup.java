package implementations.one.nordshrift;

public class NordshriftStartup
{
    public static void main(String...args)
    {
        try
        {
            Class.forName("implementations.one.nordshrift.drivers.NordshriftDriver").newInstance();
        }
        catch (Exception exception)
        {
            java.lang.System.err.println(exception);
        }
    }
}
