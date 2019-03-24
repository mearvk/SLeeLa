package implementations._001_.nordshrift;

import implementations._001_.bodi.Bodi;
import implementations._001_.nordshrift.events.RegisterableNordshriftEvent;

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
