package implementations.one.nordshrift;

import implementations.one.bodi.Bodi;
import implementations.one.nordshrift.events.RegisterableNordshriftEvent;

import java.rmi.Remote;

public class NordshriftStartup implements Remote
{
    public static void main(String...args)
    {
        NordshriftStartup startup001 = new NordshriftStartup();
    }

    public NordshriftStartup()
    {
        Bodi.reference.push("::", this, RegisterableNordshriftEvent.class);
    }
}

/**
 *         java.lang.System.out.println("[ns:nordshrift]  Nordshrift Guarded Startup™ ");
 *
 *         try
 *         {
 *             Class.forName("implementations.one.nordshrift.drivers.NordshriftDriver").newInstance();
 *         }
 *         catch (Exception exception)
 *         {
 *             java.lang.System.out.println(exception);
 *         }
 */
