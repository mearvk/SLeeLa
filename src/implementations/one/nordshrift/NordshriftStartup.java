package implementations.one.nordshrift;

public class NordshriftStartup
{
    public static void main(String...args)
    {
        java.lang.System.out.println("[ns:nordshrift]  Nordshrift™ Guarded Startup ");

        try
        {
            Class.forName("implementations.one.nordshrift.factory.NordshriftDriver").newInstance();
        }
        catch (Exception exception)
        {
            java.lang.System.err.println(exception);
        }
    }
}
